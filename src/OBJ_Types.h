#ifndef __OBJ_TYPES_H__
#define __OBJ_TYPES_H__

enum
{
	OBJECT_None,
	
	OBJECT_Potion,		// there should be one of these for each item type.
	OBJECT_Armor,
	OBJECT_Weapon,

	OBJECT_MAX
};
typedef uint8 objType;

#endif //__OBJ_TYPES_H__
