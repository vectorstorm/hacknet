#include "HN_Point.h"
#include "HN_Enum.h"

hnPoint offsetVector[DIR_MAX] = {
	hnPoint(0,-1,0),
	hnPoint(1,-1,0),
	hnPoint(1,0,0),
	hnPoint(1,1,0),
	hnPoint(0,1,0),
	hnPoint(-1,1,0),
	hnPoint(-1,0,0),
	hnPoint(-1,-1,0),
	hnPoint(0,0,-1),
	hnPoint(0,0,1)
};

hnPoint::hnPoint():
	x(0),y(0),z(0)
{
}

hnPoint::hnPoint(sint8 x, sint8 y, sint8 z):
	x(x), 
	y(y), 
	z(z)
{
}

hnPoint::~hnPoint()
{
}

void
hnPoint::Increment( hnDirection dir )
{
	x += offsetVector[dir].x;
	y += offsetVector[dir].y;
	z += offsetVector[dir].z;
}

hnPoint 
hnPoint::operator+(const hnPoint &b) const
{
	return hnPoint(x + b.x, y + b.y, z + b.z );
}

hnPoint 
hnPoint::operator-(const hnPoint &b) const
{
	return hnPoint(x - b.x, y - b.y, z - b.z );
}

hnPoint &
hnPoint::operator+=(const hnPoint &b)
{
	x += b.x; y += b.y; z += b.z;
	return *this;
}

hnPoint &
hnPoint::operator-=(const hnPoint &b)
{
	x += b.x; y += b.y; z += b.z;
	return *this;
}

bool
hnPoint::operator==(const hnPoint &b)
{
	return ( (x == b.x) && (y == b.y) && (z == b.z) );
}



hnPoint2D::hnPoint2D():
	x(0),y(0)
{
}

hnPoint2D::hnPoint2D(sint8 x, sint8 y):
	x(x), 
	y(y)
{
}

hnPoint2D::~hnPoint2D()
{
}

hnPoint2D 
hnPoint2D::operator+(const hnPoint2D &b) const
{
	return hnPoint2D(x + b.x, y + b.y );
}

hnPoint2D 
hnPoint2D::operator-(const hnPoint2D &b) const
{
	return hnPoint2D(x - b.x, y - b.y );
}

hnPoint2D &
hnPoint2D::operator+=(const hnPoint2D &b)
{
	x += b.x; y += b.y;
	return *this;
}

hnPoint2D &
hnPoint2D::operator-=(const hnPoint2D &b)
{
	x -= b.x; y -= b.y;
	return *this;
}

bool
hnPoint2D::operator==(const hnPoint2D &b)
{
	return ( (x == b.x) && (y == b.y) );
}
