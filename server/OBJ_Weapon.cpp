#include "OBJ_Manager.h"
#include "OBJ_Weapon.h"
#include <stdlib.h>


objWeapon::objWeapon(uint32 type) :
	objBase(type),
	m_enchantment(0)
{
	m_flags |= FLAG_Wieldable;
}

objWeapon::~objWeapon()
{
}

bool
objWeapon::SetWieldedPrimary(bool wielded)
{
	objBase::SetWieldedPrimary(wielded);

	if ( m_flags & FLAG_Wielded )
		m_flags &= ~FLAG_Wieldable;
	else
		m_flags |= FLAG_Wieldable;
}


bool
objWeapon::SetWieldedSecondary(bool wielded)
{
	objBase::SetWieldedSecondary(wielded);

	if ( m_flags & FLAG_Wielded )
		m_flags &= ~FLAG_Wieldable;
	else
		m_flags |= FLAG_Wieldable;
}

sint16
objWeapon::RollDamage( entBase * foe )
{
        const objPrototype &proto = objManager::GetInstance()->GetPrototype( GetItemID() );
	
	sint8 damage = objBase::RollDamage(foe);
	
	// add our enchantments...
	
	damage += m_enchantment;	// since we could be cursed, we must
	if ( damage < 0 ) damage = 0;	// make sure damage doesn't go negative.
	
	return damage;
}
