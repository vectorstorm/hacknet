#include <stdlib.h>
#include "MAP_Hack.h"
#include "HN_Point.h"

#include "ENT_GridBug.h"

#include <stdio.h>

#define RND(x)  (int)(rand() % (long)(x))
#define min(x,y) ( (x>y)?y:x )
#define max(x,y) ( (x<y)?y:x )

mapHack::mapHack(uint8 width, uint8 height, uint8 depth):
	mapBase(width,height,depth)
{
}

mapHack::~mapHack()
{
}

void
mapHack::GenerateStairsUp()
{
	bool didit = false;

	while ( !didit )
	{
		int x = rand() % GetWidth();
		int y = rand() % GetHeight();

		if ( WallAt(x,y) == WALL_Room )
		{
			WallAt(x,y) = WALL_StairsUp;
			m_stairsUp.Set(x,y);
			didit = true;
		}
	}
}

void
mapHack::GenerateStairsDown()
{
	bool didit = false;

	while ( !didit )
	{
		int x = rand() % GetWidth();
		int y = rand() % GetHeight();

		if ( WallAt(x,y) == WALL_Room )
		{
			WallAt(x,y) = WALL_StairsDown;
			m_stairsDown.Set(x,y);
			didit = true;
		}
	}
}

void
mapHack::MakeMonsters()
{
	for ( int i = 0; i < 10; i++ )
	{
		bool didit = false;

		while ( !didit )
		{
			int x = rand() % GetWidth();
			int y = rand() % GetHeight();

			if ( WallAt(x,y) == WALL_Room && MapTile(x,y).entity == NULL )
			{
				MapTile(x,y).entity = new entGridBug( hnPoint(x, y, m_depth) );
				didit = true;
			}
		}
	}
}

void
mapHack::MakeRooms()
{
	bool done = false;
	
	while( m_roomCount < MAX_ROOMS && !done )
	{
		if (!CreateRoom(-1,-1,-1,-1,-1,-1,true))
			done = true;
		else
			m_roomCount++;
	}
}

void
mapHack::MakeCorridors()
{
	bool anyUnconnectedRooms = true;
	
	printf("We have %d rooms.\n", m_roomCount);

	// Connect each room to the next room to the right.
	for ( int i = 0; i < m_roomCount-1; i++ )
	{
		Join(i, i+1, false);
		if ( (rand() % 50) == 0 )	// randomly stop generating corridors
			break;
	}
	// Connect each room to the room two to the right, if not already connected.
	for ( int i = 0; i < m_roomCount-2; i++ )
		if ( m_room[i]->glob != m_room[i+2]->glob )
			Join(i, i+2, false);
	
	printf("Sanity checks...\n");
	// Now make sure that we've connected each room to something.
	for ( int i = 0; anyUnconnectedRooms && i < m_roomCount; i++ )
	{
		anyUnconnectedRooms = false;
		
		for ( int j = i; j < m_roomCount; j++ )
		{
			if ( m_room[i]->glob != m_room[j]->glob )
			{
				Join(i,j,false);
				anyUnconnectedRooms = true;
			}
		}
	}
	
	printf("Random extra corridors...\n");
	if ( m_roomCount > 2 )
	{
		// just for fun, add some more corridors at random.

		for (int i = (rand() % m_roomCount) + 2; i > 0; i--)
		{
			int a = rand() % m_roomCount;
			int b = rand() % (m_roomCount-2);
			if ( b >= a ) b += 2;
			Join(a,b,true);
		}
	}
}

