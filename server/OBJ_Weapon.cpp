#include "OBJ_Manager.h"
#include "OBJ_Weapon.h"
#include <stdlib.h>


objWeapon::objWeapon(uint32 type) :
	objBase(type),
	m_enchantment(0)
{
}

objWeapon::~objWeapon()
{
}

sint16
objWeapon::RollDamage( entBase * foe )
{
        const objPrototype &proto = objManager::GetInstance()->GetPrototype( GetType() );
	
	sint8 damage = objBase::RollDamage(foe);
	
	// add our enchantments...
	
	damage += m_enchantment;	// since we could be cursed, we must
	if ( damage < 0 ) damage = 0;	// make sure damage doesn't go negative.
	
	return damage;
}
