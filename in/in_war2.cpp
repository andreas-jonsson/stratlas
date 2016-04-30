/* based on previous pud2bmp code */

#include "in_war2.h"

struct cfg_war2_struct war2_cfg;

#define TRUCOLOR 1

//TODO: These palette colors are from Wargus.
// Red (p1) is always at 208-211.
// Blue (p2) is nonstandard and is 212-215 in Ice,Swamp, Xswamp, not
//           present at all in Forest.
// p3 is not present in any palette. (?)
// TODO: need p4-p6...
// White (p7) is always at 232-235.
// Yellow (p8) is always at 13-16.
// They only work in true-color mode: we need to find free entries in the
// 256color palette and stuff these in.

// [tileset][indextofirst]
unsigned char pcolors[8][4][3] = {
  {{164, 0, 0}, {124, 0, 0}, {92, 4, 0}, {68, 4, 0}},
  {{12, 72, 204}, {4, 40, 160}, {0, 20, 116}, {0, 4, 76}},
  {{44, 180, 148}, {20, 132, 92}, {4, 84, 44}, {0, 40, 12}},
  {{152, 72, 176}, {116, 44, 132}, {80, 24, 88}, {44, 8, 44}},
  {{248, 140, 20}, {200, 96, 16}, {152, 60, 16}, {108, 32, 12}},
  {{40, 40, 60}, {28, 28, 44}, {20, 20, 32}, {12, 12, 20}},
  {{224, 224, 224}, {152, 152, 180}, {84, 84, 128}, {36, 40, 76}},
  {{252, 252, 72}, {228, 204, 40}, {204, 160, 16}, {180, 116, 0}}
};

//TODO: Get a real GRP loader to get the second frame of art/unit/other/shadow.grp ...
//   or, pack this and use bit-ops to draw the shadow.

unsigned char shadowmap[1024] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0x00,0xEF,0x00,0xEF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//TODO: these have GOT to be stored somewhere.
int unitxoff[] = {
    20,20, // grunt
    20,20, // peon
    16,16, // catapult
    20,20, // knight
    20,20, // spear
    20,20, // wiz
    20,20, // pally
    12,12, // demosquad
    20,20, // atk peon
    20,20, // ranger
    20,20, // h:alleria, h:teron
    24,20, // h:kurdan, h:dentarg
    20,20, // h:khadgar, h:grom
    20,20, // tanker
    20,20, // transport
    24,28, // destroyer
    28,28, // bship
     0,28, // h:deathwing
     0, 0, // NULL
    20,20, // sub
    24,20, // flying machine
    24,28, // griffon
    20,20, // h:turalyon, TODO: eye of kilrogg
    20,20, // h:danath, h: khorgath
     0,20, // NULL, h:cho'gall
    20,20, // h:lothar, h:gul'dan
    20,20, // h:uther, h:zuljin
     0,12, // NULL, skele
    20     // daemon
};

int unityoff[] = {
    20,20, // grunt
    20,20, // peon
    16,16, // catapult
    20,20, // knight
    20,20, // spear
    20,20, // wiz
    20,20, // pally
    12,12, // demosquad
    20,20, // atk peon
    20,20, // ranger
    20,20, // h:alleria, h:teron
    40,20, // h:kurdan, h:dentarg
    20,20, // h:khadgar, h:grom
    20,20, // tanker
    20,20, // transport
    28,28, // destroyer
    28,28, // bship
     0,40, // NULL, h:deathwing
     0, 0, // NULL
    20,20, // sub
    40,36, // flying machine
    40,40, // griffon
    20,20, // h:turalyon, TODO: eye of kilrogg
    20,20, // h:danath, h: khorgath
     0,20, // NULL, h:cho'gall
    20,20, // h:lothar, h:gul'dan
    20,20, // h:uther, h:zuljin
     0,12, // NULL, skele
    36     // daemon
};

const char *basenames[] = {
//0x0
     "human/grunt.grp",
     "orc/grunt.grp",
     "human/peon.grp",
     "orc/peon.grp",
     "human/catapult.grp",
     "orc/catapult.grp",
     "human/knight.grp",
     "orc/knight.grp",
     "human/spear.grp",
     "orc/spear.grp",
     "human/wizard.grp",
     "orc/dknight.grp",
     "human/knight.grp", // paladin
     "orc/knight.grp",   // ogre-mage
     "human/dwarves.grp",
     "orc/goblins.grp",
//0x10
     "human/peon.grp",  // attack peasant
     "orc/peon.grp",    // attack peon
     "human/spear.grp", // ranger
     "orc/spear.grp",   // berserker
     "human/spear.grp",  // hero: alleria
     "orc/dknight.grp",  // hero: teron gorefiend
     "human/griffon.grp",  // hero: kurdan / skyree
     "orc/knight.grp",  // hero: dentarg
     "human/wizard.grp",  // hero: khadgar
     "orc/grunt.grp",  // hero: grom hellscream
     "human/tanker.grp",
     "orc/tanker.grp",
     "human/transp.grp",
     "orc/transp.grp",
     "human/destroy.grp",
     "orc/destroy.grp",
//0x20
     "human/battlshp.grp",
     "orc/battlshp.grp",
     NULL,
     "orc/dragon.grp",  // hero: deathwing
     NULL,
     NULL,
     "human/sub.grp",
     "orc/sub.grp",
     "human/orn.grp",
     "orc/zep.grp",
     "human/griffon.grp",
     "orc/dragon.grp",
     "human/knight.grp", // hero: turalyon
     "orc/eyeofkil.grp",
     "human/grunt.grp", // hero: danath
     "orc/grunt.grp", // hero: khorgath bladefist
//0x30
     NULL,
     "orc/knight.grp", // hero: cho'gall
     "human/knight.grp", // hero: lothar
     "orc/dknight.grp", // hero: gul'dan
     "human/knight.grp", // hero: uther lightbringer
     "orc/spear.grp", // hero: zuljin
     NULL,
     "orc/skeleton.grp",
     "monster/demon.grp",
     "monster/sheep.grp", // critter!
     "human/farm.grp",
     "orc/farm.grp",
     "human/barr.grp",
     "orc/barr.grp",
     "human/church.grp",
     "orc/temple.grp",
     "human/tower.grp",
     "orc/tower.grp",
     "human/stable.grp",
     "orc/ogrecamp.grp",
     "human/invent.grp",
     "orc/invent.grp",
     "human/aviary.grp",
     "orc/roost.grp",
     "human/ship2.grp",
     "orc/ship2.grp",
     "human/thall.grp",
     "orc/thall.grp",
     "human/lmill.grp",
     "orc/lmill.grp",
     "human/found2.grp",
     "orc/found2.grp",
     "human/wtower.grp",
     "orc/dtower.grp",
     "human/black.grp",
     "orc/black.grp",
     "human/ref2.grp",
     "orc/ref2.grp",
     "human/oplat2.grp",
     "orc/oplat2.grp",
     "human/keep.grp",
     "orc/keep.grp",
     "human/castle.grp",
     "orc/blakrock.grp",
     "other/mine.grp",
     "other/patch.grp",
     "human/startloc.grp",
     "orc/startloc.grp",
     "human/towera.grp",
     "orc/towera.grp",
     "human/towerc.grp",
     "orc/towerc.grp",
     "other/vcircle.grp",
     "other/gate.grp",
     "other/rock.grp",
     "other/fwalunit.grp", // not really certain about how walls work.
     "other/fwalunit.grp"
};

