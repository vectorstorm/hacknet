#ifndef __OBJ_WEAPON_H__
#define __OBJ_WEAPON_H__

#include "OBJ_Base.h"

class objWeapon : public objBase
{
	sint8			m_enchantment;
public:
				objWeapon( uint32 type );
	virtual		 	~objWeapon();

	virtual bool		SetWieldedPrimary(bool);
	virtual bool		SetWieldedSecondary(bool);

	virtual sint16		RollDamage( entBase *foe );
};

#endif // __OBJ_LONGSWORD_H__
