#ifndef __HN_OBJECT_H__
#define __HN_OBJECT_H__

#include "HN_Enum.h"
#include "HN_Point.h"

class hnObject
{
	hnObjectType	m_type;
	hnPoint		m_position;

	hnObject	*m_next;
	hnObject	*m_prev;

public:
				hnObject( hnObjectType type, const hnPoint & );
	virtual			~hnObject();
	
	const hnPoint &		GetPosition();
	void			SetPosition( const hnPoint & );
	hnObjectType		GetType() { return m_type; }
	
	void			AddObject(hnObject *object);	// add us into our circular linked list
	void			RemoveObject(hnObject *object);	// find this object in our circular linked list and remove it

	virtual void		Move( const hnPoint & offset ) {}
	virtual void		MoveTo( const hnPoint & position ) {}
	
protected:
	
	void			Unlink();
	
	
};

#endif