//  We've been asked to draw a corridor joining room A to room B.
//  if 'extraCorridor' is set, the two rooms are already joined,
//  so we can do mean things like putting boulders in the corridor
//  or not make the corridor at all.
void
mapHack::Join(uint8 roomA, uint8 roomB, bool extraCorridor)
{
	bool success = false;
	
	// pick a door point on each room, and then pass those points off to DrawCorridor.	
	mapRoom *from = m_room[roomA];
	mapRoom *to = m_room[roomB];
	hnPoint2D direction(0,0);
	hnPoint2D topLeft;
	hnPoint2D bottomRight;
	hnPoint2D fromEntrance;
	hnPoint2D toEntrance;
	
	printf("Joining room %d to %d...", roomA, roomB);
	
	if ( to->left > from->right )		// if target's west side is further east
	{					// dig east
		direction.x = 1;
		topLeft.Set( from->right + 1, from->top );
		bottomRight.Set( from->right + 1, from->bottom );
		FindEntranceInRect( &fromEntrance, topLeft, bottomRight );
		topLeft.Set( to->left - 1, to->top );
		bottomRight.Set( to->left - 1, to->bottom );
		FindEntranceInRect( &toEntrance, topLeft, bottomRight );
	}
	else if ( to->bottom < from->top )	// if target's south side is further north
	{					// dig north
		direction.y = -1;
		topLeft.Set( from->left, from->top-1 );
		bottomRight.Set( from->right, from->top-1 );
		FindEntranceInRect( &fromEntrance, topLeft, bottomRight );
		topLeft.Set( to->left, to->bottom+1 );
		bottomRight.Set( to->right, to->bottom+1 );
		FindEntranceInRect( &toEntrance, topLeft, bottomRight );
	}
	else if ( to->right < from->left )	// if target's east side is further west
	{					// dig west
		direction.x = -1;
		topLeft.Set( from->left - 1, from->top );
		bottomRight.Set( from->left - 1, from->bottom );
		FindEntranceInRect( &fromEntrance, topLeft, bottomRight );
		topLeft.Set( to->right + 1, to->top );
		bottomRight.Set( to->right + 1, to->bottom );
		FindEntranceInRect( &toEntrance, topLeft, bottomRight );
	}
	else					// target's north side must be further south
	{					// so dig south  (ASSUMPTION:  NO NESTED ROOMS)
		direction.y = 1;
		topLeft.Set( from->left, from->bottom+1 );
		bottomRight.Set( from->right, from->bottom+1 );
		FindEntranceInRect( &fromEntrance, topLeft, bottomRight );
		topLeft.Set( to->left, to->top-1 );
		bottomRight.Set( to->right, to->top-1 );
		FindEntranceInRect( &toEntrance, topLeft, bottomRight );
	}
	
	success = DigCorridor( fromEntrance, toEntrance );
	
	//printf("Digging corridor...");
	
	if ( success )
	{
		printf("success!\n");

		// this #if 0 is removing a piece of code which was written to emulate
		// what appears to be a bug in NetHack's level generation code..  Inside 
		// the #else clause is a version which correctly 'globs' rooms
		// together function, which is what it appears that NetHack's global array
		// 'smeq' is intended to do. (But doesn't seem to do correctly)
		//
		// I'll be extremely grateful to anyone who can explain to me
		// the relevance of the name 'smeq' to this room globbing function.  :)
		// "Simple Map EQuality", perhaps?  I'm baffled.
#if 0
		to->glob = from->glob = min( from->glob, to->glob );
#else
		int changeFromGlobID = max( to->glob, from->glob );
		int changeToGlobID = min( to->glob, from->glob );
		
		for ( int i = 0; i < m_roomCount; i++ )
		{
			if ( m_room[i]->glob == changeFromGlobID )
				m_room[i]->glob = changeToGlobID;
		}
#endif
	}
	else
	{
		printf("failed.\n");
	}
}

#define WALL_CorridorDiggable ( WALL_Solid | WALL_Corridor )

