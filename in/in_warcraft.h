/* in_warcraft.h
   process Blizzard Warcraft campaign and SAV files */

#ifndef IN_WARCRAFT_H_
#define IN_WARCRAFT_H_

#include "../common.h"

/* Warcraft uses WAR datafile */
#include "../hlp/hlp_war.h"

/* function to init data */
int initWarcraft();

/* function to check for Warcraft type */   
int isWarcraft(struct MapFile);

/* Warcraft config page */
int cfgWarcraft(char *);

/* render a Warcraft map */
struct Image *renderWarcraft(struct MapFile);

struct war1unit
{
       unsigned char type;
       unsigned short x;
       unsigned short y;
       unsigned short hp;
       unsigned short gold;
       unsigned char owner;
};

struct cfg_war1_struct
{
  char war1_dir[256];
};

#endif
