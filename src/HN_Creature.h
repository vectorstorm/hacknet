#ifndef __HN_CREATURE_H__
#define __HN_CREATURE_H__

#include "HN_Entity.h"

// an entity is an object that's alive.  (ie: a player, a monster, an NPC, etc).  No more than one
// entity may be in a single square at any one time.

class hnObject;

class hnCreature: public hnEntity
{
	uint16		m_hitPoints;
	uint16		m_speed;		// egads.. speed is going to be complicated to code in
						// multiplayer, isn't it?
	
	hnObject	*m_inventory;		// m_inventory is a null object used to store a circular
						// linked list of the objects we're carrying.
	
public:
			hnCreature( hnEntityType, const hnPoint & );
	virtual		~hnCreature();
};


#endif // __HN_CREATURE_H__
