#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HN_Display.h"
#include "NET_Client.h"
#include "OBJ_Registry.h"
#include "OBJ_Types.h"
#include "ENT_Types.h"

hnDisplay::hnDisplay(char * name):
	m_map(NULL),
	m_inventoryCount(0),
	m_wieldedItem(-1),
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
	
	for ( int i = 0; i < INVENTORY_MAX; i++ )
		m_inventory[i].count = 0;
	
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
hnDisplay::SetObjectStats(uint16 objectCount)
{
	objRegistry::Startup(objectCount);
}

void
hnDisplay::SetObjectName(uint16 objectID, objType type, char *name)
{
	objRegistry::GetInstance()->SetName(objectID, name);
	objRegistry::GetInstance()->SetType(objectID, type);
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
		DisplayItems();
	}
}

void
hnDisplay::DisplayItems()
{
	if ( m_map[m_position.z] )
	{
		int objCount = m_map[m_position.z]->MapTile(m_position.x, m_position.y).objectCount;
		
		if ( objCount > 0 )
		{
			char buffer[256];
		
			objDescription topObject = m_map[m_position.z]->MapTile(m_position.x, m_position.y).object[0];

			if ( objCount > 1 )
				snprintf(buffer, 256, "There are several objects here.");
			else
			{
				char objectDesc[256];
				objRegistry::GetInstance()->GetObjectDescriptionText(topObject,objectDesc,256);
				snprintf(buffer, 256, "You see here %s.", objectDesc );
			}
			TextMessage(buffer);

			Refresh();
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
	m_inventoryCount = objectCount;

	for ( int i = 0; i < INVENTORY_MAX; i++ )
	{
		if ( i < objectCount )
			m_inventory[i] = objectArray[i];
		else
			m_inventory[i].count = 0;
	}
}

void
hnDisplay::TakenItem( const objDescription &desc, int inventoryID )
{
        const char inventoryLetters[56] =
        {
                'a','b','c','d','e','f','g','h','i','j','k','l','m',
                'n','o','p','q','r','s','t','u','v','w','x','y','z',
                'A','B','C','D','E','F','G','H','I','J','K','L','M',
                'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
        };

	char buffer[256];
	char nameBuffer[128];
	objRegistry::GetInstance()->GetObjectDescriptionText(desc, nameBuffer, 128);
	snprintf(buffer,256,"%c - %s.", inventoryLetters[inventoryID], nameBuffer);
	TextMessage(buffer);
}

void
hnDisplay::DroppedItem( const objDescription &desc )
{
	char buffer[256];
	char nameBuffer[128];
	objRegistry::GetInstance()->GetObjectDescriptionText(desc, nameBuffer, 128);
	snprintf(buffer,256,"You drop %s.", nameBuffer);
	TextMessage(buffer);
}

void
hnDisplay::WieldedItem( const objDescription &desc, int inventoryID )
{
        const char inventoryLetters[56] =
        {
                'a','b','c','d','e','f','g','h','i','j','k','l','m',
                'n','o','p','q','r','s','t','u','v','w','x','y','z',
                'A','B','C','D','E','F','G','H','I','J','K','L','M',
                'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
        };


	char buffer[256];
	char nameBuffer[128];
	objRegistry::GetInstance()->GetObjectDescriptionText(desc, nameBuffer, 128);
	snprintf(buffer,256,"%c - %s (weapon in hands).", inventoryLetters[inventoryID], nameBuffer);
	TextMessage(buffer);

	m_wieldedItem = inventoryID;
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

	PostTurnSubmit();
}

void
hnDisplay::DropCommand( uint8 inventorySlot )
{
	m_client->SendDrop(&m_inventory[inventorySlot], inventorySlot);
	
	PostTurnSubmit();
}

void
hnDisplay::WieldCommand( uint8 inventorySlot )
{
	m_client->SendWield(&m_inventory[inventorySlot], inventorySlot);

	PostTurnSubmit();
}
