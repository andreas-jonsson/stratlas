/* in_warcraft.cpp
   copyright 2008 Greg Kennedy <kennedy.greg@gmail.com>
   released under the GNU GPL v3 */

#include "in_warcraft.h"

struct cfg_war1_struct war1_cfg;

unsigned short unit_lookup[] =
{ 279,280,281,282,283,284,285,286,287,288,
  289,290,291,292,293,294,296,296,306,297,
  301,298,299,  0,301,302,303,304,305,  0,
    // bldgs
    0,  0,307,308,309,310,311,312,313,314,
  315,316,317,318,319,320,321,322,323,324,
  325,  0,
  327,328,329,330
};
// 300 fire elem

unsigned int unitx[56]={0};
unsigned int unity[56]={0};

unsigned char *unitgfx[56] = {NULL};

unsigned char race[2]={0,0};

int initWarcraft()
{
	strcpy(war1_cfg.war1_dir,"C:\\WARCRAFT");
	return 0;
}

/* function to check for Warcraft type */   
int isWarcraft(struct MapFile file)
{
    // A Warcraft SAV is a mess, but it does consistently have War1C at 0x28
    if (strcmp("War1C",(char *)&file.data[40]) == 0) return 1;
    return 0;
}

/* Warcraft config - reads WAR1_DIR */
int cfgWarcraft(char *line)
{
    if (strstr(line,"WAR1_DIR ")) { strncpy(war1_cfg.war1_dir,(&line[9]),strlen(line)-10); war1_cfg.war1_dir[strlen(line)-10]='\0'; return 1; }
    return 0;
}

void makeWar1Tile(unsigned char *tile,unsigned short *megatile,unsigned char *minitiles)
{
     int s,t, x,y, Offset,flipx,flipy,index;

	static const int flip[] = {
		7, 6, 5, 4, 3, 2, 1, 0, 8
	};

     for (t=0; t<2; t++)
     for (s=0; s<2; s++)
     {
      Offset = ConvertLE16(megatile[s+t*2]);
      flipx = Offset & 2;
      flipy = Offset & 1;
      index = (Offset & 0xFFFC) << 1;
      
	for (y = 0; y < 8; ++y) {
		for (x = 0; x < 8; ++x) {
			tile[(128*t) + (16*y) + (8*s) + x] = minitiles[index +
				(flipy ? flip[y] : y) * 8 + (flipx ? flip[x] : x)];
		}
	}
     }
}

unsigned char *loadUnit(unsigned char type, struct archive_war* war)
{
  unsigned int size,i;
  unsigned short entry = unit_lookup[type];
  unsigned char *bp,*sp,*dp,*image,*buffer = extractWAR(war,entry,&size);
  bp = buffer;
  	int count;
//	int length;
	int max_width;
	int max_height;

	count = FetchLE16(bp);
	max_width = FetchByte(bp);
	max_height = FetchByte(bp);

	image = (unsigned char*)malloc(max_width * max_height);
	
	int xoff = FetchByte(bp);
	int yoff = FetchByte(bp);
	int width = FetchByte(bp);
	unsigned int height = FetchByte(bp);
	int offset = FetchLE32(bp);
	if (offset < 0) {  // High bit of width
		offset &= 0x7FFFFFFF;
		width += 256;
	}

    memset(image,0,max_width*max_height);
	sp = buffer + offset;// - 4;
	dp = image + xoff + (yoff * max_width);
	for (i = 0; i < height; ++i) {
		memcpy(dp, sp, width);
		dp += max_width;
		sp += width;
	}
	
	unitx[type]=max_width;//max_width;
	unity[type]=max_height;

  free(buffer);
  return image;
}

int mul16(int blah)
{
  blah = blah / 16;
  return blah * 16;
};

