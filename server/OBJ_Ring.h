#ifndef OBJ_RING_H
#define OBJ_RING_H

#include "OBJ_Base.h"

class objRing : public objBase
{
public:
			objRing(uint32 id);
			~objRing();

	virtual bool	SetWorn(bool);
};

#endif // OBJ_AMULET_H
