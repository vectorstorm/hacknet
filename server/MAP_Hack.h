#ifndef __HN_HACK_MAP_H__
#define __HN_HACK_MAP_H__

#include "MAP_Base.h"

#define MAX_ROOMS (8)

class mapHack : public mapBase
{
	void			MakeRooms();
	bool			CreateRoom(sint8 x, sint8 y, sint8 w, sint8 h, sint8 xalign, sint8 yalign, bool lit);
	bool			CheckRoomOkay(const mapRoom &room);
	void			SortRooms();
	void			MakeCorridors();
	void			Join(uint8 roomA, uint8 roomB, bool extraCorridor);
	void			FindEntranceInRect( hnPoint2D *result, const hnPoint2D &topLeft, const hnPoint2D &botRight  );
	bool			DigCorridor( const hnPoint2D &from, const hnPoint2D &to );
	
	bool			CheckTileOkayForRoom(uint8 x, uint8 y);	
	bool			OkayForDoor(uint8 x, uint8 y);
	bool			NearDoor(uint8 x, uint8 y);
	
	void			Wallify();

	void			GenerateMonsters();
	void			GenerateObjects();
	
public:
				mapHack( uint8 width, uint8 height, uint8 depth );
	virtual			~mapHack();

	virtual void		Generate();
	virtual void		GenerateStairsUp();
	virtual void		GenerateStairsDown();
};

#endif // __HN_HACK_MAP_H__
