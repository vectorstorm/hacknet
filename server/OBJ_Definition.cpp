#include "OBJ_Definition.h"

objPrototype g_objectPrototype[] =
{
	{"null",NULL,false,false,false,false,false,false,false,DIR_None, SKILL_Dagger, 0, MAT_Iron, Weapon, 1, 0, 1, 1, 1, 1},
	{"dagger", NULL, true, true, false, false, false, false, false, DIR_Slash, SKILL_Dagger, 0, MAT_Iron, Weapon, 1, 1, 1, 1, 1, 1},
	{"long sword", NULL, true, true, false, false, false, false, false, DIR_Slash, SKILL_LongSword, 0, MAT_Iron, Weapon, 1, 1, 1, 1, 8, 4}
};


int GetDefinitionCount()
{
	return ( sizeof(g_objectPrototype) / sizeof(objPrototype) );
}
