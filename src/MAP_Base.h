#ifndef __HN_MAP_H__
#define __HN_MAP_H__

#include "HN_Enum.h"
#include "HN_Types.h"
#include "ENT_Types.h"
#include "HN_Point.h"

#define MAX_ROOMS (8)

class objBase;
class entBase;

class mapTile
{
public:
				mapTile();
	virtual 		~mapTile();

	void			UpdateVisibility();
	
	objBase *		object;			// linked list of objects lying on the floor here.
	entBase *		entity;			// if a creature/player is standing here, this is a pointer to that creature.
	entType			entityType;		// for use if we aren't the authoritative map and so don't want to store the actual pointer.
	
	hnMaterialType		material;		// what type of material makes up this tile?
	hnWallType		wall;			// am I a wall?
	int			border;			// are we in the 'border zone' around a room?  (And thus cannot be used
							// as part of a room)
	bool			visionBlocked;		// we're blocked if we're a wall or under other conditions.
	bool			visible;		// can we see this square?
	bool			lit;			// light on this square?
};

class mapRoom
{
public:
	char			top;
	char			left;
	char			bottom;
	char			right;
	char			glob;
	bool			lit;
};

class mapBase
{
protected:

	mapTile *		m_tile;			// map tiles.  Array is [width*height] long.
	mapTile			m_backgroundType;	// if we ask for something outside the map, what type do we call it?
	uint8			m_width;
	uint8			m_height;
	uint8			m_depth;		// what level are we in the dungeon?
	mapRoom *		m_room[MAX_ROOMS];	// pointers to our rooms.  Yay!
	uint8			m_roomCount;		// how many rooms in this map?

	hnPoint2D		m_stairsUp;		// where our up stairs are located.
	hnPoint2D		m_stairsDown;		// where our down stairs are located.

	hnPoint2D		m_topLeftChanged;
	hnPoint2D		m_topLeftMaxChanged;
	hnPoint2D		m_bottomRightChanged;
	hnPoint2D		m_bottomRightMaxChanged;
	
	void			MarkPointChanged( uint8 x, uint8 y );
	
public:
				mapBase( uint8 width, uint8 height, uint8 depth );
	virtual			~mapBase();

	uint8			GetWidth() { return m_width; }
	uint8			GetHeight() { return m_height; }

	const hnPoint2D &	GetUpStairs() { return m_stairsUp; }
	const hnPoint2D &	GetDownStairs() { return m_stairsDown; }
	
	const hnPoint2D &	GetTopLeftChanged() { return m_topLeftChanged; }
	const hnPoint2D &	GetTopLeftMaxChanged() { return m_topLeftMaxChanged; }
	const hnPoint2D &	GetBottomRightChanged() { return m_bottomRightChanged; }
	const hnPoint2D &	GetBottomRightMaxChanged() { return m_bottomRightMaxChanged; }
	
	void			PrepareVisibility();
	void			UpdateVisibility( const hnPoint & position, mapBase *originalMap );
	
	virtual void		Generate();
	virtual void		GenerateStairsUp() {}
	virtual void		GenerateStairsDown() {}
	
	void			RemoveObject( objBase *object );					// remove object from map
	void			PutObjectAt( objBase *object, uint8 x, uint8 y );			// put object at given coords
	void			MoveObjectTo( objBase *object, uint8 x, uint8 y );			// remove object from map, then put it at given coords
	
	void			RemoveEntity( entBase *object );					// remove object from map
	void			PutEntityAt( entBase *object, uint8 x, uint8 y );			// put object at given coords
	void			MoveEntityTo( entBase *object, uint8 x, uint8 y );			// remove object from map, then put it at given coords
	
	hnMaterialType &	MaterialAt( uint8 x, uint8 y );
	hnWallType &  		WallAt( uint8 x, uint8 y );
	mapTile &		MapTile( uint8 x, uint8 y );
};

#endif // __HN_MAP_H__
