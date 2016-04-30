/* in_cnctd.cpp
   copyright 2008 Greg Kennedy <kennedy.greg@gmail.com>
   released under the GNU GPL v3 */

#include "in_cnctd.h"
struct cfg_cnctd_struct cnctd_cfg;

const char *template_names[216]={ "CLEAR1",
"W1",
"W2",
"SH1",
"SH2",
"SH3",
"SH4",
"SH5",
"SH11",
"SH12",
"SH13",
"SH14",
"SH15",
"S01",
"S02",
"S03",
"S04",
"S05",
"S06",
"S07",
"S08",
"S09",
"S10",
"S11",
"S12",
"S13",
"S14",
"S15",
"S16",
"S17",
"S18",
"S19",
"S20",
"S21",
"S22",
"S23",
"S24",
"S25",
"S26",
"S27",
"S28",
"S29",
"S30",
"S31",
"S32",
"S33",
"S34",
"S35",
"S36",
"S37",
"S38",
"SH32",
"SH33",
"SH20",
"SH21",
"SH22",
"SH23",
"BR1",
"BR2",
"BR3",
"BR4",
"BR5",
"BR6",
"BR7",
"BR8",
"BR9",
"BR10",
"P01",
"P02",
"P03",
"P04",
"P05",
"P06",
"P07",
"P08",
"SH16",
"SH17",
"SH18",
"SH19",
"P13",
"P14",
"P15",
"B1",
"B2",
"B3",
"B4",
"B5",
"B6",
"SH6",
"SH7",
"SH8",
"SH9",
"SH10",
"D01",
"D02",
"D03",
"D04",
"D05",
"D06",
"D07",
"D08",
"D09",
"D10",
"D11",
"D12",
"D13",
"D14",
"D15",
"D16",
"D17",
"D18",
"D19",
"D20",
"D21",
"D22",
"D23",
"D24",
"D25",
"D26",
"D27",
"D28",
"D29",
"D30",
"D31",
"D32",
"D33",
"D34",
"D35",
"D36",
"D37",
"D38",
"D39",
"D40",
"D41",
"D42",
"D43",
"RV01",
"RV02",
"RV03",
"RV04",
"RV05",
"RV06",
"RV07",
"RV08",
"RV09",
"RV10",
"RV11",
"RV12",
"RV13",
"RV14",
"RV15",
"RV16",
"RV17",
"RV18",
"RV19",
"RV20",
"RV21",
"RV22",
"RV23",
"RV24",
"RV25",
"FORD1",
"FORD2",
"FALLS1",
"FALLS2",
"BRIDGE1",
"BRIDGE1D",
"BRIDGE2",
"BRIDGE2D",
"BRIDGE3",
"BRIDGE3D",
"BRIDGE4",
"BRIDGE4D",
"SH24",
"SH25",
"SH26",
"SH27",
"SH28",
"SH29",
"SH30",
"SH31",
"P16",
"P17",
"P18",
"P19",
"P20",
"SH34",
"SH35",
"SH36",
"SH37",
"SH38",
"SH39",
"SH40",
"SH41",
"SH42",
"SH43",
"SH44",
"SH45",
"SH46",
"SH47",
"SH48",
"SH49",
"SH50",
"SH51",
"SH52",
"SH53",
"SH54",
"SH55",
"SH56",
"SH57",
"SH58",
"SH59",
"SH60",
"SH61",
"SH62",
"SH63"};

int initCNCTD()
{
  strcpy(cnctd_cfg.cnctd_dir,"C:\\WESTWOOD\\CNC");
  return 0;
}

/* function to check for CNCTD type */   
int isCNCTD(struct MapFile file)
{
    // CNCTD maps are weird.  There is a .ini and a .bin
    //  .bin is always 8192 bytes long - raw tile data
    //  .ini 'scripts' the map
    // Design decision: users must pick the .ini, not the .bin
    char binname[256];
    FILE *binfile = NULL;

  // A very easy way to check if this is CNCTD: just search the inifile for
  // [MAP]
    if (strstr((char *)file.data,"[MAP]") == NULL) return 0;
    
 // check if .bin exists, is 8192 bytes
    strcpy(binname,file.filename.mb_str());
    binname[strlen(binname)-4] = '\0';
    strcat(binname,".bin");
    binfile = fopen(binname,"rb");
    if (binfile == NULL) return 0;
    fseek(binfile,0,SEEK_END);
    if (ftell(binfile) != 8192) return 0;
    fclose(binfile);
    return 1;
}

