#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "MAP_Base.h"
#include "MAP_Client.h"
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

	m_cells = new mapCell[max(width,height)];
}

mapBase::~mapBase()
{
	for ( int i = 0; i < m_roomCount; i++ )
		delete m_room[i];

	delete [] m_tile;
	delete [] m_cells;
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

bool
mapBase::OnMap(uint8 x, uint8 y)
{
	if ( x < m_width && y < m_height )
		return true;

	return false;
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
mapBase::CalculateVisibility( const hnPoint & position, mapClient * destMap )
{
	//-------------------------------------------------------------
	//
	//  Reset dest's visibility of last frame, since we're about to
	//  recalculate it.
	//
	//-------------------------------------------------------------

	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
			destMap->MapTile(i,j).visible = false;
	
	//-------------------------------------------------------------
	//
	//  Set 'lit' property on all map tiles, based upon player
	//  location and suchlike.  (TODO:  Implement movable light
	//  sources)
	//
	//-------------------------------------------------------------
	
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
		{
			MapTile(i,j).lit = false;
			
			if ( MapTile(i,j).permalit )
				MapTile(i,j).lit = true;
			else
				if ( (fabs(i-position.x ) <=1) && (fabs(j-position.y) <= 1) )
					MapTile(i,j).lit = true;
		}
	
	hnPoint pos = position;

	CalculateOctantVisibility(pos, hnPoint(1,1), false, m_width, destMap);
	CalculateOctantVisibility(pos, hnPoint(1,-1), false, m_width, destMap);
	CalculateOctantVisibility(pos, hnPoint(-1,1), false, m_width, destMap);
	CalculateOctantVisibility(pos, hnPoint(-1,-1), false, m_width, destMap);
	
	CalculateOctantVisibility(pos, hnPoint(1,1), true, m_width, destMap);
	CalculateOctantVisibility(pos, hnPoint(1,-1), true, m_width, destMap);
	CalculateOctantVisibility(pos, hnPoint(-1,1), true, m_width, destMap);
	CalculateOctantVisibility(pos, hnPoint(-1,-1), true, m_width, destMap);
	
	destMap->MapTile(pos.x,pos.y).visible = true;
	
}


/* 
void
mapBase::CalculateVisibility( const hnPoint & position, mapClient * destMap )
{
	//-------------------------------------------------------------
	//
	//  Reset dest's visibility of last frame, since we're about to
	//  recalculate it.
	//
	//-------------------------------------------------------------

	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
			destMap->MapTile(i,j).visible = false;
	
	//-------------------------------------------------------------
	//
	//  Rogue-style LOS.. yes, it's not correct, but it's fast.
	//  Real LOS is yet-to-be-implemented.
	//
	//-------------------------------------------------------------
	
	hnPoint pos = position;
	
	if ( WallAt( pos.x, pos.y ) & WALL_WithinRoom )
	{
		//---------------------------------------------------------------
		//  Figure out what room we're in, then update visibility for the
		//  whole room
		//---------------------------------------------------------------
		mapRoom *where = NULL;
		
		for ( int i = 0; i < m_roomCount; i++ )
		{
			mapRoom *room = m_room[i];
			
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
					destMap->MapTile(x,y).visible 	= 	true;
		
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
				if ( !(WallAt(pos.x+i,pos.y+j) & WALL_Any) )
					destMap->MapTile(pos.x+i,pos.y+j).visible = true;
			}
	}
}
*/

void
mapBase::UpdateMap( mapClient * destMap )
{
	//-------------------------------------------------------------------
	//  Function for vision maps.  Passing in the map to use as the
	//  'source', for each tile in 'this' map which is set visible, it
	//  copies the contents of the 'source' map tile, and sets the
	//  'changed' flag on the tile, if that tile has changed.  This also
	//  updates the bounding box of changed tiles, which is eventually
	//  used to determine what data needs to be transmitted to the client
	//  in the next update packet.
	//-------------------------------------------------------------------
	for ( uint8 y = 0; y < m_height; y++ )
	{
		for ( uint8 x = 0; x < m_width; x++ )
		{
			mapClientTile *myTile 	= 	&destMap->MapTile(x,y);
			
			if ( myTile->visible && MapTile(x,y).lit )
			{
				mapTile *realTile 	= 	&MapTile(x,y);
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
				
				if ( myTile->entity != type )
				{
					myTile->entity = type;
					changed = true;
				}
				
				bool reconstructDesc = false;
				int realObjectCount = realTile->object->ObjectCount();
				
				if ( realObjectCount != myTile->objectCount )
				{
					reconstructDesc = true;
				}
				else
				{
					for ( int i = 0; i < myTile->objectCount; i++ )
					{
						objBase *object = realTile->object->GetObject(i);
						if ( !object->ExactMatch( myTile->object[i] ) )
						{
							reconstructDesc = true;
						}
					}
				}
				if ( reconstructDesc )
				{
					// delete our current descriptions.
					delete [] myTile->object;
					
					myTile->object = new objDescription[ realObjectCount ];

					for ( int i = 0; i < realObjectCount; i++ )
					{
						realTile->object->GetDescription( myTile->object[i], i );
					}

					myTile->objectCount = realObjectCount;
					
					changed = true;
				}
				
				if ( changed )
					destMap->MarkPointChanged( x, y );
			}
			else
			{
				// uncomment the following two lines to send full visibility information
				// each turn, marking unseen things as unknown, to test visibility algorithms.
				
					//myTile->wall = WALL_Unknown;
					//destMap->MarkPointChanged( x, y );
				if ( myTile->entity )
				{
					myTile->entity = ENTITY_None;
					destMap->MarkPointChanged( x, y );
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
	object = new objBase( 0 );
	entity = NULL;
	visionBlocked = false;
	permalit = false;
	lit = false;
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

bool
mapTile::isLitForMe(hnPoint position)
{
	return true;
}


//  Line of Sight Implementation

void
mapBound::reset()
{
	max = 0.0f;
	count = 0.0f;
}

bool
mapBound::inUse() const
{
	return ( max != 0.0f );
}

bool
mapBound::reached() const
{
	return ( inUse() && count > max );
}

bool
mapBound::steeper(float inverseGradient) const 
{
	return ( !inUse() || inverseGradient < max );
}

bool
mapBound::shallower(float inverseGradient) const
{
	return ( inverseGradient > max );
}

void
mapCell::init()
{
	lit = true;		// In shadowcasting, everything starts lit.
	upperBound.reset();
	lowerBound.reset();
}


void
mapBase::CalculateOctantVisibility( const hnPoint &position, const hnPoint &offset, bool swap, unsigned int N, mapClient *destMap )
{
	bool blocker;
	int below = 0;
	bool belowLeftLit = false;
	bool somethingWasLit = false;
	
	int boundsCounter = 0;
	int cell;
	int col;

	// Cell 0,0 is assumed to be lit.
	
	m_cells[0].init();

	// For each column...
	for (col = 1; col < N; ++col)
	{
		somethingWasLit = false;
		belowLeftLit = false;

		// For each cell in the column, such that we iterate
		// over an octant...
		for ( cell = 0; cell <= col; ++cell )
		{
			below = cell - 1;
			if ( cell == col )
			{
				m_cells[cell].init();
			}
			
			bool onMap;
			mapTile *t;
			mapClientTile *dt;
			if ( swap )
			{
				t = &MapTile( position.x + offset.x * cell, position.y + offset.y * col);
				dt = &destMap->MapTile( position.x + offset.x * cell, position.y + offset.y * col);
				onMap = OnMap( position.x + offset.x * cell, position.y + offset.y * col);
			}
			else
			{
				t = &MapTile( position.x + offset.x * col, position.y + offset.y * cell);
				dt = &destMap->MapTile( position.x + offset.x * col, position.y + offset.y * cell);
				onMap = OnMap( position.x + offset.x * col, position.y + offset.y * cell);
			}
			
			blocker = ( t->visionBlocked );

			if (blocker)
			{
				// By default, blockers are not visible
				m_cells[cell].visibleBlocker = false;

				// If cell below is lit or cell left is lit or cell diagonally below and left is lit...
				if ( (below >= 0 && m_cells[below].lit) || m_cells[cell].lit || belowLeftLit )
				{
					// This blocker is visible.

					// belowLeftLit is set to true if the cell
					// to the left of the current cell is lit --
					// this will be used during the next iteration,
					// where it will refer to the cell to the bottom left
					// of the new cell under consideration.

					belowLeftLit = m_cells[cell].lit;

					// We're visible...
					m_cells[cell].visibleBlocker = true;

					// But unlit, so that we propagate shadows
					m_cells[cell].lit = false;
					
					// Create a new shadow!

					// Calculate appropriate upper/lower inverse gradients
					// (run/rise)

					// Calculate upper inverse gradient from top-left of
					// this cell to the centre of the source (0.5,0.5).
					float upper = (float)(col - 0.5f)/(float)(cell + 0.5f);

					// If the gradient for the top is steeper than our
					// current one, set up a shadow.
					if ( m_cells[cell].upperBound.steeper(upper) )
					{
						// new upper shadow
						m_cells[cell].upperBound.max = upper;
						m_cells[cell].upperBound.count = 0;
						boundsCounter++;
					}

					// Calculate lower inverse gradient from bottom-right
					// of this cell to the centre of the source.
					if ( cell > 0 )
					{
						float lower = (float)(col + 0.5f)/(float)(cell - 0.5f);

						// If the gradient for the bottom is shallower
						// than our current one, set up a shadow.
						if ( m_cells[cell].lowerBound.shallower(lower))
						{
							// New lower shadow.
							m_cells[cell].lowerBound.max = lower;
							m_cells[cell].lowerBound.count = 0;
							boundsCounter++;
						}
					}
				}
			}

			if (below >= 0)
			{
				// Grow shadow bounds.

				m_cells[cell].upperBound.count += 1.0f;
				m_cells[cell].lowerBound.count += 1.0f;

				// Grow lower bound.
				if ( m_cells[below].lowerBound.reached() )
				{
					// light is moving up from below
					// so move below's lower shadow bound to this cell.
					m_cells[below].lit = true;
					m_cells[cell].visibleBlocker = true;
					m_cells[cell].lowerBound.max = m_cells[below].lowerBound.max;
					m_cells[cell].lowerBound.count = m_cells[below].lowerBound.count - m_cells[below].lowerBound.max;
					m_cells[below].lowerBound.max = 0;
					m_cells[below].lowerBound.count = 0;
				}

				// Grow upper bound
				if ( m_cells[below].upperBound.reached() )
				{
					// a shadow is moving up from below.
					// move below's upper shadow bound to this cell.
					m_cells[cell].lit = false;
					m_cells[cell].visibleBlocker = false;
					m_cells[cell].upperBound.max = m_cells[below].upperBound.max;
					m_cells[cell].upperBound.count = m_cells[below].upperBound.count - m_cells[below].upperBound.max;
					m_cells[below].upperBound.max = 0;
					m_cells[below].upperBound.count = 0;
				}

				// Remove superfluous shadow boundaries.

				// Remove lower bound if possible.
				if (!m_cells[below].lit)	// if the cell below isn't lit
				{
					// Remove our shadow
					if (m_cells[cell].lowerBound.inUse() )
						boundsCounter--;
					m_cells[cell].lowerBound.reset();
				}

				// Remove upper bound if possible.
				if (!m_cells[cell].lit)  	// if this cell is unlit
				{
					// Remove below's shadow.
					if (m_cells[below].upperBound.inUse())
						boundsCounter--;
					m_cells[below].upperBound.reset();
				}
			} // end if (below >= 0)

			if ( onMap )
			{
			// Apply 'lit' value.
			
				if (m_cells[cell].lit || (blocker && m_cells[cell].visibleBlocker))
				{
					somethingWasLit = true;
					dt->visible = t->isLitForMe( position );
				}
			}
		}

		if ( !somethingWasLit )
		{
			return;		
		}
	}
}
