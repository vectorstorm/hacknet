#include "OBJ_Dagger.h"
#include "OBJ_Types.h"


char * objDagger::s_name = "dagger";


objDagger::objDagger(const hnPoint &where) :
	objBase(WEAPON_Dagger, where)
{
}

objDagger::~objDagger()
{
}
