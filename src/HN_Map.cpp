#include <stdlib.h>
#include "HN_Map.h"
#include "HN_Object.h"
#include "HN_Point.h"

#include <stdio.h>

#define RND(x)  (int)(rand() % (long)(x))
#define min(x,y) ( (x>y)?y:x )

hnMap::hnMap(sint8 width, sint8 height):
	m_width(width), m_height(height), m_roomCount(0)
{
	m_tile = new hnMapTile[width * height];
	for ( int i = 0; i < width * height; i++ )
	{
		m_tile[i].material = MATERIAL_Unknown;
		m_tile[i].wall = WALL_Solid;
	}
	
	m_backgroundType.material = MATERIAL_Dirt;
	m_backgroundType.wall = WALL_Any;

	for ( int i = 0; i < MAX_ROOMS; i++ )
		m_room[i] = NULL;
}

hnMap::~hnMap()
{
	for ( int i = 0; i < m_roomCount; i++ )
		delete m_room[i];

	delete [] m_tile;
}

hnMaterialType &
hnMap::MaterialAt(sint8 x, sint8 y)
{
	if ( x < m_width && y < m_height && x >= 0 && y >= 0)
		return m_tile[x + (y * m_width)].material;

	return m_backgroundType.material;
}

hnWallType &
hnMap::WallAt(sint8 x, sint8 y)
{
	if ( x < m_width && y < m_height && x >= 0 && y >= 0)
		return m_tile[x + (y * m_width)].wall;

	return m_backgroundType.wall;
}

hnMapTile &
hnMap::MapTile(sint8 x, sint8 y)
{
	if ( x < m_width && y < m_height && x >= 0 && y >= 0)
		return m_tile[x + (y * m_width)];

	return m_backgroundType;
}

void
hnMap::RemoveObject( hnObject *object )
{
	hnObjectType type = object->GetType();
	const hnPoint & point = object->GetPosition();
	hnMapTile & tile = MapTile(point.x, point.y);
	
	if ( type == OBJECT_Player )
		tile.entity = NULL;	
	else
		tile.object->RemoveObject( object );
}

void
hnMap::PutObjectAt( hnObject *object, sint8 x, sint8 y )
{
	hnObjectType type = object->GetType();
	hnMapTile & tile = MapTile(x,y);
	
	if ( type == OBJECT_Player )
		tile.entity = object;
	else
		tile.object->AddObject( object );
}

void
hnMap::MoveObjectTo( hnObject *object, sint8 x, sint8 y )
{
	RemoveObject(object);
	PutObjectAt(object,x,y);
}

void
hnMap::Generate()
{
	for ( int i = 0; i < m_width * m_height; i++ )  //clear us to fully rock
	{
		m_tile[i].wall = WALL_Solid;
		m_tile[i].material = MATERIAL_Rock;
		m_tile[i].border = false;
	}
}


hnMapTile::hnMapTile()
{
	object = new hnObject( OBJECT_None, hnPoint(0,0,0));
	entity = NULL;
}

hnMapTile::~hnMapTile()
{
	delete object;
}
