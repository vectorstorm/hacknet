#include <string.h>
#include "ENT_Base.h"
#include "MAP_Base.h"
#include "HN_Dungeon.h"

#include "assert.h"

extern hnPoint offsetVector[10];

entBase::entBase( entType type, const hnPoint & pos, bool playerControlled ):
	m_type(type),
	m_position(pos),
	m_playerControlled(playerControlled),
	m_hitPoints(1),		// set some minimum number of hitpoints so we don't die before somebody sets
	m_maxHitPoints(1),	// the proper amount on us.
	m_changedLevel(false)
{
	m_status = new hnStatus;
}

entBase::~entBase()
{
}

void
entBase::SetName( char * name )
{
	strncpy( m_name, name, MAX_NAME_BYTES );
	// make sure that we've got a null at least at the end of our name, if not earlier
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

	if ( dir >= DIR_North && dir <= DIR_NorthWest )
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
	else	// dir == DIR_Up || dir == DIR_Down -- we can't get here otherwise.  (hnGame::ClientMove() ignores other 'directions')
	{
		hnPoint currentPos = GetPosition();
		if ( dir == DIR_Up )
		{
			if ( hnDungeon::GetLevel(currentPos.z)->WallAt(currentPos.x,currentPos.y) & WALL_StairsUp )
			{
				if ( currentPos.z-1 >= 0 )	// if we're not leaving the dungeon...
				{
					hnPoint2D stairsPos = hnDungeon::GetLevel(currentPos.z-1)->GetDownStairs();
					if ( hnDungeon::GetLevel(currentPos.z-1)->MapTile(stairsPos.x, stairsPos.y).entity == NULL )
					{
						legalMove = true;       // nobody standing where we want to go.
					}
					else
						blocked = true;
				}
				else
				{
					// we're leaving the dungeon, so don't worry about looking for the stairs to
					// be clear on the other side.
					legalMove = true;
				}
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
	
	return legalMove;
}

void
entBase::Move( hnDirection dir )
{
	// final check for legality...
	if ( !IsValidMove(dir) )
		return;
	
	hnPoint pos = m_position + offsetVector[dir];
	
	mapBase *origMap = hnDungeon::GetLevel( m_position.z );
	mapBase *map = hnDungeon::GetLevel( pos.z );
	
	if ( dir == DIR_Up )
	{
		if ( pos.z >= 0 )
		{
			hnPoint2D stairsPos = map->GetDownStairs();
			pos.x = stairsPos.x;
			pos.y = stairsPos.y;
			m_changedLevel = true;
		}
	}
	else if ( dir == DIR_Down )
	{
		hnPoint2D stairsPos = map->GetUpStairs();
		pos.x = stairsPos.x;
		pos.y = stairsPos.y;
		m_changedLevel = true;
	}
	
	origMap->RemoveEntity(this);
	if ( map )
		map->PutEntityAt(this, pos.x, pos.y);
	
	m_position = pos;
}
