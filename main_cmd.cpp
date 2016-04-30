/////////////////////////////////////////////////////////////////////////////
// Name:        main_cmd.cpp
// Purpose:     Stratlas main, command-line only
// Author:      Greg Kennedy
// Id:          $Id: main.cpp,v 1.00 2008/03/16 21:39:20 PC Exp $
// Copyright:   (c) 2008 Greg Kennedy
// Licence:     GPL v3
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

#include "common.h"

    char path[1024];


#ifdef DEFINE_SAVE_BMP
#include "out/out_bmp.h"
#endif

#ifdef DEFINE_SAVE_PNG
#include "out/out_png.h"
#endif

#ifdef DEFINE_LOAD_WARCRAFT
#include "in/in_warcraft.h"
#endif

#ifdef DEFINE_LOAD_WAR2
#include "in/in_war2.h"
#endif

#ifdef DEFINE_LOAD_CNCTD
#include "in/in_cnctd.h"
#endif

#ifdef DEFINE_LOAD_TOTALA
#include "in/in_totala.h"
#endif

int main(int argc, char *argv[])
{

    // load stratlas.ini and get config info
#ifdef DEFINE_LOAD_WARCRAFT
    initWarcraft();
#endif
#ifdef DEFINE_LOAD_WAR2
    initWar2();
#endif
#ifdef DEFINE_LOAD_CNCTD
    initCNCTD();
#endif
#ifdef DEFINE_LOAD_TOTALA
    initTotalA();
#endif
FILE *cfgfile = NULL;
char buffer[256];
    cfgfile = fopen("stratlas.ini","r");
    while (!feof(cfgfile))
    {
      fgets(buffer,256,cfgfile);
      if (!feof(cfgfile) && buffer[0] != '\n' && buffer[0] != '#') {
#ifdef DEFINE_LOAD_WARCRAFT
      if (!cfgWarcraft(buffer))
#endif
#ifdef DEFINE_LOAD_WAR2
      if (!cfgWar2(buffer))
#endif
#ifdef DEFINE_LOAD_CNCTD
      if (!cfgCNCTD(buffer))
#endif
#ifdef DEFINE_LOAD_TOTALA
      if (!cfgTotalA(buffer))
#endif
      printf("Warning: unrecognized ini entry %s",buffer);
    }
}
    fclose(cfgfile);

    getcwd(path,1024);

  FILE *fp;
  unsigned int i;
  struct MapFile map;
  struct Image *img;
  wxString str;

  for (i=1; i<(unsigned)argc; i++)
  {
//      map.filename = (wxChar *)malloc(wxStrlen(list_box_1->GetString(i))+1);
//      wxStrcpy(map.filename,list_box_1->GetString(i));
	map.filename=argv[i];

	std::cout << "Processing " << map.filename << "..." << std::endl;

      fp = fopen(map.filename.c_str(),"rb");
      if (fp != NULL) {
        fseek(fp,0,SEEK_END);
        map.size = ftell(fp);
        rewind(fp);
        map.data = (unsigned char*)malloc(map.size);
        if (map.data) {
          fread(map.data,1,map.size,fp);
		std::cout << "Read " << map.size << " bytes from file" << std::endl;
          img = NULL;
          if (0) { } // dummy
#ifdef DEFINE_LOAD_WARCRAFT
          else if (isWarcraft(map)) { img = renderWarcraft(map); }
#endif
#ifdef DEFINE_LOAD_WAR2
          else if (isWar2(map)) { img = renderWar2(map); }
#endif
#ifdef DEFINE_LOAD_CNCTD
          else if (isCNCTD(map)) { img = renderCNCTD(map); }
#endif
#ifdef DEFINE_LOAD_TOTALA
          else if (isTotalA(map)) { img = renderTotalA(map); }
#endif
          if (img != NULL) {
		std::cout << "Rendered map, saving to " << map.filename << ".png" << std::endl;
		str = map.filename + ".png";
//            sprintf(str,"%s.png",map.filename);
            savePNG(img,str.c_str());
            free(img->imgdata);
            free(img);
            img = NULL;
          } else {
		std::cout << "error" << std::endl;
		return -1;
	}
          free(map.data);
          map.data = NULL;
        }
        fclose(fp);
        fp = NULL;
      } else {
		std::cout << "error opening map" << std::endl;
      }
//      free(map.filename);
  }
return 0;
}

