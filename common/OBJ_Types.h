#ifndef __OBJ_TYPES_H__
#define __OBJ_TYPES_H__

#include "HN_Types.h"

enum {
        Random,
        Illegal,
	None,
        Amulet,
        Weapon,
        Armour,
        Potion,
        Ring,
        Tool,
        Food,
        Scroll,
        Spellbook,
        Wand,
        Gold,
        Gem,
        Rock,
        Ball,
        Chain,
        Venom,
        MAX
};
typedef uint16 objType;


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