/* CNCTD config page - nothing to set now */
int cfgCNCTD(char *line)
{
    if (strstr(line,"CNCTD_DIR ")) { strncpy(cnctd_cfg.cnctd_dir,(&line[10]),strlen(line)-11); cnctd_cfg.cnctd_dir[strlen(line)-11]='\0'; return 1; }
    return 0;
}

/* render a CNCTD map */
struct Image* renderCNCTD(struct MapFile file)
{
    struct Image *rendered = NULL;
    struct archive_mix *mix;
    unsigned char cTemplate[64][64];
    unsigned char cTileImg[64][64];
//struct archive_mix* mixes[3] = {NULL, NULL, NULL};
unsigned char *mix_ptr;
//char* mixnames[3]={"temperat.mix","desert.mix","winter.mix"}, *palnames[3]={"TEMPERAT.PAL","DESERT.PAL","WINTER.PAL"};
//unsigned int pal_entry_nums[3]={62,26,62};
//unsigned char tileset_palettes[3][256][3];
    unsigned short i,j,x,y; //,out_x,out_y;

    char binname[256];
    FILE *binfile = NULL;

    if (isCNCTD(file) == 1) {
 // let's get the bin junk out of the way first.
    strcpy(binname,file.filename.mb_str());
    binname[strlen(binname)-4] = '\0';
    strcat(binname,".bin");
    binfile = fopen(binname,"rb");
    if (binfile == NULL) return 0;
      for (j=0; j<64; j++)// {
        for (i=0; i<64; i++)
        {
          fread(&cTemplate[i][j],1,1,binfile);
          fread(&cTileImg[i][j],1,1,binfile);
//printf(":%d %d:",cTemplate[i][j],cTileImg[i][j]);
        }// printf("\n"); }
      fclose(binfile);
printf("\n");
      
    // time to parse the .ini
    //  there are big [HEADERS] and beneath that the options we need
    binfile = fopen("out.txt","w");

    struct cnctdINI ini;
    unsigned int ptr = 0;
    char inbuf[256];
    // what we are parsing now
     // section 0 is outside any header, or useless to us
     // 1 = BASIC
     // 2 = MAP
     // 3 = TERRAIN
     // 4 = OVERLAY
     // 5 = SMUDGE
     // 6 = WAYPOINTS
     // 7 = STRUCTURES
     // 8 = UNITS
     // 9 = INFANTRY
     // 10 = TEMPLATE
    unsigned char section=0;

    while (ptr < file.size)
    {
        sscanf((char *)&(file.data[ptr]),"%s\n",inbuf);
        ptr += strlen(inbuf)+1;
        if (inbuf[0] == '[') {
          if (strcmp(inbuf,"[BASIC]") == 0) section = 1;
          else if (strcmp(inbuf,"[MAP]") == 0) section = 2;
          else if (strcmp(inbuf,"[TERRAIN]") == 0) section = 3;
          else if (strcmp(inbuf,"[OVERLAY]") == 0) section = 4;
          else if (strcmp(inbuf,"[SMUDGE]") == 0) section = 5;
          else if (strcmp(inbuf,"[WAYPOINTS]") == 0) section = 6;
          else if (strcmp(inbuf,"[STRUCTURES]") == 0) section = 7;
          else if (strcmp(inbuf,"[UNITS]") == 0) section = 8;
          else if (strcmp(inbuf,"[INFANTRY]") == 0) section = 9;
          else if (strcmp(inbuf,"[TEMPLATE]") == 0) section = 10;
          else section=0;
        } else if (section != 0) {
          switch(section)
          {
            case 1:
              if (strncmp(inbuf,"Name=",5) == 0) strcpy(ini.name,&inbuf[5]);
              break;
	    case 2:
              if (strncmp(inbuf,"Theater=",8) == 0) { memset(ini.theater,0,9); strncpy(ini.theater,&inbuf[8],8); }
              else if (strncmp(inbuf,"Height=",7) == 0) ini.height = atoi(&inbuf[7]);
              else if (strncmp(inbuf,"Width=",6) == 0) ini.width = atoi(&inbuf[6]);
              else if (strncmp(inbuf,"X=",2) == 0) ini.x = atoi(&inbuf[2]);
              else if (strncmp(inbuf,"Y=",2) == 0) ini.y = atoi(&inbuf[2]);
            default:
              break;
          }
        }
    }

    fprintf(binfile,"Name=*%s*\n",ini.name);
    rendered = (struct Image*) malloc (sizeof (struct Image));
    if (rendered == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); return NULL; }

