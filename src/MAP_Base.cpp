#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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
	m_stairsUp.Set(-1,-1);
	m_stairsDown.Set(-1,-1);
	
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

void
mapBase::PrepareVisibility()
{
	for ( int j = 0; j < m_height; j++ )
		for ( int i = 0; i < m_width; i++ )
		{
			m_tile[i+(j*m_width)].UpdateVisibility();
		}
}

void
mapBase::UpdateVisibility( const hnPoint & position, mapBase *sourceMap )
{
	int range = 17;
	
	// Reset our visibility bounding box
	m_bottomRightVisibility.Set(0,0);
	m_topLeftVisibility.Set(m_width,m_height);
	
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
		{
			MapTile(i,j).visible = false;
		}
		
	
	
	//-------------------------------------------------------------
	//
	//  Rogue-style LOS.. yes, it's not correct, but it's fast.
	//  Real LOS is yet-to-be-implemented.
	//
	//-------------------------------------------------------------
	
	hnPoint pos = position;
	
	if ( WallAt( pos.x, pos.y ) == WALL_Room )
	{
		//---------------------------------------------------------------
		//  Figure out what room we're in, then update visibility for the
		//  whole room
		//---------------------------------------------------------------
		mapRoom *where = NULL;
		
		for ( int i = 0; i < sourceMap->m_roomCount; i++ )
		{
			mapRoom *room = sourceMap->m_room[i];
			
			if ( room )
			{
				if ( (room->left <= pos.x) && (room->right >= pos.x) &&
					(room->top <= pos.y) && (room->bottom >= pos.y) )
					{
						where = room;
						break;
					}
			}
		}

		if ( where )
		{
			for ( int y = where->top-1; y <= where->bottom+1; y++ )
				for ( int x = where->left-1; x <= where->right+1; x++ )
				{
					MapTile(x,y).visible = true;
					MaterialAt( x, y ) = sourceMap->MaterialAt( x, y );
					WallAt( x, y ) = sourceMap->WallAt( x, y );
					if ( sourceMap->MapTile( x, y ).entity == NULL )
						MapTile( x, y ).entityType = ENTITY_None;
					else
						MapTile( x, y ).entityType = sourceMap->MapTile( x, y ).entity->GetType();
	
					if ( x < m_topLeftVisibility.x )
						m_topLeftVisibility.x = x;
					if ( x > m_bottomRightVisibility.x )
						m_bottomRightVisibility.x = x;
					if ( y < m_topLeftVisibility.y )
						m_topLeftVisibility.y = y;
					if ( y > m_bottomRightVisibility.y )
						m_bottomRightVisibility.y = y;
				}
		
		}
		else
		{
			printf("Couldn't figure out what room at (%d,%d)\n", pos.x, pos.y);
			assert(0);
		}
	}
	else
	{
	        //---------------------------------------------------------------
	        //  Update visibility here.  In a corridor, just update the 3x3 box
	        //  around the player.
	        //---------------------------------------------------------------
		pos.x--;
		pos.y--;

		for ( int j = 0; j < 3; j++ )
			for ( int i = 0; i < 3; i++ )
			{
				int x = pos.x+i;
				int y = pos.y+j;
				
				MapTile(x,y).visible = true;
				MaterialAt( x, y ) = sourceMap->MaterialAt( x, y );
				WallAt( x, y ) = sourceMap->WallAt( x, y );
				if ( sourceMap->MapTile( x, y ).entity == NULL )
					MapTile( x, y ).entityType = ENTITY_None;
				else
					MapTile( x, y ).entityType = sourceMap->MapTile( x, y ).entity->GetType();

				if ( x < m_topLeftVisibility.x )
					m_topLeftVisibility.x = x;
				if ( x > m_bottomRightVisibility.x )
					m_bottomRightVisibility.x = x;
				if ( y < m_topLeftVisibility.y )
					m_topLeftVisibility.y = y;
				if ( y > m_bottomRightVisibility.y )
					m_bottomRightVisibility.y = y;
			}
	}
}


//---------------------------------

mapTile::mapTile()
{
	object = new objBase( OBJECT_None, hnPoint(0,0,0));
	entity = NULL;
	visionBlocked = false;
}

mapTile::~mapTile()
{
	delete object;
}

void
mapTile::UpdateVisibility()
{
	visionBlocked = !(wall & WALL_Passable);	// if we can't walk through it, we can't see through it.
}
