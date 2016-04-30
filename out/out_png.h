#ifndef OUT_PNG_H_
#define OUT_PNG_H_

#include "../common.h"

#include <png.h>

#ifndef png_voidp_NULL
#define png_voidp_NULL (png_voidp)NULL
#endif

int savePNG(struct Image*, const char*);

#endif