    // CNCTD maps are up to 64x64 tiles, 24 pixels/tile, 8bpp
    rendered->x = ini.width * 24;
    rendered->y = ini.height * 24;
    rendered->bpp = 8;
    rendered->imgdata = (unsigned char*)malloc(ini.width*ini.height*24*24);

    if (rendered->imgdata == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); free(rendered); return NULL; }

//    war = openWAR("c:/CNC/DATA/DATA.WAR");
//    if (war == NULL) { fprintf(stderr,"couldn't open WAR\n"); free(rendered->imgdata); free(rendered); return NULL; }

          
      // Read data on 200 units.
  // Load the MIX file for this Theater.

strcpy(inbuf,cnctd_cfg.cnctd_dir); strcat(inbuf,"/"); strcat(inbuf,ini.theater); strcat(inbuf,".mix");
printf("Using *%s* for mixname\n",inbuf);
  mix = openMIX(inbuf,0);


// load the three palettes from each MIX
//    for (j=0; j<3; j++) {
///j=0;
strcpy(inbuf,ini.theater); strcat(inbuf,".PAL");
mix_ptr = extractMIX(mix,inbuf);
	
    for (i=0; i<256; i++)
    {
        rendered->palette[i][0] = mix_ptr[i * 3] << 2;
        rendered->palette[i][1] = mix_ptr[(i * 3) + 1] << 2;
        rendered->palette[i][2] = mix_ptr[(i * 3) + 2] << 2;
    }
//}


    // blit map tiles to image data
    for (y=0; y<ini.height; y++)
    for (x=0; x<ini.width; x++)
    {
// first, get Template from MIX
if (cTemplate[x+ini.x][y+ini.y] != 255) {
strcpy(inbuf,template_names[cTemplate[x+ini.x][y+ini.y]]); strcat(inbuf,"."); strncat(inbuf,ini.theater,3); }
else { strcpy(inbuf,"CLEAR1."); cTileImg[x+ini.x][y+ini.y] = rand() % 16; strncat(inbuf,ini.theater,3); }
//printf("inbuf contains *%s*\n",inbuf);
	mix_ptr = extractMIX(mix,inbuf);

// next, find the Index to the Tile
unsigned int img_start_offset, img_index1, img_number;
memcpy(&img_start_offset,&mix_ptr[12],4);
img_index1 = *((unsigned int*)&mix_ptr[28]);

//printf("img_start_offset = %d, img_index1 = %d,\n",img_start_offset,img_index1);
//unsigned int 
//mix_ptr = extractMIX_rawptr(mix,img_index1);
//printf("cti = %d,",cTileImg[x+ini.x][y+ini.y]);
img_number = mix_ptr[img_index1 + (cTileImg[x+ini.x][y+ini.y])];
//printf(" -> img_number = %d\n",img_number);
//memcpy(&img_index1,&mix_ptr[cTileImg[x+ini.x][y+ini.y]*4],4);
//mix_ptr = extractMIX_rawptr(mix,img_start_offset+(24*24*img_index1));
unsigned char *tileptr = &mix_ptr[img_start_offset+(24*24*img_number)];
//printf(" -> tileptr = %x\n",tileptr - mix_ptr);

//,cTileImg[x+ini.x][y+ini.y]);
        for (i=0; i<24; i++)
        for (j=0; j<24; j++) {
        rendered->imgdata[(ini.width*24 *((24*y)+i))+((24*x)+j)] = tileptr[24*i + j];
        }
     }
        

      closeMIX(mix);

      return rendered;
    } else {
 fprintf(stderr,"file is not a CNC SAV format\n"); //free(rendered->imgdata); free(rendered);
 return NULL; 
	}
}