void blitUnit(unsigned char *data, unsigned short type, unsigned short x, unsigned short y, unsigned char owner, struct archive_war *war)
{
     unsigned int i,j;
	int xfix=0,yfix=0;
     unsigned char pixel;
     if (unitgfx[type] == NULL) unitgfx[type] = loadUnit(type,war);
     
     xfix = -8;//xoffsets[type];
     yfix = -8;//yoffsets[type];
     if (type == 0 || type == 7 || type == 19) { xfix-=8; yfix-=8; }
     for(j=0;j<unity[type];j++)
     for(i=0;i<unitx[type];i++)
     {
       if (i+xfix+(16*x) >= 0 && j+yfix+(16*y) >= 0 && i+xfix+(16*x) < 1024 && j+yfix+(16*y) < 1024 && unitgfx[type][unitx[type]*j+i] != 0)
       {
         pixel = unitgfx[type][unitx[type]*j+i];
// correcting colors for ownership
// owner == 0 for human, 1 for computer
         if (pixel >= 176 && pixel <= 183 && owner && race[0]==race[1]) pixel += 24;
         else if (pixel >= 200 && pixel <= 207 && owner && race[0]==race[1]) pixel -= 24;
         data[(16384*y) + (1024 * (yfix+j)) + (16 * x) + i+xfix] = pixel;
       }
     }
}

/* render a Warcraft map */
struct Image* renderWarcraft(struct MapFile file)
{
    struct Image *rendered = NULL;
    struct archive_war *war;
    unsigned short tilemap[64][64];
    unsigned char *map_p,*buffer,*maptiles,*megatiles;
    unsigned int i,j,x,y, size, numtiles=0,tilesetindex=189;
    
    struct war1unit units[200];

//    FILE *fpUnits = fopen("units.dat","w");

    rendered = (struct Image*) malloc (sizeof (struct Image));
    if (rendered == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); return NULL; }

    // Warcraft maps are always 64x64 tiles, 16 pixels/tile, 8bpp
    rendered->x = 1024;
    rendered->y = 1024;
    rendered->bpp = 8;
    rendered->imgdata = (unsigned char*)malloc(1048576);

    if (rendered->imgdata == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); free(rendered); return NULL; }

    { char filename[256]; strcpy(filename,war1_cfg.war1_dir); strcat(filename,"/DATA.WAR");
printf(" WAR1: Using '%s' for WAR file\n", filename);
    war = openWAR(filename);
}
    if (war == NULL) { fprintf(stderr,"couldn't open WAR\n"); free(rendered->imgdata); free(rendered); return NULL; }

    if (isWarcraft(file) == 1) {
      /* this is a SAV file */
      map_p = &file.data[4962];
      for (j=0; j<64; j++)
        for (i=0; i<64; i++)
          tilemap[i][j] = FetchLE16(map_p) & 0x0FFF; // road, wall and ownership info in high 4 bits
          
      // Read data on 200 units.
      i=0;
      map_p = &file.data[45926];
      for (map_p = &file.data[45926];map_p < &file.data[0xFB46];map_p += 92)
      {
          if (AccessLE16(map_p) != 0) {
            units[i].type = map_p[23];
            units[i].x = AccessLE16(&map_p[6]) % 64; // sanity check
            units[i].y = AccessLE16(&map_p[8]) % 64; // sanity check
            units[i].hp = AccessLE16(&map_p[18]);
            units[i].gold = AccessLE16(&map_p[80]);
            units[i].owner = map_p[26];
            if (units[i].type < 22 || units[i].type > 31) race[units[i].owner] = units[i].type % 2;
            else if (units[i].type > 21 && units[i].type < 29) race[units[i].owner] = 1;
//            fprintf(fpUnits,"%d - (%d,%d) - %d hp - %d gold\n",units[i].type,units[i].x,units[i].y,units[i].hp,units[i].gold);
          } else {
                 units[i].type = -1;
//            fprintf(fpUnits,"no unit\n");
          }

          i++;
      }
//          fprintf(fpUnits,"OK!\n");

// TODO: this should be selected properly from file.
//      tilesetindex=189; // 189 forest, 192 swamp, 195 dungeon
        tilesetindex = AccessLE16(&file.data[0x067E])-2;
    } else {
 fprintf(stderr,"file is not a WARCRAFT SAV format\n"); free(rendered->imgdata); free(rendered); closeWAR(war); return NULL; 
	}

    // extract palette info
    buffer = extractWAR(war,tilesetindex+2,&size);
    if (buffer != NULL) { memcpy(rendered->palette,buffer,768); free(buffer); }
    else {
       fprintf(stderr,"couldn't extract palette from WAR\n"); closeWAR(war); free(rendered->imgdata); free(rendered); return NULL; 
     }

		unsigned char* gpalp;
		gpalp = extractWAR(war,217, NULL);
		if (gpalp != NULL) {
		for (i = 0; i < 128; ++i) {
			if (rendered->palette[i][0] == 63 && rendered->palette[i][1] == 0 &&
					rendered->palette[i][2] == 63) {
				rendered->palette[i][0] = gpalp[i * 3 + 0];
				rendered->palette[i][1] = gpalp[i * 3 + 1];
				rendered->palette[i][2] = gpalp[i * 3 + 2];
			}
		}
		for (i = 128; i < 256; ++i) {
			if (!(gpalp[i * 3 + 0] == 63 && gpalp[i * 3 + 1] == 0 &&
					gpalp[i * 3 + 2] == 63)) {
				rendered->palette[i][0] = gpalp[i * 3 + 0];
				rendered->palette[i][1] = gpalp[i * 3 + 1];
				rendered->palette[i][2] = gpalp[i * 3 + 2];
			}
		}
		free(gpalp);
    } else {
       fprintf(stderr,"couldn't extract gpalette from WAR\n"); closeWAR(war); free(rendered->imgdata); free(rendered); return NULL; 
    }

    for (i=0; i<256; i++)
    {
        rendered->palette[i][0] <<= 2;
        rendered->palette[i][1] <<= 2;
        rendered->palette[i][2] <<= 2;
    }

    // extract tileset GFX
    megatiles = extractWAR(war,tilesetindex,&size);
    if (megatiles == NULL) {
       fprintf(stderr,"couldn't extract megatiles from WAR\n"); closeWAR(war); free(rendered->imgdata); free(rendered); return NULL; }
    numtiles = size / 8;
    buffer = extractWAR(war,tilesetindex+1,&size);
    if (buffer == NULL) {
       fprintf(stderr,"couldn't extract minitiles from WAR\n"); free(megatiles); closeWAR(war); free(rendered->imgdata); free(rendered); return NULL; }

    maptiles = (unsigned char*)malloc(numtiles * 256);
    if (maptiles == NULL) {
       fprintf(stderr,"couldn't malloc for maptiles\n"); free(buffer); free(megatiles); closeWAR(war); free(rendered->imgdata); free(rendered); return NULL; }
    for (i=0; i<numtiles; i++)
    {
        makeWar1Tile(&maptiles[i*256],(unsigned short*)&megatiles[i*8],buffer);
    }
    free(megatiles);
    free(buffer);

