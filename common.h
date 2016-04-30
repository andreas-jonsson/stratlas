// Turn on and off defines here to control features
#define DEFINE_SAVE_PNG
#define DEFINE_SAVE_BMP
//#define DEFINE_SAVE_JPG
#define DEFINE_LOAD_WARCRAFT
#define DEFINE_LOAD_WAR2
#define DEFINE_LOAD_CNCTD
#define DEFINE_LOAD_TOTALA

#define VERSION "0.2"

#ifndef COMMON_H_
#define COMMON_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifndef WITHOUT_WX
// for compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/dcclient.h"

    #include "wx/button.h"
    #include "wx/listbox.h"
    #include "wx/statbox.h"
    #include "wx/stattext.h"
    #include "wx/textctrl.h"
#endif

#include "wx/sizer.h"

#ifdef __WXUNIVERSAL__
    #include "wx/univ/theme.h"
#endif // __WXUNIVERSAL__

#else
	#define wxString std::string
	#include <iostream>
	#include <string>
#endif // WITHOUT_WX

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#ifdef _MSC_VER
#define inline __inline
#define strdup _strdup
#define DEBUG _DEBUG
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include <ctype.h>
#include <png.h>
#include <iostream>

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(USE_BEOS)
typedef unsigned long u_int32_t;
#endif

#ifndef __GNUC__
#define __attribute__(args)  // Does nothing for non GNU CC
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

// From SDL_byteorder.h
#if  defined(__i386__) || defined(__ia64__) || defined(WIN32) || \
    (defined(__alpha__) || defined(__alpha)) || \
     defined(__arm__) || \
    (defined(__mips__) && defined(__MIPSEL__)) || \
     defined(__SYMBIAN32__) || \
     defined(__x86_64__) || \
     defined(__LITTLE_ENDIAN__)
#ifdef __cplusplus
static inline unsigned short FetchLE16(unsigned char*& p) {
	unsigned short s = *(unsigned short*)p;
	p += 2;
	return s;
}
static inline unsigned int FetchLE32(unsigned char*& p) {
	unsigned int s = *(unsigned int*)p;
	p += 4;
	return s;
}
#else
#define FetchLE16(p) (*((unsigned short*)(p))); p += 2
#define FetchLE32(p) (*((unsigned int*)(p))); p += 4
#endif
#define AccessLE16(p) (*((unsigned short*)(p)))
#define AccessLE32(p) (*((unsigned int*)(p)))
#define ConvertLE16(v) (v)
#else
static inline unsigned short Swap16(unsigned short D) {
	return ((D << 8) | (D >> 8));
}
static inline unsigned int Swap32(unsigned int D) {
	return ((D << 24) | ((D << 8) & 0x00FF0000) | ((D >> 8) & 0x0000FF00) | (D >> 24));
}
#define FetchLE16(p) Swap16(*((unsigned short*)(p))); p += 2
#define FetchLE32(p) Swap32(*((unsigned int*)(p))) p += 4
#define AccessLE16(p) Swap16((*((unsigned short*)(p))))
#define AccessLE32(p) Swap32(*((unsigned int*)(p)))
#define ConvertLE16(v) Swap16(v)
#endif

#define FetchByte(p) (*((unsigned char*)(p))); ++p

/* common.h */

struct Image
{
       unsigned char bpp;
       unsigned int x, y;
       unsigned char palette[256][3];
       unsigned char *imgdata;
};

struct MapFile
{
       unsigned long size;
       wxString filename;
       unsigned char *data;
};

#endif
