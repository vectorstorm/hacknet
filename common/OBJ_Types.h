#ifndef __OBJ_TYPES_H__
#define __OBJ_TYPES_H__

enum
{
	OBJECT_None,
	
	OBJECT_Potion,		// there should be one of these for each item type.
	OBJECT_Armor,
	OBJECT_LongSword,


	OBJECT_AmuletOfYendor,	// the goal of the whole thing!

	OBJECT_MAX
};
typedef uint8 objType;

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