/*    rendered->x = 16;
    rendered->y = 16*numtiles;
    memcpy(rendered->imgdata,maptiles,256*numtiles);
    return rendered;*/
//          fprintf(fpUnits,"OK done %d numtiles\n",numtiles);

    // blit map tiles to image data
    for (y=0; y<64; y++)
    for (x=0; x<64; x++)
    {
            unsigned short type = tilemap[x][y]; // Sanity check for bad tileset number
//            unsigned int type = tilemap[x][y] % numtiles; // Sanity check for bad tileset number
//          fprintf(fpUnits,"(%d,%d) = %d / ",x,y,type);

        for (j=0; j<16; j++)
        for (i=0; i<16; i++)
        {
//            rendered->imgdata[16*(1024*y + x)] = maptiles[(type * 256)];
            rendered->imgdata[(16384 * y) + (1024*j) + (16*x) + i] = maptiles[(type*256)+(j*16)+i];
        }
    }
//          fprintf(fpUnits,"OK!\n");

   // quick hack for prioritized drawing
    for (y=0; y<64; y++)
    for (i=0; i<200; i++)
    {
        if (units[i].type < 56 && units[i].y == y) blitUnit(rendered->imgdata,units[i].type, units[i].x, units[i].y,units[i].owner,war);
    }
//          fprintf(fpUnits,"OK_units!\n");
    
    for (i=0; i<56; i++)
    {
        if (unitgfx[i]) { free(unitgfx[i]); unitgfx[i] = NULL; }
    }
    
    free(maptiles);

    closeWAR(war);
//    fclose(fpUnits);

    return rendered;
}
