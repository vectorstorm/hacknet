#ifndef OBJ_AMULET_H
#define OBJ_AMULET_H

#include "OBJ_Base.h"

class objAmulet : public objBase
{
public:
			objAmulet(uint32 id);
			~objAmulet();
	
	virtual bool	SetWorn(bool);
};

#endif // OBJ_AMULET_H
