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
	FLAG_Worn_Suit 		= 0x0001,
	FLAG_Worn_Helm 		= 0x0002,
	FLAG_Worn_Shield 	= 0x0004,
	FLAG_Worn_Boots 	= 0x0008,
	FLAG_Worn_Gloves 	= 0x0010,
	FLAG_Worn_Cloak		= 0x0020,
	FLAG_Worn_Shirt		= 0x0040,
	FLAG_Worn_Amulet 	= 0x0080,
	FLAG_Worn_LeftRing 	= 0x0100,
	FLAG_Worn_RightRing 	= 0x0200,
	FLAG_Wearable		= 0x0400,	// set if this object can be worn right now.  (not set if already worn)
	
	FLAG_WieldedPrimary 	= 0x0800,
	FLAG_WieldedSecondary 	= 0x1000,
	FLAG_Wieldable		= 0x2000,	// set if this object can be wielded right now.  (not set if already wielded)
						// note that any object can be wielded..  this is a cue that this is a real weapon that should be offered as a selection.
	
	FLAG_Legal		= 0x8000	// every object should have this bit set.
};
#define FLAG_Worn		(FLAG_Worn_Suit|FLAG_Worn_Helm|FLAG_Worn_Shield \
				|FLAG_Worn_Boots|FLAG_Worn_Gloves \
				|FLAG_Worn_Amulet|FLAG_Worn_Amulet \
				|FLAG_Worn_LeftRing|FLAG_Worn_RightRing)
#define FLAG_Wielded		(FLAG_WieldedPrimary|FLAG_WieldedSecondary)


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
	uint16		flags;
	uint8		blesscurse;	// blessed, cursed, or uncursed.
	uint8		count;		// how many of them?
};

#endif //__OBJ_TYPES_H__
