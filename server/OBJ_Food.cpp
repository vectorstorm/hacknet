#include "OBJ_Food.h"
#include "OBJ_Types.h"
#include "OBJ_Definition.h"
#include "OBJ_Manager.h"
#include "ENT_Base.h"
#include <stdio.h>
#include <assert.h>

objFood::objFood(uint32 id) : 
	objBase(id)
{
	m_flags |= FLAG_Eatable;	// set us as eatable.
}

objFood::~objFood()
{
}

bool
objFood::Eat(entBase *entity, hnPlayer *player)
{
	assert(entity);		// _somebody_ must be eating me!

	const objPrototype &proto = objManager::GetInstance()->GetPrototype( GetItemID() );

	// we should now add prototype.nutrition to the entity's eat.
	
	entity->DoEat( proto.nutrition );

	return true;
}
