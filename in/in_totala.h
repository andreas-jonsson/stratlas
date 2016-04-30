/* in_totala.h
   process Cavedog Total Annihilation TNT files */

#ifndef IN_TOTALA_H_
#define IN_TOTALA_H_

#include "../common.h"

/* TODO: TotalA may use UFO datafile */
//#include "../hlp/hlp_ufo.h"

/* function to init data */
int initTotalA();

/* function to check for TotalA type */   
int isTotalA(struct MapFile);

/* TotalA config page */
int cfgTotalA(char *);

/* render a TotalA map */
struct Image *renderTotalA(struct MapFile);

struct cfg_totala_struct
{
//  char totala_dir[256];
};

#endif
