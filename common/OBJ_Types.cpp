
#include "OBJ_Types.h"
#include "stdio.h"

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

void GetObjectDescriptionText(const objDescription &desc, char *buffer, uint16 bufferLength)
{
	const char * name = GetObjectName(desc.type);

	if (desc.count == 0)
		buffer[0]='\0';
	else if ( desc.count == 1 )
	{
		if ( name[0] == 'a' || name[0] == 'i' || name[0] == 'u' )
			snprintf(buffer, bufferLength, "an %s", name);
		else
			snprintf(buffer, bufferLength, "a %s", name);
	}
	else
	{
		snprintf(buffer, bufferLength, "%d %ss", desc.count, name);
	}
}