const char *snownames[] = { // replace names for snow bldg
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
//0x39
     "monster/seal.grp", // critter!
     "human/s_farm.grp",
     "orc/s_farm.grp",
     "human/s_barr.grp",
     "orc/s_barr.grp",
     "human/s_church.grp",
     "orc/s_temple.grp",
     "human/s_tower.grp",
     "orc/s_tower.grp",
     "human/s_stable.grp",
     "orc/s_ogre.grp",
     "human/s_inv.grp",
     "orc/s_inv.grp",
     "human/s_aviary.grp",
     "orc/s_roost.grp",
     "human/s_ship2.grp",
     "orc/s_ship2.grp",
     "human/s_thall.grp",
     "orc/s_thall.grp",
     "human/s_lmill.grp",
     "orc/s_lmill.grp",
     "human/s_found2.grp",
     "orc/s_found2.grp",
     "human/s_wtower.grp",
     "orc/s_dtower.grp",
     "human/s_black.grp",
     "orc/s_black.grp",
     "human/s_ref2.grp",
     "orc/s_ref2.grp",
     "human/s_oplat2.grp",
     "orc/s_oplat2.grp",
     "human/s_keep.grp",
     "orc/s_keep.grp",
     "human/s_castle.grp",
     "orc/s_blakrk.grp",
     "other/s_mine.grp",
     NULL,
     NULL,
     NULL,
     "human/s_towera.grp",
     "orc/s_towera.grp",
     "human/s_towerc.grp",
     "orc/s_towerc.grp",
     NULL,
     "other/s_gate.grp",
     "other/s_rock.grp",
     NULL,
     NULL
};

const char *wastenames[] = { // wasteland - e.g. l_xyz
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     "human/l_sub.grp",
     "orc/l_sub.grp",
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     "monster/boar.grp", // critter!
     "human/l_farm.grp",
     "orc/l_farm.grp",
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     "human/l_lmill.grp",
     "orc/l_lmill.grp",
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     "human/l_oplat2.grp",
     "orc/l_oplat2.grp",
     NULL,
     NULL,
     NULL,
     NULL,
     "other/l_mine.grp",
     "other/l_patch.grp",
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     "other/l_gate.grp",
     NULL,
     NULL,
     NULL
};
     
const char *xswampnames[] = {
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     "human/x_sub.grp",
     "orc/x_sub.grp",
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     "monster/hellhog.grp", // critter!
     "human/x_farm.grp",
     "orc/x_farm.grp",
     "human/x_barr.grp",
     "orc/x_barr.grp",
     "human/x_church.grp",
     "orc/x_temple.grp",
     "human/x_tower.grp",
     "orc/x_tower.grp",
     "human/x_stable.grp",
     "orc/x_ogre.grp",
     "human/x_inv.grp",
     "orc/x_inv.grp",
     "human/x_aviary.grp",
     "orc/x_roost.grp",
     "human/x_ship2.grp",
     "orc/x_ship2.grp",
     "human/x_thall.grp",
     "orc/x_thall.grp",
     "human/x_lmill.grp",
     "orc/x_lmill.grp",
     "human/x_found2.grp",
     "orc/x_found2.grp",
     "human/x_wtower.grp",
     "orc/x_dtower.grp",
     "human/x_black.grp",
     "orc/x_black.grp",
     "human/x_ref2.grp",
     "orc/x_ref2.grp",
     "human/x_oplat2.grp",
     "orc/x_oplat2.grp",
     "human/x_keep.grp",
     "orc/x_keep.grp",
     "human/x_castle.grp",
     "orc/x_blakrk.grp",
     "other/x_mine.grp",
     "other/x_patch.grp",
     NULL,
     NULL,
     "human/x_towera.grp",
     "orc/x_towera.grp",
     "human/x_towerc.grp",
     "orc/x_towerc.grp",
     "other/x_vcir.grp",
     "other/x_gate.grp",
     "other/x_rock.grp",
     NULL,
     NULL
};

