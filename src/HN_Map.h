#ifndef __HN_MAP_H__
#define __HN_MAP_H__

#include "HN_Enum.h"
#include "HN_Types.h"
#include "HN_Point.h"

#define MAX_ROOMS (8)

class hnEntity;

class hnMapTile
{
public:
				hnMapTile();
	virtual 		~hnMapTile();
	
	hnEntity *		object;			// linked list of objects lying on the floor here.
	hnEntity *		entity;			// if a creature/player is standing here, this is a pointer to that creature.
	
	hnMaterialType		material;		// what type of material makes up this tile?
	hnWallType		wall;			// am I a wall?
	int			border;			// are we in the 'border zone' around a room?  (And thus cannot be used
							// as part of a room)
};

class hnRoom
{
public:
	char			top;
	char			left;
	char			bottom;
	char			right;
	char			glob;
	bool			lit;
};

class hnMap
{
protected:

	hnMapTile *		m_tile;			// map tiles.  Array is [width*height] long.
	hnMapTile		m_backgroundType;	// if we ask for something outside the map, what type do we call it?
	sint8			m_width;
	sint8			m_height;
	hnRoom *		m_room[MAX_ROOMS];	// pointers to our rooms.  Yay!
	sint8			m_roomCount;		// how many rooms in this map?
	
public:
				hnMap( sint8 width, sint8 height );
	virtual			~hnMap();

	virtual void		Generate();
	
	void			RemoveObject( hnEntity *object );					// remove object from map
	void			PutObjectAt( hnEntity *object, sint8 x, sint8 y );			// put object at given coords
	void			MoveObjectTo( hnEntity *object, sint8 x, sint8 y );			// remove object from map, then put it at given coords
	
	hnMaterialType &	MaterialAt( sint8 x, sint8 y );
	hnWallType &  		WallAt( sint8 x, sint8 y );
	hnMapTile &		MapTile( sint8 x, sint8 y );
};

#endif // __HN_MAP_H__
