#include <stdlib.h>
#include "HN_Display.h"
#include "OBJ_Types.h"
#include "ENT_Types.h"

hnDisplay::hnDisplay():
	m_map(NULL)
{
}

hnDisplay::~hnDisplay()
{
	delete m_map;
}

void
hnDisplay::UpdateMapTile(sint8 x, sint8 y, const mapClientTile &tile)
{
	m_map->MaterialAt(x,y) = tile.material;
	m_map->WallAt(x,y) = tile.wall;

	m_map->MapTile(x,y).entity = tile.entity;
}

void
hnDisplay::MapReset(sint8 width, sint8 height)
{
	delete m_map;
	m_map = new mapClient( width, height );
}

void
hnDisplay::UpdateMapCreature(sint8 x, sint8 y, entType type)
{
/*	hnEntity *newEntity = NULL;
	hnPoint where(x,y,0);
	
	mapClientTile & tile = m_map->MapTile(x,y);

	if ( tile.entity )
		delete tile.entity;
	
	switch ( type )
	{
		case OBJECT_None:
			break;
		case OBJECT_Player:
			newEntity = new hnEntity(type,where);
			break;
	}
	
	tile.entity = newEntity;*/
}