struct player
{
  unsigned char controller;
  unsigned char race;
  unsigned short gold;
  unsigned short lumber;
  unsigned short oil;
  unsigned char present;
} players[9];

struct unit
{
  unsigned short x,y;
  unsigned char type,owner;
  unsigned short arg;
} *units;

struct grpframe
{
  unsigned short w,h;
  unsigned char *data;
};

char setname[10];
unsigned char tileset;
unsigned short mapx, mapy;
unsigned short *tilemap;
unsigned long num_units;

//unsigned char drawpstarts;

//unsigned char *rendered->imgdata;

unsigned char pal[256][3];
unsigned short tilegroups[158][16];
unsigned short *megatiles;
unsigned char *minitiles;

struct grpframe *unitimgs[104];

struct grpframe *loadGRP(unsigned char type) // braindead GRP loader for first frame only
{
  unsigned char buffer[80],loop;
//  unsigned short *lineoffsets;
  unsigned short xoff,yoff,i,j,numlines,w,h;
  signed short xpos,ypos;
  struct grpframe *tmp = NULL;
  FILE *fGRP = NULL;

  strcpy((char *)buffer,"art/unit/");
  if (tileset == 1 && snownames[type] != NULL) strcat((char *)buffer,snownames[type]);
  else if (tileset == 2 && wastenames[type] != NULL) strcat((char *)buffer,wastenames[type]);
  else if (tileset == 3 && xswampnames[type] != NULL) strcat((char *)buffer,xswampnames[type]);
  else strcat((char *)buffer,basenames[type]);

  fGRP = fopen((char *)buffer,"rb");
  if (fGRP == NULL) { fprintf(stderr,"ERROR: Couldn't open %s...\n",buffer); return NULL; }

  tmp = (struct grpframe *)malloc(sizeof(struct grpframe));
//  int debug=0;
//  if (strcmp("art/unit/other/mine.grp",buffer) == 0) debug=1;
//  fprintf(stderr,"opened grp %s\n",buffer);
  fread(buffer,1,6,fGRP);
  tmp->w = buffer[2] + 256 * buffer[3];
  tmp->h = buffer[4] + 256 * buffer[5];
  fread(buffer,1,6,fGRP);
  xoff = buffer[0];
  yoff = buffer[1];
  w = buffer[2];
  h = buffer[3];
  fseek(fGRP,buffer[4] + 256 * buffer[5], SEEK_SET); // skip ahead to frame 1
                                     // other frames would have followed

//if (debug==1)  fprintf(stderr,"w %u h %u (%u %u) xoff %u yoff %u frame1 at %x\n",tmp->w,tmp->h,w,h,xoff,yoff,buffer[4] + 256 * buffer[5]);
  fread(buffer,1,2,fGRP);
// cheap
  numlines = (buffer[0] + 256*buffer[1]-(ftell(fGRP)-2)) / 2;
  fseek(fGRP,buffer[0] + 256*buffer[1]-2,SEEK_CUR);
// proper but nonworking : )
/*  numlines = (buffer[0] + 256*buffer[1]-(ftell(fGRP)-2)) / 2;
  printf("%u lines present\n",numlines);
  lineoffsets = (unsigned short *)malloc((numlines+1)*2);
  lineoffsets[0] = buffer[0] + 256 * buffer[1];
  printf("lo[0]=%x\n",lineoffsets[0]);
  for(i=1;i<numlines;i++)
  {
    fread(buffer,1,2,fGRP);
    lineoffsets[i] = buffer[0] + 256 * buffer[1];
    printf("lo[%u]=%x\n",i,lineoffsets[i]);
  }
  printf("done reading lines.  at %x\n",ftell(fGRP));*/
//end
  tmp->data = (unsigned char*)malloc(tmp->w*tmp->h);
  
  memset(tmp->data,0,tmp->w*tmp->h);

xpos = xoff; ypos = 0;

  for (i=0; i<h; i++)
  {
    ypos = i + yoff; // center?
    while (xpos-xoff < w) {
//          fprintf(stderr,"at x:%u y:%u ",xpos,ypos);
    fread(buffer,1,1,fGRP);
    if (buffer[0] >= 0x80) {
      xpos += (buffer[0] - 0x80);
//          fprintf(stderr,"setting xpos to %d\n",xpos);
    } else if (buffer[0] >= 0x40) {
      loop = buffer[0] - 0x40;
      fread(buffer,1,1,fGRP);
//          fprintf(stderr,"looping %u times writing %u\n",loop,buffer[0]);
      for (j=0; j<loop; j++)
      {
        tmp->data[(tmp->w * ypos) + xpos] = buffer[0];
        xpos++;
      }
    } else {
      loop = buffer[0];
//          fprintf(stderr,"writing %u pixels: ",loop);
      for (j=0; j<loop; j++)
      {
        fread(buffer,1,1,fGRP);
//          fprintf(stderr,"%u ",buffer[0]);
        tmp->data[(tmp->w * ypos) + xpos] = buffer[0];
        xpos++;
      }
//          fprintf(stderr,"\n");
    }
   }
   xpos = xoff;
  }
  fclose(fGRP);
//  fprintf(stderr,"ok\n");
  return tmp;
}


