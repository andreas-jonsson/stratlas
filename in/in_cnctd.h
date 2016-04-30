/* in_cnctd.h
   process Westwood C&C Tiberian Dawn ini/bin files */

#ifndef IN_CNCTD_H_
#define IN_CNCTD_H_

#include "../common.h"

/* CNC uses MIX datafile */
#include "../hlp/hlp_mix.h"

/* hack for those without wx-widgets */
#ifdef WITHOUT_WX
#define mb_str c_str
#endif

/* function to init CNCTD vars */
int initCNCTD();

/* function to check for CNCTD type */   
int isCNCTD(struct MapFile);

/* CNCTD config page */
int cfgCNCTD(char *);

/* render a CNCTD map */
struct Image *renderCNCTD(struct MapFile);

struct cnctdterrain
{
  unsigned short cellnumber;
  char type[10];
};

struct cnctdsmudge
{
  unsigned short cellnumber;
  char type[10];
};

struct cnctdoverlay
{
  unsigned short cellnumber;
  char type[10];
};

struct cnctdwaypoint
{
  unsigned short cellnumber;
  char type[10];
};

struct cnctdstructure
{
  unsigned short cellnumber;
  unsigned char owner;
  char type[10];
  unsigned char direction;
};

struct cnctdinf
{
  unsigned short cellnumber;
  unsigned char owner;
  char type[10];
  unsigned char direction;
};

struct cnctdunit
{
  unsigned short cellnumber;
  unsigned char subcell;
  unsigned char owner;
  char type[10];
  unsigned char direction;
};

struct cnctdINI
{
       char name[128];
       unsigned char height;
       unsigned char width;
       unsigned char x;
       unsigned char y;
       char theater[9];
       unsigned char p_present[10];
       unsigned int p_credits[10];
//       unsigned int terrain_obj*
};

struct cfg_cnctd_struct
{
  char cnctd_dir[256];
};
#endif
