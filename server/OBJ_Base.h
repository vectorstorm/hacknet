#ifndef __OBJ_BASE_H__
#define __OBJ_BASE_H__

#include "HN_Types.h"
#include "HN_Point.h"
#include "OBJ_Types.h"

class objBase
{
	objType		m_type;
	hnPoint		m_position;
	
	uint8		m_blesscurse;
	uint8		m_count;	// how many of us are there?

	objBase *	m_next;
	objBase *	m_prev;
	
protected:
	void		Unlink();	
public:
			objBase( objType type, const hnPoint & where );
	virtual 	~objBase();

        const hnPoint &		GetPosition();
	void			SetPosition( const hnPoint & );
	objType			GetType() { return m_type; }

        void		AddObject(objBase *object);     // add us into our circular linked list
	void		RemoveObject(objBase *object);  // find this object in our circular linked list and remove it
	objBase *	RemoveObjectDescription( objDescription *objectDescription );	// match the description against one or more objects in our list and remove them, returning the removed object(s).

	objBase *	GetFirstObject();		// gets the first object in our list.
	objBase *	GetNextObject(objBase *object); // gets the next object in our list.
		
	
	bool		Blessed() { return (m_blesscurse == BC_Blessed); }
	bool		Cursed() { return (m_blesscurse == BC_Cursed); }
};



#endif //__OBJ_BASE_H__