struct grpframe *loadGRP_nonBNE(unsigned char type, struct archive_war *war) // braindead GRP loader for first frame only
{
  unsigned char *buffer,loop,*start,cmd;
//  unsigned short *lineoffsets;
  unsigned short xoff,yoff,i,j,numlines,w,h;
unsigned int idnum,len;
  signed short xpos,ypos;
  struct grpframe *tmp = NULL;

unsigned short archive_ids[4][105] = {{45,46,47,48,49,50,51,52,53,54,55,58,51,52,33,34,47,48,53,54,53,58,35,52,55,46,59,60,39,40,61,62,41,42,0,36,0,0,43,44,38,63,35,36,51,37,45,46,0,52,51,58,51,54,0,69,70,64,92,93,94,95,96,97,98,99,104,105,90,91,88,89,108,109,100,101,102,103,110,111,84,85,106,107,112,113,114,115,86,87,116,117,119,118,164,165,80,81,82,83,166,167,181,168,168},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,66,134,135,136,137,138,139,140,141,146,147,132,133,130,131,150,151,142,143,144,145,152,153,160,161,148,149,154,155,156,157,128,129,158,159,162,0,0,0,169,170,171,172,0,184,186,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,182,183,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,173,174,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,176,0,0,0,0,0,0,0,0,177,178,0,0,0,0,179,180,0,0,0,0,0,0,0,185,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,526,527,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,470,479,480,481,482,483,484,485,486,491,492,477,478,475,476,495,496,487,488,489,490,497,498,505,506,493,494,499,500,501,502,473,474,503,504,511,510,0,0,507,508,509,510,525,513,514,0,0}};

//printf("going to check archive_id for %d\n",type);

  if (archive_ids[tileset][type] != 0) idnum = archive_ids[tileset][type]; else idnum = archive_ids[0][type];

//printf("ok: idnum=%d (that's supposed to be %s)\n",idnum,basenames[type]);
  start = extractWAR(war,idnum,&len);
if (start == NULL) { fprintf(stderr,"EEEEEEEERRRORRRRRRR!\n"); return NULL;}
  buffer = start;

  tmp = (struct grpframe *)malloc(sizeof(struct grpframe));

  tmp->w = buffer[2] + 256 * buffer[3];
  tmp->h = buffer[4] + 256 * buffer[5];
buffer += 6;
  xoff = buffer[0];
  yoff = buffer[1];
  w = buffer[2];
  h = buffer[3];
  len = buffer[4] + 256 * buffer[5];
  buffer = (start + len);

//  fread(buffer,1,2,fGRP);
// cheap
  len = buffer[0] + 256*buffer[1];
  numlines = (len - (buffer - start)) / 2;
  buffer += len;

  tmp->data = (unsigned char*)malloc(tmp->w*tmp->h);
  
  memset(tmp->data,0,tmp->w*tmp->h);

xpos = xoff; ypos = 0;

  for (i=0; i<h; i++)
  {
    ypos = i + yoff; // center?
    while (xpos-xoff < w) {
//          fprintf(stderr,"at x:%u y:%u ",xpos,ypos);
    cmd = buffer[0]; buffer++;
    if (cmd >= 0x80) {
      xpos += (cmd - 0x80);
//          fprintf(stderr,"setting xpos to %d\n",xpos);
    } else if (cmd >= 0x40) {
      loop = cmd - 0x40;
      cmd = buffer[0]; buffer++;
//          fprintf(stderr,"looping %u times writing %u\n",loop,buffer[0]);
      for (j=0; j<loop; j++)
      {
        tmp->data[(tmp->w * ypos) + xpos] = cmd;
        xpos++;
      }
    } else {
      loop = cmd;
//          fprintf(stderr,"writing %u pixels: ",loop);
      for (j=0; j<loop; j++)
      {
        cmd = buffer[0]; buffer++;
//          fprintf(stderr,"%u ",buffer[0]);
        tmp->data[(tmp->w * ypos) + xpos] = cmd;
        xpos++;
      }
//          fprintf(stderr,"\n");
    }
   }
   xpos = xoff;
  }
  free(start);
//  fprintf(stderr,"ok\n");
  return tmp;
}

unsigned int drawunit(unsigned short u_x,unsigned short u_y,unsigned char type,unsigned char owner, struct Image* rendered, void* war)
{
  long i,j,xpos,ypos,imgindex;
  unsigned char mipixel;//,ispstart;
//printf("in drawUnit\n");
  if (war2_cfg.war2_isBNE) {
  if (unitimgs[type] == NULL) unitimgs[type] = loadGRP(type); } else {
  if (unitimgs[type] == NULL) unitimgs[type] = loadGRP_nonBNE(type,(archive_war *)war); }
  
  if (type == 0x16 || type == 0x23 || type == 0x28 || type == 0x29 || type == 0x2A || type == 0x2B || type == 0x38) 
  // TODO:  Eye of Kilrogg casts shadow?  Not here...
  {
  for (i=0; i<32; i++)
  {
    for (j=0; j<32; j++)
    {
        mipixel = shadowmap[j+(i*32)];
        xpos = (32 * u_x) + j;
        ypos = (32 * u_y) + i;
//        if (type < 0x39) { xpos -= unitxoff[type]; ypos -= unityoff[type]; }
        if (xpos >= 0 && ypos >= 0 && xpos < 32 * mapx && ypos < 32 * mapy && mipixel != 0) { // nontransparent pixel
#ifdef TRUCOLOR
           imgindex = 3 *    (((32 * mapx) * ypos) + xpos);
           rendered->imgdata[imgindex] = pal[mipixel][2];
           rendered->imgdata[imgindex + 1] = pal[mipixel][1];
           rendered->imgdata[imgindex + 2] = pal[mipixel][0];
#else
           imgindex = ((32 * (signed)mapx) * (signed)ypos) + (signed)xpos;
           rendered->imgdata[imgindex] = mipixel;
#endif
        }
    }
  }
  }

  for (i=0; i<unitimgs[type]->h; i++)
  {
    for (j=0; j<unitimgs[type]->w; j++)
    {
        mipixel = unitimgs[type]->data[j+(i*unitimgs[type]->w)];
        xpos = (32 * u_x) + j;
        ypos = (32 * u_y) + i;
        if (type < 0x39) { xpos -= unitxoff[type]; ypos -= unityoff[type]; }
        if (xpos >= 0 && ypos >= 0 && xpos < mapx * 32 && ypos < mapy * 32 && mipixel != 0) { // nontransparent pixel
#ifdef TRUCOLOR
        imgindex = 3 *    (((32 * mapx) * ypos) + xpos);
        if (mipixel < 208 || mipixel > 211) {
           rendered->imgdata[imgindex] = pal[mipixel][2];
           rendered->imgdata[imgindex + 1] = pal[mipixel][1];
           rendered->imgdata[imgindex + 2] = pal[mipixel][0];
        } else {
           rendered->imgdata[imgindex] = pcolors[owner][mipixel-208][0];
           rendered->imgdata[imgindex + 1] = pcolors[owner][mipixel-208][1];
           rendered->imgdata[imgindex + 2] = pcolors[owner][mipixel-208][2];
        }
#else
// TODO: this is wrong, see top!
        if (mipixel >= 208 && mipixel <= 211) mipixel += (4 * owner);
           imgindex = ((32 * (signed)mapx) * (signed)ypos) + (signed)xpos;
           rendered->imgdata[imgindex] = mipixel;
#endif
        }
    }
  }
  return 0;
}

