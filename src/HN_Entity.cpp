#include "HN_Entity.h"

hnEntity::hnEntity( hnEntityType type, const hnPoint & pos ):
	m_type(type),
	m_position(pos)
{
	m_prev = m_next = this;
}

hnEntity::~hnEntity()
{
}

const hnPoint &
hnEntity::GetPosition()
{
	return m_position;
}

void
hnEntity::SetPosition( const hnPoint & pos )
{
	m_position = pos;
}

void
hnEntity::AddObject( hnEntity * object )
{
	object->m_prev = this;
	object->m_next = m_next;
	m_next->m_prev = object;
	m_next = object;
}

void
hnEntity::RemoveObject( hnEntity * object )
{
	object->Unlink();
}

void
hnEntity::Unlink()
{
	m_next->m_prev = m_prev;
	m_prev->m_next = m_next;

	m_next = m_prev = this;
}
