#include <stdlib.h>
#include "HN_Display.h"
#include "MAP_Base.h"
#include "HN_Consts.h"
#include "HN_Entity.h"
#include "HN_Creature.h"

hnDisplay::hnDisplay()
{
	m_map = new mapBase(LEVEL_WIDTH, LEVEL_HEIGHT);
}

hnDisplay::~hnDisplay()
{
	delete m_map;
}

void
hnDisplay::UpdateMapTile(sint8 x, sint8 y, const mapTile &tile)
{
	m_map->MaterialAt(x,y) = tile.material;
	m_map->WallAt(x,y) = tile.wall;

	if ( m_map->MapTile(x,y).entity != NULL )
		delete m_map->MapTile(x,y).entity;
	
	m_map->MapTile(x,y).entity = tile.entity;
}

void
hnDisplay::UpdateMapCreature(sint8 x, sint8 y, hnEntityType type)
{
/*	hnEntity *newEntity = NULL;
	hnPoint where(x,y,0);
	
	mapTile & tile = m_map->MapTile(x,y);

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