unsigned int blit(unsigned int tilex, unsigned int tiley, unsigned short tdat, struct Image *rendered)
{
  unsigned long i,j,s,t,mgindex,miindex,imgindex;
  unsigned char mipixel,horiflip,vertflip;
  
  mgindex = tilegroups[tdat/16][tdat%16]; // get megatile number to use
//  fprintf(stderr,"%u %u %u %u %u\n",tdat,tdat/16,tdat%16,tilegroups[tdat/16][tdat%16],mgindex);

  for (s = 0; s < 4; s++)
  {
    for (t = 0; t < 4; t++)
    {
       horiflip = megatiles[(16*mgindex) + (s * 4 + t)] & 0x01;
       vertflip = megatiles[(16*mgindex) + (s * 4 + t)] & 0x02;
       miindex = megatiles[(16*mgindex) + (s * 4 + t)] >> 2;
//       fprintf(stderr,"%u (16*%u) %u %u\n",miindex,mgindex,s,t);
       for(j=0; j<8; j++)
       {
         for(i=0; i< 8; i++)
         {
//           fprintf(stderr,"ok? %u %u\n",i,j);
            if (!horiflip && !vertflip) mipixel = minitiles[i + (8*j) + (64*miindex)];
            else if (vertflip) mipixel = minitiles[(7-i) + (8*j) + (64*miindex)];
            else if (horiflip) mipixel = minitiles[i + (8*(7-j)) + (64*miindex)];
            else mipixel = minitiles[(7-i) + (8*(7-j)) + (64*miindex)];
//           fprintf(stderr,"ok!\n");
                     //24bpp //row
#ifdef TRUCOLOR
           imgindex = 3 *    (((32 * mapx) * ((32 * tiley) + (8*s) + j)) + ((32 * tilex) + (8*t) + i));
//           imgindex = 3 * (((32*mapx) * (32 * tiley + ((8*s)+j))) + (32 * tilex) + ((8*t)+i));
//           if (imgindex >= 1024*3*mapx*mapy) fprintf(stderr,"oops\n");
           rendered->imgdata[imgindex] = pal[mipixel][2];
           rendered->imgdata[imgindex + 1] = pal[mipixel][1];
           rendered->imgdata[imgindex + 2] = pal[mipixel][0];
#else
           imgindex = (((32 * mapx) * ((32 * tiley) + (8*s) + j)) + ((32 * tilex) + (8*t) + i));
           rendered->imgdata[imgindex] = mipixel;
#endif
         }
       }
    }
  }

  return 0;
}

