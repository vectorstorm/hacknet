#ifndef __HN_ENTITY_H__
#define __HN_ENTITY_H__

#include "HN_Enum.h"
#include "HN_Point.h"

class hnEntity
{
	sint32		m_id;					// unique object ID number
	sint8		m_revision;				// revision counter.  Each time this object is visibly
								// changed (ie: polymorphed, destroyed, etc.), this value 
								// is incremented by one.
	hnEntityType	m_type;
	hnPoint		m_position;
	
	hnEntity	*m_next;
	hnEntity	*m_prev;

public:
				hnEntity( hnEntityType type, const hnPoint & );
	virtual			~hnEntity();
	
	const hnPoint &		GetPosition();
	void			SetPosition( const hnPoint & );
	hnEntityType		GetType() { return m_type; }
	
	void			AddObject(hnEntity *object);	// add us into our circular linked list
	void			RemoveObject(hnEntity *object);	// find this object in our circular linked list and remove it

	virtual void		Move( const hnPoint & offset ) {}
	virtual void		MoveTo( const hnPoint & position ) {}
	
protected:
	
	void			Unlink();
	
	
};

#endif
