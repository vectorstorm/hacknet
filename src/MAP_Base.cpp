#include <stdlib.h>
#include <stdio.h>
#include "MAP_Base.h"
#include "OBJ_Base.h"
#include "ENT_Base.h"
//#include "HN_Object.h"
#include "HN_Point.h"


#define RND(x)  (int)(rand() % (long)(x))
#define min(x,y) ( (x>y)?y:x )

mapBase::mapBase(sint8 width, sint8 height):
	m_width(width), m_height(height), m_roomCount(0)
{
	m_tile = new mapTile[width * height];
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

mapBase::~mapBase()
{
	for ( int i = 0; i < m_roomCount; i++ )
		delete m_room[i];

	delete [] m_tile;
}

hnMaterialType &
mapBase::MaterialAt(sint8 x, sint8 y)
{
	if ( x < m_width && y < m_height && x >= 0 && y >= 0)
		return m_tile[x + (y * m_width)].material;

	return m_backgroundType.material;
}

hnWallType &
mapBase::WallAt(sint8 x, sint8 y)
{
	if ( x < m_width && y < m_height && x >= 0 && y >= 0)
		return m_tile[x + (y * m_width)].wall;

	return m_backgroundType.wall;
}

mapTile &
mapBase::MapTile(sint8 x, sint8 y)
{
	if ( x < m_width && y < m_height && x >= 0 && y >= 0)
		return m_tile[x + (y * m_width)];

	return m_backgroundType;
}

void
mapBase::RemoveObject( objBase *object )
{
	const hnPoint & point = object->GetPosition();
	mapTile & tile = MapTile(point.x, point.y);
	
	tile.object->RemoveObject( object );
}

void
mapBase::PutObjectAt( objBase *object, sint8 x, sint8 y )
{
	mapTile & tile = MapTile(x,y);
	tile.object->AddObject( object );
}

void
mapBase::MoveObjectTo( objBase *object, sint8 x, sint8 y )
{
	RemoveObject(object);
	PutObjectAt(object,x,y);
}

void
mapBase::RemoveEntity( entBase *entity )
{
	const hnPoint & point = entity->GetPosition();
	mapTile & tile = MapTile(point.x, point.y);
	
	tile.entity = NULL;
}

void
mapBase::PutEntityAt( entBase *entity, sint8 x, sint8 y )
{
	mapTile & tile = MapTile(x,y);
	tile.entity = entity;
}

void
mapBase::MoveEntityTo( entBase *entity, sint8 x, sint8 y )
{
	RemoveEntity(entity);
	PutEntityAt(entity,x,y);
}


void
mapBase::Generate()
{
	for ( int i = 0; i < m_width * m_height; i++ )  //clear us to fully rock
	{
		m_tile[i].wall = WALL_Solid;
		m_tile[i].material = MATERIAL_Rock;
		m_tile[i].border = false;
	}
}


mapTile::mapTile()
{
	object = new objBase( OBJECT_None, hnPoint(0,0,0));
	entity = NULL;
}

mapTile::~mapTile()
{
	delete object;
}
