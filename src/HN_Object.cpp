#include "HN_Object.h"

hnObject::hnObject( hnObjectType type, const hnPoint & pos ):
	m_type(type),
	m_position(pos)
{
	m_prev = m_next = this;
}

hnObject::~hnObject()
{
}

const hnPoint &
hnObject::GetPosition()
{
	return m_position;
}

void
hnObject::SetPosition( const hnPoint & pos )
{
	m_position = pos;
}

void
hnObject::AddObject( hnObject * object )
{
	object->m_prev = this;
	object->m_next = m_next;
	m_next->m_prev = object;
	m_next = object;
}

void
hnObject::RemoveObject( hnObject * object )
{
	object->Unlink();
}

void
hnObject::Unlink()
{
	m_next->m_prev = m_prev;
	m_prev->m_next = m_next;

	m_next = m_prev = this;
}
