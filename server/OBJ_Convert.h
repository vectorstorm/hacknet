/*  OBJ_Convert.h
     Contains necessary #defines to convert from NetHack-style
     object property enums to HackNet-style object property enums  */

#ifndef OBJ_CONVERT_H
#define OBJ_CONVERT_H

#include "OBJ_Types.h"
#include "OBJ_Definition.h"
#include "HN_Properties.h"

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

#define P_DART			SKILL_Dart
#define P_SHURIKEN		SKILL_Shuriken
#define P_BOOMERANG		SKILL_Boomerang
#define P_SPEAR			SKILL_Spear
#define P_JAVELIN		SKILL_Javelin
#define P_TRIDENT		SKILL_Trident

#define P_BOW			SKILL_Bow
#define P_SLING			SKILL_Sling
#define P_CROSSBOW		SKILL_Crossbow

/*  Armour Types  */

#define ARM_SUIT		ARM_Suit
#define ARM_SHIRT		ARM_Shirt

/* Materials */

#define IRON            MAT_Iron
#define METAL           MAT_Metal
#define WOOD            MAT_Wood
#define MINERAL         MAT_Mineral
#define SILVER          MAT_Silver
#define LEATHER         MAT_Leather
#define CLOTH		MAT_Cloth
#define DRAGON_HIDE	MAT_Dragon_Hide
#define MITHRIL		MAT_Mithril
#define GLASS		MAT_Glass
#define COPPER		MAT_Copper

#define HI_METAL        MAT_Metal
#define HI_WOOD         MAT_Wood
#define HI_SILVER       MAT_Silver
#define HI_COPPER	MAT_Copper
#define HI_CLOTH	MAT_Cloth
#define HI_LEATHER	MAT_Leather

/*  Colors */

#define CLR_BLACK       	COLOR_Black
#define CLR_WHITE       	COLOR_White
#define CLR_BROWN       	COLOR_Brown
#define CLR_GREEN		COLOR_Green
#define CLR_RED			COLOR_Red
#define CLR_BLUE		COLOR_Blue
#define CLR_MAGENTA		COLOR_Magenta
#define CLR_BRIGHT_MAGENTA	COLOR_Magenta
#define CLR_GRAY		COLOR_Gray
#define CLR_YELLOW		COLOR_Yellow
#define CLR_ORANGE		COLOR_Orange

/* Properties */

#define SHOCK_RES		PROP_Shock_Resistance
#define SLEEP_RES		PROP_Sleep_Resistance
#define DISINT_RES		PROP_Disintegration_Resistance
#define POISON_RES		PROP_Poison_Resistance
#define ACID_RES		PROP_Acid_Resistance
#define FIRE_RES		PROP_Fire_Resistance
#define COLD_RES		PROP_Cold_Resistance
#define STEALTH			PROP_Stealth
#define PROTECTION		PROP_Protection
#define INVIS			PROP_Invisible
#define ANTIMAGIC		PROP_Antimagic
#define DISPLACED		PROP_Displaced
#define REFLECTING		PROP_Reflecting
#define FUMBLING		PROP_Fumbling
#define FAST			PROP_Fast
#define WWALKING		PROP_Water_Walking
#define JUMPING			PROP_Jumping
#define LEVITATION		PROP_Levitation
#define CLAIRVOYANT		PROP_Clairvoyant
#define TELEPAT			PROP_Telepathy

#endif  // OBJ_CONVERT_H
