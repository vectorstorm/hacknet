#ifndef __HN_HACK_MAP_H__
#define __HN_HACK_MAP_H__

#include "MAP_Base.h"

#define MAX_ROOMS (8)

class mapHack : public mapBase
{
	void			MakeRooms();
	bool			CreateRoom(char x, char y, char w, char h, char xalign, char yalign, char lit);
	bool			CheckRoomOkay(const mapRoom &room);
	void			SortRooms();
	void			MakeCorridors();
	void			Join(char roomA, char roomB, bool extraCorridor);
	void			FindEntranceInRect( hnPoint2D *result, const hnPoint2D &topLeft, const hnPoint2D &botRight  );
	bool			DigCorridor( const hnPoint2D &from, const hnPoint2D &to );
	
	bool			CheckTileOkayForRoom(unsigned int x, unsigned int y);	
	bool			OkayForDoor(char x, char y);
	bool			NearDoor(char x, char y);
	
	void			Wallify();
	
public:
				mapHack( unsigned int width, unsigned int height );
	virtual			~mapHack();

	virtual void		Generate();
	virtual void		GenerateStairsUp();
	virtual void		GenerateStairsDown();
};

#endif // __HN_HACK_MAP_H__
