/* hlp_mix.h
loader for MIX archives
C&C: Tiberian Dawn / Red Alert / Tiberian Sun */

#ifndef HLP_MIX_H_
#define HLP_MIX_H_

#include "../common.h"

struct archive_mix
{
  unsigned char game;
  short entries;
  int size;
//  unsigned char hasChecksum, isEncrypted;
//  unsigned char key_source[80];
  unsigned int *entry_id;
  int *entry_offset;
  int *entry_size;
  unsigned char* data;
};

struct archive_mix *openMIX(const char*,unsigned char);
unsigned char *extractMIX(archive_mix* MIX, char *filename);
//unsigned char *extractMIX_byid(archive_mix* MIX, unsigned int);
unsigned char *extractMIX_rawptr(archive_mix* MIX, unsigned int);
void closeMIX(struct archive_mix*);

#endif
