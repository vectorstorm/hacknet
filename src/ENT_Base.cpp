#include "ENT_Base.h"

entBase::entBase( entType type, const hnPoint & pos ):
	m_type(type),
	m_position(pos)
{
	//m_prev = m_next = this;
}

entBase::~entBase()
{
}

const hnPoint &
entBase::GetPosition()
{
	return m_position;
}

void
entBase::SetPosition( const hnPoint & pos )
{
	m_position = pos;
}

void
entBase::Move( hnDirection dir )
{
	// By the time we're here, we've already determined that the requested
	// move is legal.  So go ahead and do it.


}
