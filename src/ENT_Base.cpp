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

bool
entBase::IsValidMove( hnDirection dir )
{
	bool legalMove = false;
	bool blocked = false;
	// validate the direction we've been given, and return true if we're
	// able to go that way; false if not.

	if ( dir >= 0 && dir < 8 )
	{
		// north->northwest
		hnPoint potentialPos = offsetVector[dir] + GetPosition();

		if ( hnDungeon::GetLevel(potentialPos.z)->WallAt(potentialPos.x,potentialPos.y) & WALL_Passable )
			if ( hnDungeon::GetLevel(potentialPos.z)->MapTile(potentialPos.x,potentialPos.y).entity == NULL )
			{
				legalMove = true;
			}
			else
				blocked = true;
	}
	else
	{
		// up or down -- check for appropriate stairways
		hnPoint currentPos = GetPosition();
		if ( dir == DIR_Up )
		{
			if ( hnDungeon::GetLevel(currentPos.z)->WallAt(currentPos.x,currentPos.y) & WALL_StairsUp )
			{
				hnPoint2D stairsPos = hnDungeon::GetLevel(currentPos.z-1)->GetDownStairs();
				if ( hnDungeon::GetLevel(currentPos.z-1)->MapTile(stairsPos.x, stairsPos.y).entity == NULL )
				{
					legalMove = true;       // nobody standing where we want to go.
				}
				else
					blocked = true;
			}
		}
		else if ( dir == DIR_Down )
		{
			if ( hnDungeon::GetLevel(currentPos.z)->WallAt(currentPos.x,currentPos.y) & WALL_StairsDown )
			{
				hnPoint2D stairsPos = hnDungeon::GetLevel(currentPos.z+1)->GetUpStairs();
				if ( hnDungeon::GetLevel(currentPos.z+1)->MapTile(stairsPos.x, stairsPos.y).entity == NULL )
				{
					legalMove = true;       // nobody standing where we want to go.
				}
				else
					blocked = true;
			}
		}
	}
/*	
	if ( blocked )
	{
		char *buffer = "Someone is blocking the way.";
		netServer::GetInstance()->StartMetaPacket( playerID );
		netServer::GetInstance()->SendMessage(buffer);
		netServer::GetInstance()->TransmitMetaPacket();
	}
*/
	return legalMove;
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
