#include "OBJ_Manager.h"
#include "OBJ_Weapon.h"
#include <stdlib.h>


objWeapon::objWeapon(uint32 type) :
	objBase(type)
{
}

objWeapon::~objWeapon()
{
}

sint16
objWeapon::RollDamage()
{
	const objPrototype &proto = objManager::GetInstance()->GetPrototype( GetType() );
	sint16 damage = (rand() % proto.smallDamage)+1;

	return damage;
}