unsigned int loadTileset()
{
  // this is a pain.
  // cv4 is tilegroups
  // ppl is palette
  // vr4 are minitiles
  // vx4 are megatiles

  unsigned long i,j,lSize;
  unsigned char buffer[80];
  FILE *fCV4=NULL, *fPPL=NULL, *fVR4=NULL, *fVX4=NULL;
  switch(tileset)
  {
    case 0:
      strcpy(setname,"forest");
      break;
    case 1:
      strcpy(setname,"iceland");
      break;
    case 2:
      strcpy(setname,"swamp");
      break;
    case 3:
      strcpy(setname,"xswamp");
      break;
    default:
      fprintf(stderr,"ERROR: Invalid tileset %u - damaged PUD?\n",tileset);
      return 1;
  }
  fprintf(stderr,"Info: Map uses tileset %s\n",setname);

  sprintf((char *)buffer,"art/bgs/%s/%s.cv4",setname,setname);
  fCV4 = fopen((char *)buffer,"rb");
  sprintf((char *)buffer,"art/bgs/%s/%s.ppl",setname,setname);
  fPPL = fopen((char *)buffer,"rb");
  sprintf((char *)buffer,"art/bgs/%s/%s.vr4",setname,setname);
  fVR4 = fopen((char *)buffer,"rb");
  sprintf((char *)buffer,"art/bgs/%s/%s.vx4",setname,setname);
  fVX4 = fopen((char *)buffer,"rb");
  
  if (fCV4 == NULL || fPPL == NULL || fVR4 == NULL || fVX4 == NULL)
  {
    fprintf(stderr,"ERROR: Bad file open on tileset %s.\n",setname);
    fclose(fCV4); fclose(fPPL); fclose(fVR4); fclose(fVX4); return 1;
  }

  fread(pal,1,768,fPPL);
  fclose(fPPL);
  for (i=0; i<256; i++)
  {
    pal[i][0] = pal[i][0] << 2;
    pal[i][1] = pal[i][1] << 2;
    pal[i][2] = pal[i][2] << 2;
  }
  
  for (i=0; i<158; i++)
  {
    for (j=0; j<16; j++)
    {
      fread(buffer,1,2,fCV4);
      tilegroups[i][j] = buffer[0] + 256 * buffer[1];
    }
    fseek(fCV4,10,SEEK_CUR); // skip 10 unknown bytes...
  }
  fclose(fCV4);

  // obtain file size:
  fseek (fVX4, 0 , SEEK_END);
  lSize = ftell (fVX4);
  rewind (fVX4);
  
//  printf("There are %d megatiles here.\n",lSize/32);

  megatiles = (unsigned short *) malloc (lSize);// sizeof(unsigned short));
  if (megatiles == NULL) { fprintf(stderr,"MEMORY ERROR\n"); return 1; }
  for (i=0; i<lSize / 2; i++)
  {
    fread(buffer,1,2,fVX4);
    megatiles[i] = buffer[0] + 256 * buffer[1];
//    printf("%d",i);
  }
  fclose(fVX4);

  // obtain file size:
  fseek (fVR4, 0 , SEEK_END);
  lSize = ftell (fVR4);
  rewind (fVR4);
  
//  printf("There are %d minitiles here.\n",lSize / 64);
  
  minitiles = (unsigned char *)malloc(lSize);
  if (minitiles == NULL) { fprintf(stderr,"MEMORY ERROR\n"); return 1; }
  fread(minitiles,1,lSize,fVR4);
  
  fclose(fVR4);
  return 0;
}

unsigned int loadNonBNETileset(struct archive_war *war) {
  unsigned int id,i,j;
  switch(tileset)
  {
    case 0: // summer
	id = 2;
      break;
    case 1:
	id = 18;
 //     strcpy(setname,"iceland");
      break;
    case 2:
	id = 10;
//      strcpy(setname,"swamp");
      break;
    case 3:
	id = 438;
      //strcpy(setname,"xswamp");
      break;
    default:
      fprintf(stderr,"ERROR: Invalid tileset %u - damaged PUD?\n",tileset);
      return 1;
  }
  fprintf(stderr,"Info: Map uses tileset %d\n",id);

	unsigned int size, vx4size,vr4size;
 unsigned char* cPPL = extractWAR(war,id,&size);
//memcpy(pal,cPPL,768); free(cPPL);
  unsigned char *cVX4 = extractWAR(war,id+1,&vx4size);
  minitiles = extractWAR(war,id+2,&vr4size);
  unsigned char *cCV4 = extractWAR(war,id+3,&size);
  
  for (i=0; i<256; i++)
  {
    pal[i][0] = cPPL[i * 3 + 2] << 2;
    pal[i][1] = cPPL[i * 3 + 1] << 2;
    pal[i][2] = cPPL[i * 3] << 2;
  }
//  free (cPPL);
  
  unsigned char *cv4buf = cCV4;
  for (i=0; i<158; i++)
  {
    for (j=0; j<16; j++)
    {
      tilegroups[i][j] = cv4buf[0] + 256 * cv4buf[1];
      cv4buf += 2;
    }
	cv4buf += 10; // skip 10 unknown bytes...
  }
  free(cCV4);

//  printf("There are %d megatiles here.\n",vx4size/32);

  megatiles = (unsigned short *) malloc (vx4size);// sizeof(unsigned short));
  if (megatiles == NULL) { fprintf(stderr,"MEMORY ERROR\n"); return 1; }
  cv4buf = cVX4;
  for (i=0; i<vx4size / 2; i++)
  {
//    fread(buffer,1,2,fVX4);
    megatiles[i] = cv4buf[0] + 256 * cv4buf[1];
	cv4buf += 2;
//    printf("%d",i);
  }
	free(cVX4);

//  printf("There are %d minitiles here.\n",vr4size / 64);
  
  return 0;
}

int initWar2()
{
        strcpy(war2_cfg.war2_dir,"C:\\WAR2");
        war2_cfg.war2_isBNE=false;
        war2_cfg.war2_drawPStart=false;
        return 0;
}

/* function to check for War2 (PUD) type */
int isWar2(struct MapFile file)
{
   for (unsigned int i=0; i<file.size-17; i++) 
     if (memcmp(&file.data[i],"TYPE\020\0\0\0WAR2 MAP\0",17) == 0) return 1;
   return 0;
}

/* War2 config - reads WAR2_DIR and WAR2_TYPE */
int cfgWar2(char *line)
{
    if (strstr(line,"WAR2_DIR ")) { strncpy(war2_cfg.war2_dir,(&line[9]),strlen(line)-10); war2_cfg.war2_dir[strlen(line)-10]='\0'; return 1; }
    else if (strstr(line,"WAR2_ISBNE")) { war2_cfg.war2_isBNE=true; return 1; }
    else if (strstr(line,"WAR2_DRAWPSTART")) { war2_cfg.war2_drawPStart=true; return 1; }
    return 0;
}


struct Image* renderWar2(struct MapFile file)
{
    struct Image *rendered = NULL;
    struct archive_war *war = NULL;
//    unsigned short tilemap[64][64];
    unsigned int i,j; //,x,y;
    
  unsigned char buffer2[80];
  char tag[5] = {'\0'};
  unsigned long length;
  
//  unsigned char filename[80];
  unsigned char desc[32];
  unsigned char *buffer = file.data;

//printf("%x < %x + %x (%x)?\n",buffer,file.data,file.size,file.data+file.size);
  
