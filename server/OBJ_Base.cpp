#include "OBJ_Base.h"

objBase::objBase( objType type, const hnPoint &where ):
	m_type(type),
	m_position(where),
	m_blesscurse(BC_Uncursed),
	m_next(this),
	m_prev(this)
{
}

objBase::~objBase()
{
}

const hnPoint &
objBase::GetPosition()
{
        return m_position;
}

void
objBase::SetPosition( const hnPoint & pos )
{
        m_position = pos;
}



void
objBase::AddObject( objBase * object )
{
        object->m_prev = this;
        object->m_next = m_next;
        m_next->m_prev = object;
        m_next = object;
}

void
objBase::RemoveObject( objBase * object )
{
        object->Unlink();
}

void
objBase::Unlink()
{
        m_next->m_prev = m_prev;
        m_prev->m_next = m_next;

        m_next = m_prev = this;
}

