#ifndef OBJ_ARMOUR_H
#define OBJ_ARMOUR_H

#include "OBJ_Base.h"

class objArmour: public objBase
{
	uint16		m_wearFlag;
public:
			objArmour( uint32 type );
	virtual 	~objArmour();

	virtual bool	SetWorn(bool);
};

#endif // OBJ_ARMOUR_H
