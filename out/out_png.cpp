/* Simple PNG writer
   8- or 24-bit */

#include "out_png.h"

int savePNG(struct Image* incoming, const char* filename)
{
    FILE *fp = NULL;

   png_structp png_ptr=NULL;
   png_infop info_ptr=NULL;
   png_colorp palette=NULL;
   png_bytepp row_pointers = NULL;
   unsigned int i;

    struct Image rendered = *(incoming);

    fp = fopen(filename,"wb");
    if (fp == NULL)
    {
        fprintf(stderr,"savePNG: couldn't open %s\n",filename);
        return -1;
    }
    
   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible with the one used at compile time,
    * in case we are using dynamically linked libraries.  REQUIRED.
    */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if (png_ptr == NULL)
    return -1;

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
       return -1;
   if (setjmp(png_jmpbuf(png_ptr)))
       return -1;

   row_pointers = (png_bytepp) malloc (rendered.y * sizeof(png_bytepp));
   for (i=0; i<rendered.y; i++)
   {
     row_pointers[i] = rendered.imgdata + (rendered.x * i * (rendered.bpp / 8));
   }

   /* set up the output control if you are using standard C streams */
   png_init_io(png_ptr, fp);

if (rendered.bpp == 8)
{
   png_set_IHDR(png_ptr,info_ptr,rendered.x,rendered.y,8,PNG_COLOR_TYPE_PALETTE,
   PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);

   /* set the palette if there is one.  REQUIRED for indexed-color images */
   palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH
             * sizeof (png_color));
   for (i=0; i<256; i++)
   {
       palette[i].red = rendered.palette[i][0];
       palette[i].green = rendered.palette[i][1];
       palette[i].blue = rendered.palette[i][2];
   }
   /* ... set palette colors ... */
   png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
   /* You must not free palette here, because png_set_PLTE only makes a link to
      the palette that you malloced.  Wait until you are about to destroy
      the png structure. */

} else
   png_set_IHDR(png_ptr,info_ptr,rendered.x,rendered.y,8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);

   /* This is the easy way.  Use it if you already have all the
    * image info living info in the structure.  You could "|" many
    * PNG_TRANSFORM flags into the png_transforms integer here.
    */

   png_set_rows(png_ptr, info_ptr, row_pointers);

   png_write_png(png_ptr, info_ptr, 0, png_voidp_NULL);
        
   free(row_pointers);
   
   if (rendered.bpp == 8) png_free(png_ptr,palette);

   /* If you png_malloced a palette, free it here (don't free info_ptr->palette,
      as recommended in versions 1.0.5m and earlier of this example; if
      libpng mallocs info_ptr->palette, libpng will free it).  If you
      allocated it with malloc() instead of png_malloc(), use free() instead
      of png_free(). */
//   png_free(png_ptr, palette);
   palette=NULL;

   /* clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   /* close the file */
   fclose(fp);

    return 0;
}
