#include "OBJ_Dagger.h"
#include "OBJ_Types.h"
#include <stdlib.h>


char * objDagger::s_name = "dagger";


objDagger::objDagger(const hnPoint &where) :
	objBase(WEAPON_Dagger, where)
{
}

objDagger::~objDagger()
{
}

sint16
objDagger::RollDamage()
{
	return (rand() % 4) + 1;
}
