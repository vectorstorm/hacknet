#include "OBJ_Base.h"

char * objBase::s_name = "BaseObject";

objBase::objBase( objType type, const hnPoint &where ):
	m_type(type),
	m_position(where),
	m_blesscurse(BC_Uncursed),
	m_count(1),
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
	//---------------------------------------------------------
	//  We can't just innocently add the object to our list --
	//  since we're tracking object counts and stacking items
	//  properly, we need to actually check this new object
	//  against our current contents, and only add it if we
	//  don't have another object of identical type!
	//---------------------------------------------------------

	objBase *shuttle = m_next;

	while ( shuttle != this )
	{
		if ( shuttle->m_type == object->m_type &&
			shuttle->m_blesscurse == object->m_blesscurse )
		{
			// we're stackable, so just stack us.
			shuttle->m_count += object->m_count;
			delete object;
			return;
		}
		
		shuttle = shuttle->m_next;
	}
	//---------------------------------------------------------
	//  If we reach this point, we weren't stackable, so add
	//  us to the list.
	//---------------------------------------------------------
	
        object->m_prev = this;
        object->m_next = m_next;
        m_next->m_prev = object;
        m_next = object;
}

void
objBase::RemoveObject( objBase * object )
{
	//---------------------------------------------------------
	//  Check the list.  If we find this object in our list,
	//  then remove it from the list.  This function probably
	//  will never actually be called.
	//---------------------------------------------------------

	objBase *shuttle = m_next;

	while ( shuttle != this )
	{
		if ( shuttle == object )
		{
			object->Unlink();
			break;
		}
		shuttle = shuttle->m_next;
	}
}

void
objBase::Unlink()
{
        m_next->m_prev = m_prev;
        m_prev->m_next = m_next;

        m_next = m_prev = this;
}

bool
objBase::ExactMatch( const objDescription &desc )
{
	bool matches = false;
	
	if ( desc.type == m_type )
	{
		if ( desc.blesscurse == BC_Unknown ||
			desc.blesscurse == m_blesscurse )
		{
			if ( desc.count == m_count )
			{
				matches = true;
			}
		}
	}

	return matches;
}

bool
objBase::PartialMatch( const objDescription &desc )
{
	bool matches = false;
	
	if ( desc.type == m_type )
	{
		if ( desc.blesscurse == BC_Unknown ||
			desc.blesscurse == m_blesscurse )
		{
			if ( desc.count <= m_count )
			{
				matches = true;
			}
		}
	}

	return matches;
}


int
objBase::ObjectCount()
{
	int count = 0;

	objBase *shuttle = m_next;

	while ( shuttle != this )
	{
		count++;
		shuttle = shuttle->m_next;
	}

	return count;
}

objBase *
objBase::GetObject(int id)
{
	objBase *result = NULL;
	
	if ( id < ObjectCount() )
	{
		result = m_next;

		for (int i = 0; i < id; i++)
			result = result->m_next;
	}
	
	return result;
}

void
objBase::GetDescription(objDescription &result, int id)
{
	objBase *object = GetObject(id);

	if ( object )
		object->FillDescription(result);
}

void
objBase::FillDescription(objDescription &desc)
{
	desc.type = m_type;
	desc.count = m_count;
	desc.blesscurse = m_blesscurse;
}
