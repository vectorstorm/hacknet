#ifndef __OBJ_WEAPON_H__
#define __OBJ_WEAPON_H__

#include "OBJ_Base.h"

class objWeapon : public objBase
{
public:
				objWeapon( uint32 type );
	virtual		 	~objWeapon();

	virtual sint16		RollDamage();
};

#endif // __OBJ_LONGSWORD_H__
