#ifndef __MAP_CLIENT_H__
#define __MAP_CLIENT_H__

#include "HN_Enum.h"
#include "HN_Types.h"
#include "HN_Point.h"
#include "ENT_Types.h"
#include "OBJ_Types.h"

class mapBase;

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
	uint8			m_width;
	uint8			m_height;
	uint8			m_roomCount;		// how many rooms in this map?

	bool			m_visited;		// have I been visited yet?
	
public:
				mapClient( uint8 width, uint8 height );
	virtual			~mapClient();

	uint8			GetWidth() { return m_width; }
	uint8			GetHeight() { return m_height; }
	
	bool			isVisited() { return m_visited; }
	void			SetVisited(bool val) { m_visited = val; }
	virtual void		Generate();

	void			RemoveObject( objType object );					// remove object from map
	void			PutObjectAt( objType object, uint8 x, uint8 y );			// put object at given coords
	void			MoveObjectTo( objType object, uint8 x, uint8 y );			// remove object from map, then put it at given coords
	
	void			RemoveEntity( entType object );					// remove object from map
	void			PutEntityAt( entType object, uint8 x, uint8 y );			// put object at given coords
	void			MoveEntityTo( entType object, uint8 x, uint8 y );			// remove object from map, then put it at given coords

	hnMaterialType &	MaterialAt( uint8 x, uint8 y );
	hnWallType &  		WallAt( uint8 x, uint8 y );
	mapClientTile &		MapTile( uint8 x, uint8 y );
};

#endif // __MAP_CLIENT_H__
