#ifndef __HN_HACK_MAP_H__
#define __HN_HACK_MAP_H__

#include "HN_Map.h"

#define MAX_ROOMS (8)

class hnHackMap : public hnMap
{
	void			MakeRooms();
	bool			CreateRoom(char x, char y, char w, char h, char xalign, char yalign, char lit);
	bool			CheckRoomOkay(const hnRoom &room);
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
				hnHackMap( unsigned int width, unsigned int height );
	virtual			~hnHackMap();

	virtual void		Generate();
};

#endif // __HN_HACK_MAP_H__