    while (buffer < file.data + file.size) {
	memcpy(tag,buffer,4);
      length = buffer[4] + 256 * buffer[5] + 65536 * buffer[6] + 16777216 * buffer[7];
//printf("[%s] (len=%lu)\n",tag,length);
	buffer += 8;
      if (strncmp(tag,"TYPE",4) == 0) {
	memcpy(buffer2,buffer,9);
	buffer += 9;
        if (strncmp((char *)buffer2,"WAR2 MAP",9) != 0) {
          fprintf(stderr,"ERROR: Map type is invalid (%s) - damaged PUD?\n",buffer2);
          return rendered;
        }
	memcpy(buffer2,buffer,7);
        buffer += 7;
        fprintf(stderr,"Info: Map type is \"WAR2 MAP\"\nInfo: Map ID is \"%s\"\n",buffer2);
      } else if (strncmp(tag,"VER ",4) == 0) {
	memcpy(buffer2,buffer,2);
        buffer += 2;
        fprintf(stderr,"Info: Map is PUD version $%x\n",buffer2[0] + 256 * buffer2[1]);
      } else if (strncmp(tag,"DESC",4) == 0) {
	memcpy(desc,buffer,32);
        buffer += 32;
        fprintf(stderr,"Info: Map description is \"%s\"\n",desc);
      } else if (strncmp(tag,"OWNR",4) == 0) {
        for (i = 0; i < 8; i++)
        {
	  memcpy(&players[i].controller,buffer,1);
          buffer++;
          players[i].present = 0;
        }
        buffer+=7;
	  memcpy(&players[8].controller,buffer,1);
          buffer++;
      } else if (strncmp(tag,"ERA ",4) == 0) {
	memcpy(buffer2,buffer,2);
	buffer += 2;
        tileset = buffer2[0] + 256 * buffer2[1];
        if (tileset > 0x03) tileset = 0;
//        if (loadTileset()) { fclose(fPUD); fclose(fBMP); return 1; }
        fprintf(stderr,"Info: Map tileset is %u\n",tileset);
      } else if (strncmp(tag,"ERAX",4) == 0) {
	memcpy(buffer2,buffer,2);
	buffer += 2;
        tileset = buffer2[0] + 256 * buffer2[1];
        if (tileset > 0x03) tileset = 0;
        fprintf(stderr,"Info: Map tilesetX is %u\n",tileset);
      } else if (strncmp(tag,"DIM ",4) == 0) {
	memcpy(buffer2,buffer,2);
	buffer += 2;
        mapx = buffer2[0] + 256 * buffer2[1];
	memcpy(buffer2,buffer,2);
	buffer += 2;
        mapy = buffer2[0] + 256 * buffer2[1];
        tilemap = (unsigned short *) malloc (mapy*mapx * sizeof(unsigned short));
        if (tilemap == NULL) { fprintf(stderr,"ERROR: out of memory allocating tilemap.\n"); return NULL; }
        fprintf(stderr,"Info: Map dimensions are %u x %u\n",mapx,mapy);
      } else if (strncmp(tag,"UDTA",4) == 0) {
	memcpy(buffer2,buffer,2);
	buffer += 2;
        if (buffer2[0] == 0)
          fprintf(stderr,"Info: Map contains custom unit data.\n");
        else
          fprintf(stderr,"Info: Map uses default unit data.\n");
	buffer += (length-2);
      } else if (strncmp(tag,"ALOW",4) == 0) {
        //don't care much about this now, although it may be interesting
        // to note in map info
	buffer += length;
      } else if (strncmp(tag,"UGRD",4) == 0) {
	memcpy(buffer2,buffer,2);
	buffer += 2;
        if (buffer2[0] == 0)
          fprintf(stderr,"Info: Map contains custom upgrade data.\n");
        else
          fprintf(stderr,"Info: Map uses default upgrade data.\n");
	buffer += (length-2);
      } else if (strncmp(tag,"SIDE",4) == 0) {
        for (i = 0; i < 8; i++){
          players[i].race=buffer[0];
		buffer++;
	}
		buffer+=7;
          players[8].race=buffer[0];
	buffer++;
      } else if (strncmp(tag,"SGLD",4) == 0) {
        for (i = 0; i < 8; i++)
        {
	  memcpy(buffer2,buffer,2);
	  buffer += 2;
          players[i].gold = buffer2[0] + 256 * buffer2[1];
        }
	  buffer += 14;
	  memcpy(buffer2,buffer,2);
	  buffer += 2;
        players[8].gold = buffer2[0] + 256 * buffer2[1];
      } else if (strncmp(tag,"SLBR",4) == 0) {
        for (i = 0; i < 8; i++)
        {
	  memcpy(buffer2,buffer,2);
	  buffer += 2;
          players[i].lumber = buffer2[0] + 256 * buffer2[1];
        }
	  buffer += 14;
	  memcpy(buffer2,buffer,2);
	  buffer += 2;
        players[8].lumber = buffer2[0] + 256 * buffer2[1];
      } else if (strncmp(tag,"SOIL",4) == 0) {
        for (i = 0; i < 8; i++)
        {
	  memcpy(buffer2,buffer,2);
	  buffer += 2;
          players[i].oil = buffer2[0] + 256 * buffer2[1];
        }
	  buffer += 14;
	  memcpy(buffer2,buffer,2);
	  buffer += 2;
        players[8].oil = buffer2[0] + 256 * buffer2[1];
      } else if (strncmp(tag,"AIPL",4) == 0) {
        // Don't care much about AI info
	  buffer += length;
      } else if (strncmp(tag,"MTXM",4) == 0) {
//        fread(tilemap,1,mapx*mapy*2,fPUD);
          for (i=0; i<(unsigned)(mapx*mapy); i++) {
            tilemap[i] = buffer[0] + 256 * buffer[1];
	    buffer += 2;
          }        
      } else if (strncmp(tag,"SQM ",4) == 0) {
        // Don't care much about movement map info
	  buffer += length;
      } else if (strncmp(tag,"OILM",4) == 0) {
        // Oil map info is completely unused
	  buffer += length;
      } else if (strncmp(tag,"REGM",4) == 0) {
        // Don't care much about action map info
	  buffer += length;
      } else if (strncmp(tag,"UNIT",4) == 0) {
        num_units = length / 8;
        units = (struct unit*)malloc(sizeof(struct unit) * num_units);
        fprintf(stderr,"Info: Map contains %lu units.\n",num_units);
        for (i=0; i<num_units; i++) {
//          fread(buffer,1,2,fPUD);
          units[i].x = buffer[0] + 256 * buffer[1];
//          fread(buffer,1,2,fPUD);
          units[i].y = buffer[2] + 256 * buffer[3];
          units[i].type=buffer[4];
          units[i].owner=buffer[5];
          units[i].arg = buffer[6] + 256 * buffer[7];
		buffer += 8;
        }
      } else if (strncmp(tag,"SIGN",4) == 0) {
        fprintf(stderr,"Info: Map has official signature %08x\n",*((unsigned int*)buffer));
		buffer += 4;
      } else {
//        if (!feof(fPUD)) {
          fprintf(stderr,"ERROR: Unknown section [%s] encountered - damaged PUD?\n",tag);
//          fseek(fPUD,length,SEEK_CUR);
          // we used to exit here, but evidently PUD has multiple revisions...
          // just try seeking past it here.
//          return NULL;
		buffer += length;
//        }
      }
    }

