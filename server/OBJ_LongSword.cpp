#include "OBJ_LongSword.h"
#include "OBJ_Types.h"


char * objLongSword::s_name = "long sword";


objLongSword::objLongSword(const hnPoint &where) :
	objBase(WEAPON_LongSword, where)
{
}

objLongSword::~objLongSword()
{
}
