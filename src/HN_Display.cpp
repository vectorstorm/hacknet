#include <stdlib.h>
#include <string.h>
#include "HN_Display.h"
#include "OBJ_Types.h"
#include "ENT_Types.h"

hnDisplay::hnDisplay(char * name):
	m_map(NULL),
	m_groupMemberCount(1),
	m_groupMemberTurnCount(0),
	m_submittedTurn(0)
{
	if ( name )
	{
		strncpy( m_name, name, MAX_NAME_BYTES );
		m_name[MAX_NAME_BYTES-1] = '\0';
	}
	else	// this branch is unnecessary, and will never be executed.  Still,
	{	// sanity checking is always a good idea!
		strncpy( m_name, "Unknown", MAX_NAME_BYTES );
		m_name[MAX_NAME_BYTES-1] = '\0';
	}
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
		if ( tile.material != MATERIAL_Unknown )
			m_map[point.z]->MaterialAt(point.x,point.y) = tile.material;
		if ( tile.wall != WALL_Unknown )
			m_map[point.z]->WallAt(point.x,point.y) = tile.wall;
		
		m_map[point.z]->MapTile(point.x,point.y).entity = tile.entity;
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