    rendered = (struct Image*) malloc (sizeof (struct Image));
    if (rendered == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); return NULL; }

    // War2 maps are always 64x64 tiles, 16 pixels/tile, 8bpp
    rendered->x = 32 * mapx;
    rendered->y = 32 * mapy;
// TODO: figure out how to reduce this to 8bpp!!!!!!
    rendered->bpp = 24;
    rendered->imgdata = (unsigned char*)malloc(3 * 1024 * mapx * mapy * sizeof(unsigned char));

    if (rendered->imgdata == NULL) { fprintf(stderr,"couldn't malloc image struct\n"); free(rendered); return NULL; }

    if (war2_cfg.war2_isBNE) { } else
    { char filename[256]; strcpy(filename,war2_cfg.war2_dir); strcat(filename,"/data/maindat.war");
//printf(" WAR1: Using '%s' for WAR file\n", filename);
    war = openWAR(filename);
    if (war == NULL) { fprintf(stderr,"couldn't open WAR\n"); free(rendered->imgdata); free(rendered); return NULL; } }

    if (war2_cfg.war2_isBNE) {
    if (loadTileset()) { fprintf(stderr,"couldn't load war2 tileset data\n"); free (rendered->imgdata); free (rendered); free(units); return NULL; } } else {
    if (loadNonBNETileset(war)) { fprintf(stderr,"couldn't load war2 tileset data\n"); free (rendered->imgdata); free (rendered); free(units); return NULL; } }

/*#ifdef TRUCOLOR
    rendered->imgdata = (unsigned char*)malloc(3 * 1024 * mapx * mapy * sizeof(unsigned char));
#else
    rendered->imgdata = (unsigned char*)malloc(1024 * mapx * mapy * sizeof(unsigned char));
#endif */

    for (j=0; j<mapy; j++)
      for (i=0; i<mapx; i++)
        blit(i,j,tilemap[(i+(j*mapx))],rendered);

    for (i=0; i<num_units; i++)
      if (units[i].type == 0x5E || units[i].type == 0x5F)
      {
        players[units[i].owner].present = 1;
        if (war2_cfg.war2_drawPStart) drawunit(units[i].x,units[i].y,units[i].type,units[i].owner,rendered,war);
      } else {
        drawunit(units[i].x,units[i].y,units[i].type,units[i].owner,rendered,war);
      } 
  
    free(megatiles);
    free(minitiles);

    free(units);
    free(tilemap);

    for (i=0; i<104; i++)
    {
      if (unitimgs[i] != NULL) {
        free(unitimgs[i]->data);
        free(unitimgs[i]);
	unitimgs[i] = NULL;
      }
    }

    // produce report
/*    int numplayers = 0;
    for (i=0; i<8; i++)
      if (players[i].controller == 0x05) numplayers++;

    if (tileset == 0x01) strcpy(setname,"Winter");
    else if (tileset == 0x02) strcpy(setname,"Wasteland");
    else if (tileset == 0x03) strcpy(setname,"Draenor");
    else strcpy(setname,"Forest");

    printf("Filename:    %s\nDescription: %s\nDimensions:  %u x %u\nTerrain:     %s\nPlayers:     %u\n\n",filename,desc,mapx,mapy,setname,numplayers);//,players[0].gold,players[0].lumber,players[0].oil);

    const char *colors[] = {"Red","Blue","Green","Purple","Orange","Black","White","Yellow"};
    const char *controllers[] = {"Computer","Computer","Computer","Nobody","Computer","Person","Rescue","Rescue"};
    
    for (i=0; i<8; i++)
    {
        if (players[i].present) printf("Player %u (%s) %s - %s\n  (G=%u  L=%u  O=%u)\n\n",i+1,colors[i],controllers[players[i].controller % 0x08],(players[i].race==0x00?"Human":"Orc"),players[i].gold,players[i].lumber,players[i].oil);
    } */
    return rendered;
}
