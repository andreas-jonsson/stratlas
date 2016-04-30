/* WAR file loader
   used by Warcraft 1 / Warcraft 2 / expansion (NOT BNE!) */

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "hlp_war.h"

struct archive_war *openWAR(const char *filename)
{
  struct archive_war *war=NULL;
  unsigned char *cp;
//	unsigned char** op;
  unsigned int i;
  FILE *fp;

  war = (struct archive_war*)malloc(sizeof(struct archive_war));
  if (war == NULL) return NULL;

  fp = fopen(filename,"rb");
// failed to open file - probably wrong filename : (
if (fp == NULL) return NULL;
  fseek(fp,0,SEEK_END);
  war->filesize = ftell(fp);
  rewind(fp);
  war->data = (unsigned char*)malloc(war->filesize);
  if (war->data == NULL) { free(war); return NULL; }
  fread(war->data,1,war->filesize,fp);
  fclose(fp);

  cp = war->data;
	war->magicnumber = FetchLE32(cp);
/*	if (i != 0x19 && i != 0x18) {
		printf("Wrong magic %08x, expected %08x or %08x\n",
			i, 0x00000019, 0x00000018);
			free(war->data); free(war); return NULL;
	}*/
	war->entries = FetchLE16(cp);
	war->type = FetchLE16(cp);
/*	if (i != type) {
		printf("Wrong type %08x, expected %08x\n", i, type);
		exit(-1);
	}*/

	//
	//  Read offsets.
	//
	war->op = (unsigned char**)malloc((war->entries + 1) * sizeof(unsigned char**));
	if (!war->op) {
             free(war->data);
             free(war);
             return NULL;
	}
	for (i = 0; i < war->entries; ++i) {
		war->op[i] = war->data + FetchLE32(cp);
	}
	war->op[i] = war->data + war->filesize;
	return war;
}

/**
**  Extract/uncompress entry.
**
**  @param cp    Pointer to compressed entry
**  @param lenp  Return pointer of length of the entry
**
**  @return      Pointer to uncompressed entry
*/
unsigned char *extractWAR(archive_war* war, unsigned int entrynum, unsigned int* lenp)
{
	unsigned char* dp = NULL;
	unsigned char* dest = NULL;
	unsigned int uncompressed_length;
	int flags;

    unsigned char *cp = war->op[entrynum];

	uncompressed_length = FetchLE32(cp);
	flags = uncompressed_length >> 24;
	uncompressed_length &= 0x00FFFFFF;

// This check was actually wrong and didn't make sense, it was causing
//  errors when using an Orcish submarine on a Swamp map (expansion)
/*
	if (uncompressed_length + (cp - war->data) > war->filesize) {
		printf("Entry goes past end of file\n");
		return NULL;
	}
*/

	dp = dest = (unsigned char*)malloc(uncompressed_length);
	if (!dest) {
		printf("Can't malloc %d\n", uncompressed_length);
		exit(-1);
	}

	if (flags == 0x20) {
		unsigned char buf[4096];
		unsigned char* ep;
		int bi;

		bi = 0;
		memset(buf, 0, sizeof(buf));
		ep = dp + uncompressed_length;

		// FIXME: If the decompression is too slow, optimise this loop :->
		while (dp < ep) {
			int i;
			int bflags;

			bflags = FetchByte(cp);
			for (i = 0; i < 8; ++i) {
				int j;
				int o;

				if (bflags & 1) {
					j = FetchByte(cp);
					*dp++ = j;
					buf[bi++ & 0xFFF] = j;
				} else {
					o = FetchLE16(cp);
					j = (o >> 12) + 3;
					o &= 0xFFF;
					while (j--) {
						buf[bi++ & 0xFFF] = *dp++ = buf[o++ & 0xFFF];
						if (dp == ep) {
							break;
						}
					}
				}
				if (dp == ep) {
					break;
				}
				bflags >>= 1;
			}
		}
		//if (dp!=ep ) printf("%p,%p %d\n",dp,ep,dp-dest);
	} else if (flags == 0x00) {
		memcpy(dest, cp, uncompressed_length);
	} else {
		printf("Unknown flags %x\n", flags);
		free(dest);
		return NULL;
	}

	if (lenp) {						// return resulting length
		*lenp = uncompressed_length;
	}

	return dest;
}

/**
**  Close the archive file.
*/
void closeWAR(archive_war* war)
{
     if (war != NULL) {
    free(war->data);
    free(war->op);
    free(war);
//    war=NULL;
}
}
