/* Simple BMP writer
   8- or 24-bit */

#include "out_bmp.h"

int saveBMP(struct Image* incoming, const char* filename)
{
    FILE *fBMP = NULL;
    unsigned int temp;
    int i,j;
    int xmod4;
    struct Image rendered = *(incoming);

    // This BMP header is pre-configured for 24-bit
    // We will mangle it later to support 8-bit.
                                           //filesize
  unsigned char bmpheader[] = {0x42, 0x4D, 0,0,0,0, 0,0, 0,0, 0x36,0,0,0,
                               0x28,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
                                           //w      //h                          //datasize


    fBMP = fopen(filename,"wb");
    if (fBMP == NULL)
    {
        fprintf(stderr,"saveBMP: couldn't open %s\n",filename);
        return -1;
    }

    if (rendered.bpp == 8)
    {
       xmod4 = (((rendered.x+3)/4)*4);
    } else {
       xmod4 = ((((3*rendered.x)+3)/4)*4);
    }

    if (rendered.bpp == 8) {
      bmpheader[11] = 4;
      bmpheader[28] = 8; // 8bpp
      bmpheader[47] = 1; // number of colors
//      bmpheader[51] = 1; // number of important colors
      temp=(unsigned int)(1078 + (xmod4 * rendered.y));
    } else {
      temp =(unsigned int)(54 + (3 * xmod4 * rendered.y));
    }
    // filesize.
    memcpy(&bmpheader[2],&temp,4);
    // dimensions
    memcpy(&bmpheader[18],&rendered.x,4);
    memcpy(&bmpheader[22],&rendered.y,4);

    if (rendered.bpp == 8) {
        temp = xmod4 * rendered.y;
    } else {
        temp = 3 * xmod4 * rendered.y; // size without header
    }
    memcpy(&bmpheader[34],&temp,4);

    fwrite(bmpheader,1,54,fBMP);

    // save palette for 8bpp images
    if (rendered.bpp == 8) {
      for (i=0; i<256; i++)
      {
       fwrite(&rendered.palette[i][2],1,1,fBMP);
       fwrite(&rendered.palette[i][1],1,1,fBMP);
       fwrite(&rendered.palette[i][0],1,1,fBMP);
       fwrite("",1,1,fBMP);
      }
    }

    // BMP files are stored upside down.
    for (i=rendered.y-1; i>=0; i--)
    {
        for (j=0; j<rendered.x; j++)
        {
            if (rendered.bpp == 8)
              fwrite(&rendered.imgdata[j+(i*rendered.x)],1,1,fBMP);
            else
            {
                // BGR format
              fwrite(&rendered.imgdata[3*(j+(i*rendered.x))+2],1,1,fBMP);
              fwrite(&rendered.imgdata[3*(j+(i*rendered.x))+1],1,1,fBMP);
              fwrite(&rendered.imgdata[3*(j+(i*rendered.x))],1,1,fBMP);
            }
        }
        if (rendered.bpp == 8) {
        for (j=0; j<xmod4-rendered.x; j++)
        {
              fwrite("",1,1,fBMP);
        }
        } else {
        for (j=0; j<xmod4-(3*rendered.x); j++)
        {
              fwrite("",1,1,fBMP);
        }
        }
    }
    fclose(fBMP);
    return 0;
}
