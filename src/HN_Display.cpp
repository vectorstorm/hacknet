#include <stdlib.h>
#include "HN_Display.h"
#include "HN_Map.h"
#include "HN_Consts.h"
#include "HN_Entity.h"

hnDisplay::hnDisplay()
{
	m_map = new hnMap(LEVEL_WIDTH, LEVEL_HEIGHT);
}

hnDisplay::~hnDisplay()
{
	delete m_map;
}

void
hnDisplay::UpdateMapTile(sint8 x, sint8 y, const hnMapTile &tile)
{
	m_map->MaterialAt(x,y) = tile.material;
	m_map->WallAt(x,y) = tile.wall;

	if ( m_map->MapTile(x,y).entity != NULL )
		delete m_map->MapTile(x,y).entity;
	
	m_map->MapTile(x,y).entity = tile.entity;
}

void
hnDisplay::UpdateMapEntity(sint8 x, sint8 y, hnObjectType type)
{
/*	hnObject *newEntity = NULL;
	hnPoint where(x,y,0);
	
	hnMapTile & tile = m_map->MapTile(x,y);

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
