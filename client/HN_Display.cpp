#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HN_Display.h"
#include "NET_Client.h"
#include "OBJ_Types.h"
#include "ENT_Types.h"

hnDisplay::hnDisplay(char * name):
	m_map(NULL),
	m_inventory(NULL),
	m_inventoryCount(0),
	m_groupMemberCount(1),
	m_groupMemberTurnCount(0),
	m_submittedTurn(false),
	m_moved(false)
{
	if ( name )
	{
		strncpy( m_name, name, MAX_NAME_BYTES );
		m_name[MAX_NAME_BYTES-1] = '\0';
	}
	else	// this branch is unnecessary, and should never be executed.  Still,
	{	// sanity checking is always a good idea!
		strncpy( m_name, "Unknown", MAX_NAME_BYTES );
		m_name[MAX_NAME_BYTES-1] = '\0';
	}

	m_status = new hnStatus;
}

hnDisplay::~hnDisplay()
{
	if ( m_map )
	{
		for ( int i = 0; i < m_levelCount; i++ )
			delete m_map[i];
		delete [] m_map;
	}
}

void
hnDisplay::UpdateMapTile(const hnPoint &point, const mapClientTile &tile)
{
	if ( m_map[point.z] != NULL )
	{
		mapClientTile *myTile = &m_map[point.z]->MapTile(point.x,point.y);
		
		if ( tile.material != MATERIAL_Unknown )
			m_map[point.z]->MaterialAt(point.x,point.y) = tile.material;
		if ( tile.wall != WALL_Unknown )
			m_map[point.z]->WallAt(point.x,point.y) = tile.wall;
		
		myTile->entity = tile.entity;

		delete [] myTile->object;
		myTile->object = new objDescription[tile.objectCount];

		for ( int i = 0; i < tile.objectCount; i++ )
			myTile->object[i] = tile.object[i];

		myTile->objectCount = tile.objectCount;
	}
}

void
hnDisplay::DungeonReset( sint8 levelCount )
{
	if ( m_map )
	{
		for ( int i = 0; i < m_levelCount; i++ )
			delete m_map[i];
		delete [] m_map;
	}
	
	m_levelCount = levelCount;
	m_map = new (mapClient *)[m_levelCount];
	
	for ( int i = 0; i < m_levelCount; i++ )
		m_map[i] = NULL;
}

void
hnDisplay::MapReset(sint8 width, sint8 height, sint8 depth)
{
	if ( m_map[depth] )
		delete m_map[depth];
	m_map[depth] = new mapClient( width, height );
}

void
hnDisplay::UpdateLocation( const hnPoint &point )
{
	if ( m_position != point )
	{
		m_moved = true;
		m_position = point;
	}
}

void
hnDisplay::Refresh()
{
	if ( m_moved )
	{
		m_moved = false;
		// check to see if there's an object under us.  If so,
		// print a message about it.

		if ( m_map[m_position.z] )
		{
			int objCount = m_map[m_position.z]->MapTile(m_position.x, m_position.y).objectCount;
			
			if ( objCount > 0 )
			{
				// hardcode to long sword, since that's all we have right now.
				char buffer[256];
			
				objDescription topObject = m_map[m_position.z]->MapTile(m_position.x, m_position.y).object[0];

				if ( objCount > 1 )
					snprintf(buffer, 256, "There are several objects here.");
				else
					snprintf(buffer, 256, "You see here a %s.", GetObjectName(topObject.type) );

				TextMessage(buffer);

				Refresh();
			}
		}
	}
}

void
hnDisplay::UpdateMapCreature(const hnPoint &point, entType type)
{
	// rolled into UpdateMapTile.
}

void
hnDisplay::UpdateGroupData( int groupMemberCount, int groupMemberTurnCount, bool submittedTurn )
{
	m_groupMemberCount = groupMemberCount;
	m_groupMemberTurnCount = groupMemberTurnCount;
	m_submittedTurn = submittedTurn;
}

void
hnDisplay::UpdateInventory( int objectCount, objDescription *objectArray )
{
	delete [] m_inventory;

	m_inventoryCount = objectCount;
	m_inventory = new objDescription[ objectCount ];

	for ( int i = 0; i < m_inventoryCount; i++ )
		m_inventory[i] = objectArray[i];
}

void
hnDisplay::MoveCommand( hnDirection dir )
{
	bool attackInstead = false;
	
	if ( dir != DIR_Up && dir != DIR_Down )
	{
		// check the square we're going to move into...
		hnPoint proposedSquare = m_position;
		proposedSquare.Increment(dir);
		
		mapClient *map = m_map[proposedSquare.z];
		
		if ( map && map->MapTile(proposedSquare.x,proposedSquare.y).entity != ENTITY_None )
			attackInstead = true;
	}
	if ( attackInstead )
		m_client->SendAttack(dir);
	else
		m_client->SendMove(dir);	

	PostTurnSubmit();
}

void
hnDisplay::WaitCommand()
{
	m_client->SendWait();

	PostTurnSubmit();
}

void
hnDisplay::TakeCommand( objDescription *desc, uint8 stackID )
{
	m_client->SendTake(desc,stackID);
}
