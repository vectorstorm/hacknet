#include <stdlib.h>
#include "HN_Display.h"
#include "HN_Consts.h"
#include "OBJ_Types.h"
#include "ENT_Types.h"

hnDisplay::hnDisplay()
{
	m_map = new mapClient(LEVEL_WIDTH, LEVEL_HEIGHT);
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
