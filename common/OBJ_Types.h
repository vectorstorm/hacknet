#ifndef __OBJ_TYPES_H__
#define __OBJ_TYPES_H__

#include "HN_Types.h"

enum
{
	OBJECT_None,
	
	/*                POTIONS                  */
	POTION_MIN,
	POTION_Bubbly,		// there should be one of these for each item type.
	POTION_MAX,
	
	/*                 ARMOURS                  */
	ARMOUR_MIN,
	ARMOUR_Leather,
	ARMOUR_MAX,
	
	/*                 WEAPONS                 */
	WEAPON_MIN,
	WEAPON_Dagger,
	WEAPON_LongSword,
	WEAPON_MAX,
	
	/*                ARTIFACTS                */
	AMULET_MIN,
	AMULET_ImitationYendor,		// Accept no substitute!
	AMULET_Yendor,			// the goal of the whole thing!
	AMULET_MAX,
	
	OBJECT_MAX
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
	objType type;		// what type of object
	uint8	blesscurse;	// blessed, cursed, or uncursed.
	uint8	count;		// how many of them?
};

const char * 	GetObjectName(objType objectID);
void		GetObjectDescriptionText(const objDescription &desc, char *buffer, uint16 bufferLen);

#endif //__OBJ_TYPES_H__
