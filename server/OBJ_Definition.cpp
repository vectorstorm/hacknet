#include "OBJ_Definition.h"

objPrototype g_objectPrototype[] =
{
	{"null",NULL,
		false,false,false,false,false,false,false,DIR_None, SKILL_Dagger, 0, MAT_Iron, Weapon, 1, 0, 1, 1, 1, 1},
	{"dagger", NULL, 
		true, true, false, false, false, false, false, DIR_Slash, SKILL_Dagger, 0, MAT_Iron, Weapon, 1, 30, 10, 4, 4, 3},
	{"elven dagger", "runed dagger", 
		true, true, false, false, false, false, false, DIR_Slash, SKILL_Dagger, 0, MAT_Wood, Weapon, 1, 10, 10, 4, 5, 3},
	{"orcish dagger", "crude dagger", 
		true, true, false, false, false, false, false, DIR_Slash, SKILL_Dagger, 0, MAT_Iron, Weapon, 1, 12, 10, 4, 3, 3},
	{"silver dagger", NULL, 
		true, true, false, false, false, false, false, DIR_Slash, SKILL_Dagger, 0, MAT_Silver, Weapon, 1, 3, 12, 40, 4, 3},
	{"athame", NULL, 
		true, true, false, false, false, false, false, DIR_Slash, SKILL_Dagger, 0, MAT_Iron, Weapon, 1, 0, 10, 4, 4, 3},
	{"long sword", NULL, 
		true, true, false, false, false, false, false, DIR_Slash, SKILL_LongSword, 0, MAT_Iron, Weapon, 1, 50, 40, 15, 8, 12},
	{"two-handed sword", NULL,
		true, true, false, false, false, false, false, DIR_Slash, SKILL_TwoHandedSword, 0, MAT_Iron, Weapon, 1, 22, 150, 50, 12, 6}
		
};


int GetDefinitionCount()
{
	return ( sizeof(g_objectPrototype) / sizeof(objPrototype) );
}