bool
mapHack::DigCorridor( const hnPoint2D &f, const hnPoint2D &t )
{
	// f is from, t is to.
	bool success = true;
	
	hnWallType tType = WallAt(t.x,t.y);
	
	WallAt(f.x,f.y) = WALL_Solid;		// temporarily set our 'doorway' points to solid stone, so we can dig through them.
	WallAt(t.x,t.y) = WALL_Solid;
	
	sint8 dx = 0;
	sint8 dy = 0;
	
	if ( t.x > f.x ) dx = 1;
	else if ( t.y > f.y ) dy = 1;
	else if ( t.x < f.x ) dx = -1;
	else dy = -1;

	int corridorCount = 0;
	hnPoint2D corridor = f;
	
	corridor.x -= dx;		// take a step backwards into the room so we start where we want to start
	corridor.y -= dy;
	
	while ( corridor != t )
	{
		if ( corridorCount++ > 500 )
		{
			success = false;
			break;
		}
		
		corridor.x += dx;
		corridor.y += dy;
		
		if ( corridor == t )
		{
			break;
		}
		
		if ( corridor.x >= m_width || corridor.x < 0 || corridor.y >= m_height || corridor.y < 0 )
		{
			success = false;
			break;
		}

		if ( WallAt(corridor.x, corridor.y) & WALL_Solid )
		{
			WallAt(corridor.x, corridor.y) = WALL_Corridor;
		}
		else if ( !(WallAt(corridor.x, corridor.y) & WALL_Corridor) )
		{
			success = false;	// weird..  we've run into something unknown.
			break;
		}
		
		// check to see if we want to change direction
		sint8 dix = abs(corridor.x - t.x);
		sint8 diy = abs(corridor.y - t.y);
		
		//printf("travel (%d,%d), offset (%d, %d).\n", dx, dy, dix, diy);
		
		// if we're going vertically but we really want to be going horizontally...
		if ( dy && dix > diy )
		{
			//printf("Turning.\n");
			int ddx = (corridor.x > t.x) ? -1 : 1;

			if ( WallAt(corridor.x+ddx, corridor.y) & WALL_CorridorDiggable )
			{
				dx = ddx;
				dy = 0;
				continue;
			}
			else
			{
				//printf("Aborting turning.\n");
			}
		}
		else if ( dx && diy > dix )
		{
			//printf("Turning.\n");
			int ddy = (corridor.y > t.y) ? -1 : 1;

			if ( WallAt(corridor.x, corridor.y+ddy) & WALL_CorridorDiggable )
			{
				dx = 0;
				dy = ddy;
				continue;
			}
			else
			{
				//printf("Aborting turning.\n");
			}
		}
		
		// can we continue straight?
		if ( WallAt(corridor.x+dx,corridor.y+dy) & WALL_CorridorDiggable )
			continue;
	
		// no?  What now?
	
		if ( dx )
		{
			//printf("Emergency Turning.\n");
			dx = 0;
			dy = (t.y < corridor.y) ? -1 : 1;
		}
		else
		{
			//printf("Emergency Turning.\n");
			dx = (t.x < corridor.x) ? -1 : 1;
			dy = 0;
		}

		if ( WallAt(corridor.x+dx,corridor.y+dy) & WALL_CorridorDiggable )
			continue;
	
		//printf("Emergency Turning Around!\n");
		dx = -dx;
		dy = -dy;
	}

	// if we've made it here, we've succeeded !
	
	WallAt(f.x,f.y) = WALL_Doorway;		// our 'from' point always gets its door
	
	if ( success )
		WallAt(t.x,t.y) = WALL_Doorway;
	else
		WallAt(t.x,t.y) = tType;	// if we failed, put our 'to' point back to what it was before.
	
	return success;
}

