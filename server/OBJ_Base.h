#ifndef __OBJ_BASE_H__
#define __OBJ_BASE_H__

#include "HN_Types.h"
#include "HN_Point.h"
#include "OBJ_Types.h"

class objBase
{
	enum {
		BC_Uncursed,
		BC_Blessed,
		BC_Cursed
	};

	objType		m_type;
	hnPoint		m_position;
	
	uint8		m_blesscurse;

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
		
	
	bool		Blessed() { return (m_blesscurse == BC_Blessed); }
	bool		Cursed() { return (m_blesscurse == BC_Cursed); }
};



#endif //__OBJ_BASE_H__
