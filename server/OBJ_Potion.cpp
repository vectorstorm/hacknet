#include "OBJ_Potion.h"
#include "HN_Player.h"
#include <assert.h>

objPotion::objPotion(uint32 id) : objBase(id)
{
}

objPotion::~objPotion()
{
}

bool
objPotion::Quaff(entBase *entity, hnPlayer *player)
{
	assert(entity);		// Well _somebody_ must be quaffing me!
	
	if ( player )
	{
		// player did it, so send a message to him.

		player->Listen("That was refreshing!\n");
	}
	else
	{
		// TODO: monster quaffed us, so register a 'see' event.
	}

	m_count--;
	
	return true;
}
