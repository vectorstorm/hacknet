#include <string.h>
#include "ENT_Base.h"
#include "MAP_Base.h"
#include "HN_Dungeon.h"

#include "assert.h"

extern hnPoint offsetVector[10];

entBase::entBase( entType type, const hnPoint & pos ):
	m_type(type),
	m_position(pos),
	m_changedLevel(false)
{
	//m_prev = m_next = this;
}

entBase::~entBase()
{
}

void
entBase::SetName( char * name )
{
	strncpy( m_name, name, MAX_NAME_BYTES );
	m_name[MAX_NAME_BYTES-1] = '\0';
}

char *
entBase::GetName()
{
	return m_name;
}

const hnPoint &
entBase::GetPosition()
{
	return m_position;
}

void
entBase::SetPosition( const hnPoint & pos )
{
	m_position = pos;
}

void
entBase::Move( hnDirection dir )
{
	// By the time we're here, we've already determined that the requested
	// move is legal.  So go ahead and do it.
	
	hnPoint pos = m_position + offsetVector[dir];
	
	mapBase *origMap = hnDungeon::GetLevel( m_position.z );
	mapBase *map = hnDungeon::GetLevel( pos.z );
	assert(map != NULL);
	
	if ( dir == DIR_Up )
	{
		hnPoint2D stairsPos = map->GetDownStairs();
		pos.x = stairsPos.x;
		pos.y = stairsPos.y;
		m_changedLevel = true;
	}
	else if ( dir == DIR_Down )
	{
		hnPoint2D stairsPos = map->GetUpStairs();
		pos.x = stairsPos.x;
		pos.y = stairsPos.y;
		m_changedLevel = true;
	}
	
	origMap->RemoveEntity(this);
	map->PutEntityAt(this, pos.x, pos.y);
	
	m_position = pos;
}
