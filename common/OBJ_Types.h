#ifndef __OBJ_TYPES_H__
#define __OBJ_TYPES_H__

#include "HN_Types.h"

enum
{
	OBJECT_None,
	
	/*                POTIONS                  */
	POTION_Bubbly,		// there should be one of these for each item type.

	/*                 ARMOURS                  */
	ARMOUR_Leather,

	/*                 WEAPONS                 */
	WEAPON_Dagger,
	WEAPON_LongSword,

	/*                ARTIFACTS                */
	AMULET_ImitationYendor,		// Accept no substitute!
	AMULET_Yendor,			// the goal of the whole thing!

	OBJECT_MAX
};
typedef uint16 objType;

const char * GetObjectName(objType objectID);

enum {
	BC_Unknown,
	BC_Uncursed,
	BC_Blessed,
	BC_Cursed
};

struct objDescription
{
	objType type;		// what type of object
	uint8	blesscurse;	// blessed, cursed, or uncursed.
	uint8	count;		// how many of them?
};

#endif //__OBJ_TYPES_H__
