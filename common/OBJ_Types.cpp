
#include "OBJ_Types.h"

	static const char * g_objectName[OBJECT_MAX+1] = {
		"No Object",
		
		"bubbly potion",
		"leather armour",

		"dagger",
		"long sword",

		"cheap plastic imitation of the Amulet of Yendor",
		"the Amulet of Yendor",

		"OBJECT TYPE ERROR"
	};
	
const char * GetObjectName(objType id)
{
	
	if ( id < OBJECT_MAX )
	{
		return g_objectName[id];
	}
	
	return g_objectName[OBJECT_MAX];
}
