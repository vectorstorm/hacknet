#ifndef __HN_ENTITY_H__
#define __HN_ENTITY_H__

#include "HN_Object.h"

// an entity is an object that's alive.  (ie: a player, a monster, an NPC, etc).  No more than one
// entity may be in a single square at any one time.

class hnEntity: public hnObject
{
public:
			hnEntity( hnObjectType, const hnPoint & );
	virtual		~hnEntity();
};


#endif // __HN_ENTITY_H__
