#ifndef __OBJ_TYPES_H__
#define __OBJ_TYPES_H__

#include "HN_Types.h"

enum {
        OBJ_TYPE_Random,
        OBJ_TYPE_Illegal,
	OBJ_TYPE_None,
        OBJ_TYPE_Amulet,
        OBJ_TYPE_Weapon,
        OBJ_TYPE_Armour,
        OBJ_TYPE_Potion,
        OBJ_TYPE_Ring,
        OBJ_TYPE_Tool,
        OBJ_TYPE_Food,
        OBJ_TYPE_Scroll,
        OBJ_TYPE_Spellbook,
        OBJ_TYPE_Wand,
        OBJ_TYPE_Gold,
        OBJ_TYPE_Gem,
        OBJ_TYPE_Rock,
        OBJ_TYPE_Ball,
        OBJ_TYPE_Chain,
        OBJ_TYPE_Venom,
        OBJ_TYPE_MAX
};
typedef uint16 objType;


enum {
	WORN_Suit,
	WORN_Helmet,
	WORN_Shield,
	WORN_Boots,
	WORN_Gloves,
	WORN_Amulet,
	WORN_LeftRing,
	WORN_RightRing,
	WORN_MAX
};
typedef uint16 wornType;
	

enum {
	BC_Unknown,
	BC_Uncursed,
	BC_Blessed,
	BC_Cursed
};

struct objDescription
{
	objType		type;
	uint16		itemID;		// what type of object
	uint8		blesscurse;	// blessed, cursed, or uncursed.
	uint8		count;		// how many of them?
};

#endif //__OBJ_TYPES_H__
