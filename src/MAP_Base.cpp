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
#define max(x,y) ( (x<y)?y:x )

mapBase::mapBase(uint8 width, uint8 height, uint8 depth):
	m_width(width), 
	m_height(height),
	m_depth(depth),
	m_roomCount(0)
{
	m_stairsUp.Set(-1,-1);
	m_stairsDown.Set(-1,-1);

	ResetChanged();
	
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
mapBase::MaterialAt(uint8 x, uint8 y)
{
	if ( x < m_width && y < m_height ) // x and y cannot be less than zero, since they're unsigned
		return m_tile[x + (y * m_width)].material;

	return m_backgroundType.material;
}

hnWallType &
mapBase::WallAt(uint8 x, uint8 y)
{
	if ( x < m_width && y < m_height )
		return m_tile[x + (y * m_width)].wall;

	return m_backgroundType.wall;
}

mapTile &
mapBase::MapTile(uint8 x, uint8 y)
{
	if ( x < m_width && y < m_height )
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
mapBase::PutObjectAt( objBase *object, uint8 x, uint8 y )
{
	mapTile & tile = MapTile(x,y);
	tile.object->AddObject( object );
}

void
mapBase::MoveObjectTo( objBase *object, uint8 x, uint8 y )
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
mapBase::PutEntityAt( entBase *entity, uint8 x, uint8 y )
{
	mapTile & tile = MapTile(x,y);
	tile.entity = entity;
}

void
mapBase::MoveEntityTo( entBase *entity, uint8 x, uint8 y )
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
mapBase::ResetChanged()
{
	m_bottomRightChanged.Set(0,0);
	m_topLeftChanged.Set(m_width,m_height);
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
mapBase::UpdateVisibility( const hnPoint & position, mapBase * sourceMap )
{
	//-------------------------------------------------------------
	//
	//  Reset visibility of last frame, since we're about to
	//  recalculate it.
	//
	//-------------------------------------------------------------

	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
			MapTile(i,j).visible = false;
	
	//-------------------------------------------------------------
	//
	//  Rogue-style LOS.. yes, it's not correct, but it's fast.
	//  Real LOS is yet-to-be-implemented.
	//
	//-------------------------------------------------------------
	
	hnPoint pos = position;
	
	if ( sourceMap->WallAt( pos.x, pos.y ) & WALL_WithinRoom )
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
					MapTile(x,y).visible 	= 	true;
		
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

		for ( int j = -1; j <= 1; j++ )
			for ( int i = -1; i <= 1; i++ )
			{
				if ( !(sourceMap->WallAt(pos.x+i,pos.y+j) & WALL_Any) )
					MapTile(pos.x+i,pos.y+j).visible = true;
			}
	}
}


void
mapBase::UpdateMap( mapBase * sourceMap )
{
	for ( uint8 y = 0; y < m_height; y++ )
	{
		for ( uint8 x = 0; x < m_width; x++ )
		{
			mapTile *myTile 	= 	&MapTile(x,y);
			
			if ( myTile->visible )
			{
				mapTile *realTile 	= 	&sourceMap->MapTile(x,y);
				bool changed 		= 	false;
			
				if ( myTile->material != realTile->material )
				{
					myTile->material = realTile->material;
					changed = true;
				}
				if ( myTile->wall != realTile->wall )
				{
					myTile->wall = realTile->wall;
					changed = true;
				}
				entType type = (realTile->entity) ? (realTile->entity->GetType()) : ENTITY_None;
				
				if ( myTile->entityType != type )
				{
					myTile->entityType = type;
					changed = true;
				}
					
				if ( changed )
					MarkPointChanged( x, y );
			}
			else
			{
				if ( myTile->entityType )
				{
					myTile->entityType = ENTITY_None;
					MarkPointChanged( x, y );
				}
			}
		}
	}
}

void
mapBase::MarkPointChanged( uint8 x, uint8 y )
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

//---------------------------------

mapTile::mapTile()
{
	object = new objBase( OBJECT_None, hnPoint(0,0,0));
	entity = NULL;
	entityType = ENTITY_None;
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
