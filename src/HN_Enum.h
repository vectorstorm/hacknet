#ifndef __HN_ENUM_H__
#define __HN_ENUM_H__

#include "HN_Types.h"

/***  MAP TILE MATERIAL ***/

enum hnDirection{
	DIR_North = 0,
	DIR_NorthEast = 1,
	DIR_East = 2,
	DIR_SouthEast = 3,
	DIR_South = 4,
	DIR_SouthWest = 5,
	DIR_West = 6,
	DIR_NorthWest = 7,
	DIR_Up = 8,
	DIR_Down = 9
};

enum {
	MATERIAL_Unknown,	// haven't yet been seen
	MATERIAL_Dirt,	// dirt.  Easily diggable
	MATERIAL_Rock,	// stone.  Not diggable
	MATERIAL_Ice,	// ice.  diggable, etc.
	MATERIAL_Water,	// water.  Not diggable.  If a 'wall', it's a waterfall.
	MATERIAL_Lava,	// lava.  Not diggable.  If a 'wall', it's a lavafall.  (Whee!)
	MATERIAL_None,     // no floor here.. fall to the next level down.  (This is different from a trap door!)
	MATERIAL_MAX
};

typedef sint16 hnMaterialType;


/**   MAP TILE WALL STATUS  **/

//
//  This has GOT to be the wrong way to do this.
//
//  TODO:  Figure out the following issues:
//
//  Do we need a 'horizontal' and 'vertical' wall type, or is that an element
//  of a TTY-based client that is meaningless for other client types?  ie:
//  should it be replaced by just 'WALL_None' and 'WALL_Present', or somesuch,
//  and let the client figure out how to draw the wall based upon nearby open
//  space.
//
//  Also, doors should probably be an external bitflag, not a part of the
//  wall type.  Or should they be an object?
//

enum {	// actually, 'vertical structure' type stuff goes here.
	
	WALL_Unknown 	= 	0x000,
	WALL_Room 	= 	0x001,
	WALL_Corridor 	= 	0x002,
	WALL_Doorway 	= 	0x004,
	WALL_OpenDoor 	= 	0x008,
	WALL_ClosedDoor = 	0x010,
	WALL_Solid 	=	0x020,
	WALL_Horizontal = 	0x040,
	WALL_Vertical 	= 	0x080,
	WALL_StairsUp	=	0x100,
	WALL_StairsDown =	0x200
};

typedef sint16 hnWallType;

#define WALL_Stairs		(WALL_StairsUp | WALL_StairsDown)
#define WALL_Passable		(WALL_Room | WALL_Corridor | WALL_Doorway | WALL_OpenDoor | WALL_Stairs)
#define WALL_None		(WALL_Room | WALL_Corridor | WALL_Stairs)
#define WALL_Any		(WALL_Horizontal | WALL_Vertical | WALL_Solid)
#define WALL_Door		(WALL_OpenDoor | WALL_ClosedDoor)
#define WALL_WithinRoom		(WALL_Room | WALL_Stairs)
#define WALL_RoomEntrance	(WALL_Doorway | WALL_Door)
#define WALL_Cross		(WALL_Horizontal | WALL_Vertical)

#endif //__HN_ENUM_H__
