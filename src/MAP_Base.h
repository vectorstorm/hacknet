#ifndef __HN_MAP_H__
#define __HN_MAP_H__

#include "HN_Enum.h"
#include "HN_Types.h"
#include "HN_Point.h"

#define MAX_ROOMS (8)

class objBase;
class entBase;

class mapTile
{
public:
				mapTile();
	virtual 		~mapTile();
	
	objBase *		object;			// linked list of objects lying on the floor here.
	entBase *		entity;			// if a creature/player is standing here, this is a pointer to that creature.
	
	hnMaterialType		material;		// what type of material makes up this tile?
	hnWallType		wall;			// am I a wall?
	int			border;			// are we in the 'border zone' around a room?  (And thus cannot be used
							// as part of a room)
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
	sint8			m_width;
	sint8			m_height;
	mapRoom *		m_room[MAX_ROOMS];	// pointers to our rooms.  Yay!
	sint8			m_roomCount;		// how many rooms in this map?
	
public:
				mapBase( sint8 width, sint8 height );
	virtual			~mapBase();

	virtual void		Generate();
	
	void			RemoveObject( objBase *object );					// remove object from map
	void			PutObjectAt( objBase *object, sint8 x, sint8 y );			// put object at given coords
	void			MoveObjectTo( objBase *object, sint8 x, sint8 y );			// remove object from map, then put it at given coords
	
	void			RemoveEntity( entBase *object );					// remove object from map
	void			PutEntityAt( entBase *object, sint8 x, sint8 y );			// put object at given coords
	void			MoveEntityTo( entBase *object, sint8 x, sint8 y );			// remove object from map, then put it at given coords
	
	hnMaterialType &	MaterialAt( sint8 x, sint8 y );
	hnWallType &  		WallAt( sint8 x, sint8 y );
	mapTile &		MapTile( sint8 x, sint8 y );
};

#endif // __HN_MAP_H__
