#ifndef __HN_OBJECT_H__
#define __HN_OBJECT_H__

#include "HN_Types.h"
#include "HN_Point.h"

enum
{
	OBJECT_None,
	
	OBJECT_Potion,		// there should be one of these for each item type.
	OBJECT_Armor,
	OBJECT_Weapon,

	OBJECT_MAX
};
typedef uint8 objType;

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



#endif //__HN_OBJECT_H__
