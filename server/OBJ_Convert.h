/*  OBJ_Convert.h
     Contains necessary #defines to convert from NetHack-style
     object property enums to HackNet-style object property enums  */

#ifndef OBJ_CONVERT_H
#define OBJ_CONVERT_H

#include "OBJ_Types.h"
#include "OBJ_Definition.h"

/* Skills */

#define P               DIR_Pierce
#define S               DIR_Slash
#define B               DIR_Whack

#define P_DAGGER                SKILL_Dagger
#define P_KNIFE                 SKILL_Knife
#define P_AXE                   SKILL_Axe
#define P_SHORT_SWORD           SKILL_ShortSword
#define P_SCIMITAR              SKILL_Scimitar
#define P_SABER                 SKILL_Saber
#define P_BROAD_SWORD           SKILL_BroadSword
#define P_LONG_SWORD            SKILL_LongSword
#define P_TWO_HANDED_SWORD      SKILL_TwoHandedSword
#define P_POLEARMS              SKILL_Polearms
#define P_LANCE                 SKILL_Lance
#define P_PICK_AXE              SKILL_PickAxe
#define P_MACE                  SKILL_Mace
#define P_MORNING_STAR          SKILL_MorningStar
#define P_HAMMER                SKILL_Hammer
#define P_CLUB                  SKILL_Club
#define P_WHIP                  SKILL_Whip
#define P_QUARTERSTAFF          SKILL_Quarterstaff
#define P_FLAIL                 SKILL_Flail

/* Materials */

#define IRON            MAT_Iron
#define METAL           MAT_Metal
#define WOOD            MAT_Wood
#define MINERAL         MAT_Mineral
#define SILVER          MAT_Silver
#define LEATHER         MAT_Leather

/*  Colors */

#define HI_METAL        COLOR_Metal
#define HI_WOOD         COLOR_Wood
#define HI_SILVER       COLOR_Silver
#define CLR_BLACK       COLOR_Black
#define CLR_WHITE       COLOR_White
#define CLR_BROWN       COLOR_Brown


#endif  // OBJ_CONVERT_H
