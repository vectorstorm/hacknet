#include <stdlib.h>
#include <stdio.h>
#include "MAP_Client.h"
#include "HN_Point.h"

#define RND(x)  (int)(rand() % (long)(x))
#define min(x,y) ( (x>y)?y:x )
#define max(x,y) ( (x>y)?x:y )

mapClient::mapClient(uint8 width, uint8 height):
	m_width(width), 
	m_height(height), 
	m_roomCount(0),
	m_visited(false)
{
	m_tile = new mapClientTile[width * height];
	for ( int i = 0; i < width * height; i++ )
	{
		m_tile[i].material 	= 	MATERIAL_Unknown;
		m_tile[i].wall 		= 	WALL_Solid;
		m_tile[i].entity 	= 	ENTITY_None;
		m_tile[i].object	=	OBJECT_None;
	}
	
	m_backgroundType.material = MATERIAL_Dirt;
	m_backgroundType.wall = WALL_Any;

	ResetChanged();
	m_bottomRightMaxChanged.Set(0,0);
	m_topLeftMaxChanged.Set(width,height);
}

mapClient::~mapClient()
{
	delete [] m_tile;
}

hnMaterialType &
mapClient::MaterialAt(uint8 x, uint8 y)
{
	if ( x < m_width && y < m_height )
		return m_tile[x + (y * m_width)].material;

	return m_backgroundType.material;
}

hnWallType &
mapClient::WallAt(uint8 x, uint8 y)
{
	if ( x < m_width && y < m_height )
		return m_tile[x + (y * m_width)].wall;

	return m_backgroundType.wall;
}

mapClientTile &
mapClient::MapTile(uint8 x, uint8 y)
{
	if ( x < m_width && y < m_height )
		return m_tile[x + (y * m_width)];

	return m_backgroundType;
}

void
mapClient::RemoveObject( objType object )
{
	// no current way to do this.  :)  Oops!
}

void
mapClient::PutObjectAt( objType object, uint8 x, uint8 y )
{
	mapClientTile & tile = MapTile(x,y);
	tile.object = object;
}

void
mapClient::MoveObjectTo( objType object, uint8 x, uint8 y )
{
	RemoveObject(object);
	PutObjectAt(object,x,y);
}

void
mapClient::RemoveEntity( entType entity )
{
	/*  No current way to do this, either.
	const hnPoint & point = entity->GetPosition();
	mapClientTile & tile = MapTile(point.x, point.y);
	
	tile.entity = ENTITY_None;*/
}

void
mapClient::PutEntityAt( entType entity, uint8 x, uint8 y )
{
	mapClientTile & tile = MapTile(x,y);
	tile.entity = entity;
}

void
mapClient::MoveEntityTo( entType entity, uint8 x, uint8 y )
{
	RemoveEntity(entity);
	PutEntityAt(entity,x,y);
}


void
mapClient::Generate()
{
	for ( int i = 0; i < m_width * m_height; i++ )  //clear us to fully rock
	{
		m_tile[i].wall = WALL_Solid;
		m_tile[i].material = MATERIAL_Rock;
		m_tile[i].border = false;
	}
}



void
mapClient::MarkPointChanged( uint8 x, uint8 y )
{
	m_topLeftChanged.x = min( m_topLeftChanged.x, x );
	m_topLeftMaxChanged.x = min( m_topLeftMaxChanged.x, x );
	m_topLeftChanged.y = min( m_topLeftChanged.y, y );
	m_topLeftMaxChanged.y = min( m_topLeftMaxChanged.y, y );

	m_bottomRightChanged.x = max( m_bottomRightChanged.x, x );
	m_bottomRightMaxChanged.x = max( m_bottomRightMaxChanged.x, x );
	m_bottomRightChanged.y = max( m_bottomRightChanged.y, y );
	m_bottomRightMaxChanged.y = max( m_bottomRightMaxChanged.y, y );
}

void
mapClient::ResetChanged()
{
	m_bottomRightChanged.Set(0,0);
	m_topLeftChanged.Set(m_width,m_height);
}

mapClientTile::mapClientTile()
{
	object = OBJECT_None;
	entity = ENTITY_None;
}

mapClientTile::~mapClientTile()
{
}