void
mapHack::FindEntranceInRect( hnPoint2D *result, const hnPoint2D & topLeft, const hnPoint2D & bottomRight )
{
	sint8 x, y;

	x = (topLeft.x == bottomRight.x) ? topLeft.x : (rand() % (bottomRight.x - topLeft.x)) + topLeft.x;
	y = (topLeft.y == bottomRight.y) ? topLeft.y : (rand() % (bottomRight.y - topLeft.y)) + topLeft.y;

	if ( OkayForDoor(x,y) )
	{
		result->Set(x,y);
		return;
	}

	for ( x = topLeft.x; x <= bottomRight.x; x++ )
		for ( y = topLeft.y; y <= bottomRight.y; y++ )	// check each square on this side for a place we can put a door
			if ( OkayForDoor(x,y) )
			{
				result->Set(x,y);
				return;
			}
	
	for ( x = topLeft.x; x <= bottomRight.x; x++ )
		for ( y = topLeft.y; y <= bottomRight.y; y++ )	// find an existing door on this side, and use that instead.
			if ( WallAt(x,y) & WALL_RoomEntrance )
			{
				result->Set(x,y);
				return;
			}
	// Weird.  We should never get here!

	*result = topLeft;
}

void
mapHack::SortRooms()
{
	bool swapped;
	
	do{
		swapped = false;

		for ( int i = 0; i < m_roomCount-1; i++ )
		{
			if ( m_room[i]->left > m_room[i+1]->left )
			{
				//printf("Swapping %d with %d.\n", i, i+1);
				mapRoom *hold = m_room[i+1];
				m_room[i+1] = m_room[i];
				m_room[i] = hold;
				swapped = true;
			}
		}
		
	}while(swapped);

	for ( int i = 0; i < m_roomCount; i++ )
	{
		m_room[i]->glob = i;
//		printf("Left coord %d: %d\n", i, m_room[i]->left);
	}

//	printf("Done swapping.\n");


}

bool
mapHack::CreateRoom(sint8 x, sint8 y, sint8 w, sint8 h, sint8 xalign, sint8 yalign, bool lit)
{

	if ( m_roomCount >= MAX_ROOMS )
		return false;

	mapRoom result;
	bool success = false;
	bool exit = false;
	char trycount = 0;
	
	lit = true;	// always fully lit, for now.
	
	do{
		char xtmp = x;
		char ytmp = y;
		char wtmp = w;
		char htmp = h;
		char xaltmp = xalign;
		char yaltmp = yalign;
		char xabs, yabs;
		
		if ( xtmp < 0 || ytmp < 0 )
		{
			xtmp = rand() % 5;
			ytmp = rand() % 5;
		}
		if ( wtmp < 0 || htmp < 0 )
		{
			wtmp = (rand()%15)+3;
			htmp = (rand()%8)+2;
		}
		if ( xaltmp < 0 )
			xaltmp = rand()%3;
		if ( yaltmp < 0 )
			yaltmp = rand()%3;

		xabs = (((xtmp) * m_width) / 5) + 1;
		yabs = (((ytmp) * m_height) / 5) + 1;

		switch( xaltmp )
		{
			case 0:
				break;
			case 1:
				xabs += (m_width/5) - wtmp;
				break;
			case 2:
				xabs += ((m_width/5) - wtmp) / 2;
				break;
		}
		switch( yaltmp )
		{
			case 0:
				break;
			case 1:
				yabs += (m_height/5) - htmp;
				break;
			case 2:
				yabs += ((m_height/5) - htmp) / 2;
				break;
		}

		if ( xabs + wtmp - 1 > m_width - 2 )
			xabs = m_width - wtmp - 3;
		if ( xabs < 2 )
			xabs = 2;
		if ( yabs + htmp - 1 > m_height - 2 )
			yabs = m_height - htmp - 3;
		if ( yabs < 2 )
			yabs = 2;
		
		if ( htmp * wtmp > 50 )
		{
			htmp = 50 / wtmp;	// if we're too big, make us smaller.
		}
		
		result.top = yabs;
		result.left = xabs;
		result.bottom = yabs + htmp;
		result.right = xabs + wtmp;
		
		if ( CheckRoomOkay(result) )
			success = true;
	
	}while(++trycount<100 && !success && !exit);
	
	if ( success )
	{
		for ( int i = result.left; i <= result.right; i++ )
			for ( int j = result.top; j <= result.bottom; j++ )
			{
				WallAt(i,j) = WALL_Room;
				MapTile(i,j).lit = lit;
			}
		
		// now make our walls..

		WallAt(result.left-1,result.top-1) = WALL_Cross;
		WallAt(result.left-1,result.bottom+1) = WALL_Cross;
		WallAt(result.right+1,result.top-1) = WALL_Cross;
		WallAt(result.right+1,result.bottom+1) = WALL_Cross;

		for ( int i = result.left; i <= result.right; i++ )
		{
			WallAt(i,result.top-1) = WALL_Horizontal;
			WallAt(i,result.bottom+1) = WALL_Horizontal;
		}
		
		for ( int j = result.top; j <= result.bottom; j++ )
		{
			WallAt(result.left-1,j) = WALL_Vertical;
			WallAt(result.right+1,j) = WALL_Vertical;
		}
		
		printf("Succeeded at generating room: %dx%d at %d,%d.\n", result.right - result.left,result.bottom-result.top, result.left, result.top);
		
		for ( int i = result.left-5; i <= result.right+5; i++ )
			for ( int j = result.top-5; j <= result.bottom+5; j++ )
			{
				MapTile(i,j).border = true;
			}
		
		m_room[m_roomCount] = new mapRoom;
		*m_room[m_roomCount] = result;
		m_room[m_roomCount]->glob = m_roomCount;
	}
	
	return success;
}

