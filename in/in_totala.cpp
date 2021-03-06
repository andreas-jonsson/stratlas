/* in_totala.cpp
   copyright 2008 Greg Kennedy <kennedy.greg@gmail.com>
   released under the GNU GPL v3 */

#include "in_totala.h"

struct cfg_totala_struct totala_cfg;

int initTotalA()
{
//	strcpy(totala_cfg.totala_dir,"C:\\WARCRAFT");
	return 0;
}

/* function to check for TotalA type */   
int isTotalA(struct MapFile file)
{
//    if (strcmp("War1C",(char *)&file.data[40]) == 0) return 1;
    if (*(unsigned int*)(file.data) == 8192) return 1;
//    printf("%x\n",file.data);
    return 0;
}

/* TotalA config - reads TOTALA_DIR */
int cfgTotalA(char *line)
{
//    if (strstr(line,"TOTALA_DIR ")) { strncpy(totala_cfg.totala_dir,(&line[9]),strlen(line)-10); totala_cfg.totala_dir[strlen(line)-10]='\0'; return 1; }
    return 0;
}

/* render a TotalA map */
struct Image* renderTotalA(struct MapFile file)
{
    unsigned int mapx, mapy, i, j, x, y,tilenum;
    unsigned int PTRmapdata, PTRmapattr,PTRtilegfx,PTRtileanim;
    unsigned char *tile;
    struct Image *rendered = NULL;

    unsigned char totala_palette[] = {0,0,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,235,243,235,199,211,215,163,179,195,135,151,175,111,127,155,91,99,139,71,79,123,59,71,111,51,59,99,43,51,87,35,43,75,27,39,59,23,31,47,15,23,35,11,15,23,7,11,115,255,223,87,231,191,67,207,159,47,183,131,31,159,103,19,139,79,15,119,63,11,107,55,7,95,47,7,83,43,0,71,39,0,63,35,0,51,27,0,39,23,0,27,15,0,19,11,227,239,255,199,223,231,175,207,203,147,183,167,127,159,131,107,135,103,95,111,83,95,99,71,91,87,59,83,67,51,71,59,43,59,51,35,47,43,27,35,31,19,23,19,15,11,11,7,251,251,215,223,223,183,195,195,155,171,171,131,147,147,111,119,119,87,99,99,67,83,83,51,67,67,35,51,51,23,35,35,15,27,27,7,23,23,7,19,19,0,15,15,0,11,11,0,251,251,251,235,235,235,219,219,219,203,203,203,187,187,187,171,171,171,155,155,155,139,139,139,123,123,123,107,107,107,91,91,91,75,75,75,59,59,59,43,43,43,31,31,31,15,15,15,235,243,255,203,227,255,175,207,255,151,179,255,123,151,255,103,127,255,83,107,239,63,91,227,51,75,215,35,59,203,23,47,175,15,39,151,7,31,123,7,23,99,0,15,71,0,11,47,227,247,255,191,219,231,159,191,207,131,167,183,107,143,163,83,119,139,63,95,115,47,75,95,39,63,87,35,55,79,31,47,71,27,39,63,23,31,55,19,27,47,15,19,39,11,15,31,215,239,255,187,227,239,155,203,223,131,183,207,107,163,195,83,143,179,63,123,163,47,107,151,35,91,135,27,75,119,19,63,103,11,51,87,7,39,71,0,27,55,0,19,39,0,11,27,255,231,255,231,199,235,211,171,215,187,147,195,167,123,179,143,99,159,119,75,143,99,59,127,79,43,111,67,31,99,55,23,87,43,15,71,31,7,59,19,0,43,11,0,31,7,0,19,215,255,167,171,231,127,131,211,91,103,191,63,75,171,43,67,151,43,55,135,39,47,119,27,43,103,19,35,91,15,31,79,11,27,67,7,23,51,0,15,39,0,11,27,0,7,15,0,255,227,159,227,199,115,203,175,83,179,151,63,155,131,47,131,111,35,107,91,23,83,71,15,75,59,11,67,51,7,59,43,7,55,35,0,47,27,0,39,19,0,31,15,0,27,11,0,255,255,163,251,243,131,247,227,103,243,211,79,239,187,51,239,167,27,235,143,19,231,123,15,223,79,7,215,35,0,191,31,0,167,27,0,147,23,0,123,19,0,99,19,0,79,15,0,255,255,0,255,191,0,255,131,0,255,71,0,211,43,0,171,23,0,127,7,0,87,0,0,223,203,255,187,159,223,155,119,191,127,87,159,103,59,127,75,35,95,51,19,63,27,7,31,211,219,255,135,159,247,67,111,239,23,71,231,11,43,187,7,23,143,0,7,99,0,0,55,123,255,119,83,223,79,51,191,43,27,159,19,27,127,11,23,95,7,19,63,0,11,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,36,255,0,255,255,255};

    if (isTotalA(file) == 1) {
      mapx = *(unsigned int*)(&file.data[4]) / 2;
      mapy = *(unsigned int*)(&file.data[8]) / 2;
      PTRmapdata = *(unsigned int*)(&file.data[12]);
      PTRmapattr = *(unsigned int*)(&file.data[16]);
      PTRtilegfx = *(unsigned int*)(&file.data[20]);
      PTRtileanim = *(unsigned int*)(&file.data[0x20]);
      printf(" TotalA: map is %d by %d with %d tiles\n",mapx,mapy,*(unsigned int*)(&file.data[0x18]));

    rendered = (struct Image*) malloc (sizeof (struct Image));
    if (rendered == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); return NULL; }

    // TotalA maps are width by height, 32x32 tiles, 8bpp I think
    rendered->x = mapx * 32;
    rendered->y = mapy * 32;
    rendered->bpp = 8;
    rendered->imgdata = (unsigned char*)malloc(mapx * mapy * 32 * 32);

    if (rendered->imgdata == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); free(rendered); return NULL; }

   for (i=0; i<256; i++)
  {
    rendered->palette[i][0] = totala_palette[3*i];
    rendered->palette[i][1] = totala_palette[3*i + 1];
    rendered->palette[i][2] = totala_palette[3*i + 2];
  }

   for (y=0; y<mapy; y++)
   for (x=0; x<mapx; x++)
   {
//     tile = &file.data[file.data[1 + PTRmapdata + 2*(x + (mapx * y))] * 256 + file.data[PTRmapdata + 2*(x + (mapx * y))]];
	tilenum = file.data[PTRmapdata + 2*(x + (mapx * y))] + (file.data[1 + PTRmapdata + 2*(x + (mapx * y))] * 256);
	tile = &file.data[PTRtilegfx + (1024 * tilenum)];
//	printf("Going to blit tile %d\n",tilenum);
//continue;
     for (j=0; j<32; j++)
     for (i=0; i<32; i++)
     {
        rendered->imgdata[i+(32*x)+(32*mapx*j) + (32*32*mapx*y)] = tile[i+(32*j)];
     }
   }
	}
     

    return rendered;
}
