#ifndef __MAP_CLIENT_H__
#define __MAP_CLIENT_H__

#include "HN_Enum.h"
#include "HN_Types.h"
#include "HN_Point.h"
#include "ENT_Types.h"
#include "OBJ_Types.h"

class mapClientTile
{
public:
				mapClientTile();
	virtual 		~mapClientTile();
	
	objType 		object;			// linked list of objects lying on the floor here.
	entType			entity;			// if a creature/player is standing here, this is a pointer to that creature.
	
	hnMaterialType		material;		// what type of material makes up this tile?
	hnWallType		wall;			// am I a wall?
	int			border;			// are we in the 'border zone' around a room?  (And thus cannot be used
							// as part of a room)
};

class mapClient
{
protected:

	mapClientTile *		m_tile;			// map tiles.  Array is [width*height] long.
	mapClientTile		m_backgroundType;	// if we ask for something outside the map, what type do we call it?
	sint8			m_width;
	sint8			m_height;
	sint8			m_roomCount;		// how many rooms in this map?
	
public:
				mapClient( sint8 width, sint8 height );
	virtual			~mapClient();

	virtual void		Generate();
	
	void			RemoveObject( objType object );					// remove object from map
	void			PutObjectAt( objType object, sint8 x, sint8 y );			// put object at given coords
	void			MoveObjectTo( objType object, sint8 x, sint8 y );			// remove object from map, then put it at given coords
	
	void			RemoveEntity( entType object );					// remove object from map
	void			PutEntityAt( entType object, sint8 x, sint8 y );			// put object at given coords
	void			MoveEntityTo( entType object, sint8 x, sint8 y );			// remove object from map, then put it at given coords
	
	hnMaterialType &	MaterialAt( sint8 x, sint8 y );
	hnWallType &  		WallAt( sint8 x, sint8 y );
	mapClientTile &		MapTile( sint8 x, sint8 y );
};

#endif // __MAP_CLIENT_H__