bool
mapHack::OkayForDoor(uint8 x, uint8 y)
{
	bool okay = false;

	if ( WallAt(x,y) & WALL_Any )
	{
		if ( !NearDoor(x,y) )
			okay = true;
	}

	return okay;
}

bool
mapHack::NearDoor(uint8 x, uint8 y)
{
	bool nearDoor = false;
	
	for ( sint8 i = x-1; i <= x+1; i++ )
		for ( sint8 j = y-1; j <= y+1; j++ )
		{
			if ( WallAt(i,j) & WALL_RoomEntrance )
				nearDoor = true;
		}
	return nearDoor;		// are we near a doorway or a door?
}

bool
mapHack::CheckRoomOkay( const mapRoom &room )
{
	bool roomOkay = true;

	for ( int i = room.left; i <= room.right; i++ )
		for ( int j = room.top; j <= room.bottom; j++ )
		{
			if ( !CheckTileOkayForRoom(i,j) )
				roomOkay = false;
		}

	return roomOkay;
}

bool
mapHack::CheckTileOkayForRoom( uint8 x, uint8 y )
{
	bool tileOkay = true;
	
	mapTile &tile = MapTile(x,y);
	
	if ( x >= (m_width-1) )	// make sure we don't generate rooms
		tileOkay = false;		// with open space on left or right border
	
	if ( y >= (m_height-1) )	// make sure we don't generate rooms
		tileOkay = false;		// with open space on top or bottom border
	
	if ( (!(tile.wall & WALL_Any)) || tile.border )
		tileOkay = false;		// not an okay tile if there's no wall here yet, or if we're
						// in the border zone of another room.
	return tileOkay;
}

void
mapHack::Wallify()
{
	for ( int i = 0; i < m_width; i++ )
		for ( int j = 0; j < m_height; j++ )
			if ( WallAt(i,j) & WALL_Any )
				MaterialAt(i,j) = MATERIAL_Rock;
			else if ( WallAt(i,j) & WALL_Room )
				MaterialAt(i,j) = MATERIAL_Dirt;
			else if ( WallAt(i,j) & WALL_Corridor )
				MaterialAt(i,j) = MATERIAL_Ice;
}

void
mapHack::Generate()
{
	for ( int i = 0; i < m_width * m_height; i++ )  //clear us to fully rock
	{
		m_tile[i].wall = WALL_Solid;
		m_tile[i].material = MATERIAL_Rock;
		m_tile[i].border = false;
	}
	
	MakeRooms();
	SortRooms();
	MakeCorridors();
	Wallify();
	MakeMonsters();

	PrepareVisibility();
}
