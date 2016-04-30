/* hlp_war.h
loader for WAR archives
Warcraft I / Warcraft II / Warcraft II expansion */

#ifndef HLP_WAR_H_
#define HLP_WAR_H_

#include "../common.h"

struct archive_war
{
  unsigned int entries;
  unsigned int type;
  unsigned int magicnumber;
  unsigned long filesize;
  unsigned char **op;
  unsigned char *data;
};

struct archive_war *openWAR(const char*);
unsigned char *extractWAR(struct archive_war*, unsigned int, unsigned int*);
void closeWAR(struct archive_war*);

#endif
