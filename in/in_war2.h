/* in_war2.h
   process Blizzard War2 PUD files */

#ifndef IN_WAR2_H_
#define IN_WAR2_H_

#include "../common.h"

/* War2 uses WAR datafile for DOS version */
#include "../hlp/hlp_war.h"
/* War2 uses MPQ datafile for BNE version */
#include "../hlp/hlp_mpq.h"

/* function to init data */
int initWar2();

/* function to check for War2 type */   
int isWar2(struct MapFile);

/* War2 config page */
int cfgWar2(char *);

/* render a War2 map */
struct Image *renderWar2(struct MapFile);

struct cfg_war2_struct
{
  bool war2_isBNE;
  bool war2_drawPStart;
  char war2_dir[256];
};

#endif
