#ifndef OBJ_ARMOUR_H
#define OBJ_ARMOUR_H

#include "OBJ_Base.h"

class objArmour: public objBase
{
public:
			objArmour( uint32 type );
	virtual 	~objArmour();

	virtual bool	SetWorn(bool);
};

#endif // OBJ_ARMOUR_H
