#ifndef __HN_ENTITY_H__
#define __HN_ENTITY_H__

#include "HN_Point.h"

enum {
        ENTITY_None,                    // a mapobjectupdate with this type means nothing's here any more.
		        
        ENTITY_Monster,      	        // there should be one of these for every type of monster
        ENTITY_Player,

        ENTITY_MAX
};

typedef uint8 entType;

class entBase
{
	sint32		m_id;					// unique object ID number
	sint8		m_revision;				// revision counter.  Each time this object is visibly
								// changed (ie: polymorphed, destroyed, etc.), this value 
								// is incremented by one.
	entType	m_type;
	hnPoint		m_position;
	
	entBase	*m_next;
	entBase	*m_prev;

public:
				entBase( entType type, const hnPoint & );
	virtual			~entBase();
	
	const hnPoint &		GetPosition();
	void			SetPosition( const hnPoint & );
	entType			GetType() { return m_type; }
	
	void			AddObject(entBase *object);	// add us into our circular linked list
	void			RemoveObject(entBase *object);	// find this object in our circular linked list and remove it

	virtual void		Move( const hnPoint & offset ) {}
	virtual void		MoveTo( const hnPoint & position ) {}
	
protected:
	
	void			Unlink();
	
	
};

#endif
