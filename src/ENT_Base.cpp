#include "ENT_Base.h"

entBase::entBase( entType type, const hnPoint & pos ):
	m_type(type),
	m_position(pos)
{
	m_prev = m_next = this;
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
entBase::AddObject( entBase * object )
{
	object->m_prev = this;
	object->m_next = m_next;
	m_next->m_prev = object;
	m_next = object;
}

void
entBase::RemoveObject( entBase * object )
{
	object->Unlink();
}

void
entBase::Unlink()
{
	m_next->m_prev = m_prev;
	m_prev->m_next = m_next;

	m_next = m_prev = this;
}
