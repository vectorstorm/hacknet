#ifndef __HN_POINT_H__
#define __HN_POINT_H__

#include "HN_Types.h"
#include "HN_Enum.h"

class hnPoint
{
public:
			hnPoint();
			hnPoint(sint8 x, sint8 y, sint8 z);
	virtual		~hnPoint();	
	
	sint8		x, y, z;

	void		Set(sint8 newx, sint8 newy, sint8 newz) {x=newx;y=newy;z=newz;}

	void		Increment( hnDirection dir );

	hnPoint 	operator+(const hnPoint &) const;
	hnPoint		operator-(const hnPoint &) const;
	hnPoint & 	operator+=(const hnPoint &);
	hnPoint &	operator-=(const hnPoint &);
	bool		operator==(const hnPoint &);
	bool		operator!=(const hnPoint &pt) { return !(*this==pt); }
};

class hnPoint2D
{
public:
			hnPoint2D();
			hnPoint2D(sint8 x, sint8 y);
	virtual		~hnPoint2D();
			
	sint8		x, y;
	void		Set(sint8 newx, sint8 newy) {x=newx;y=newy;}

	hnPoint2D	operator+(const hnPoint2D &) const;
	hnPoint2D	operator-(const hnPoint2D &) const;
	hnPoint2D &	operator+=(const hnPoint2D &);
	hnPoint2D &	operator-=(const hnPoint2D &);
	bool		operator==(const hnPoint2D &);
	bool		operator!=(const hnPoint2D &pt) { return !(*this==pt); }
};

#endif //__HN_POINT_H__
