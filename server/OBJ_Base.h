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
	static char *	s_name;
public:
				objBase( objType type, const hnPoint & where );
	virtual 		~objBase();

        const hnPoint &		GetPosition();
	void			SetPosition( const hnPoint & );
	objType			GetType() { return m_type; }

	virtual const char *	GetName() { return s_name; }
	
        void			AddObject(objBase *object);     // add us into our circular linked list
	void			RemoveObject(objBase *object);  // find this object in our circular linked list and remove it
	objBase *		RemoveObjectDescription( objDescription *objectDescription );	// match the description against one or more objects in our list and remove them, returning the removed object(s).

	int			ObjectCount();
	objBase *		GetObject(int id);
	void			GetDescription(objDescription &result, int id);

	void			FillDescription( objDescription & );		// set the passed description to exactly match this object.
	
	bool			ExactMatch( objDescription *objectDescription );	// returns true if the passed description matches me exactly.  (ie: '3 beers' matches an object of '3 beers', but not of '4 beers'.)
	bool			PartialMatch( objDescription *objectDescription );	// returns true if the passed description matches any quantity of me.	(ie: '3 beers' matches an object of '3 beers', and also of '4 beers'.)
	
	bool			Blessed() { return (m_blesscurse == BC_Blessed); }
	bool			Cursed() { return (m_blesscurse == BC_Cursed); }
};



#endif //__OBJ_BASE_H__
