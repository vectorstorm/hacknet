#ifndef __OBJ_BASE_H__
#define __OBJ_BASE_H__

#include "OBJ_Types.h"
#include "HN_Types.h"
#include "HN_Point.h"

class entBase;

class objBase
{
	uint32		m_type;		// what type of object are we? (weapon, etc)
	uint32		m_itemID;	// what are we, exactly?
	hnPoint		m_position;
	
	uint8		m_blesscurse;
	uint8		m_count;	// how many of us are there?


	objBase *	m_next;
	objBase *	m_prev;
	
protected:
	void		Unlink();	
public:
				objBase( uint32 type );
	virtual 		~objBase();

	virtual sint16		RollDamage( entBase * );
	
        const hnPoint &		GetPosition();
	void			SetPosition( const hnPoint & );
	uint32			GetType() { return m_type; }
	uint32			GetItemID() { return m_itemID; }

	const char *		GetName();
	
        void			AddObject(objBase *object);     // add us into our circular linked list
	void			RemoveObject(objBase *object);  // find this object in our circular linked list and remove it
	//objBase *		RemoveObjectDescription( const objDescription &objectDescription, uint8 inventorySlot );	// match the description against one or more objects in our list and remove them, returning the removed object(s).
	objBase *		RemoveObjectQuantity( objBase *object, uint8 count );	// match the description against one or more objects in our list and remove them, returning the removed object(s).

	int			ObjectCount();
	objBase *		GetObject(int id);
	int			GetObjectID(objBase *object);
	void			GetDescription(objDescription &result, int id);

	void			FillDescription( objDescription & );		// set the passed description to exactly match this object.
	
	bool			ExactMatch( const objDescription &objectDescription );	// returns true if the passed description matches me exactly.  (ie: '3 beers' matches an object of '3 beers', but not of '4 beers'.)
	bool			PartialMatch( const objDescription &objectDescription );	// returns true if the passed description matches any quantity of me.	(ie: '3 beers' matches an object of '3 beers', and also of '4 beers'.)
	
	bool			Blessed() { return (m_blesscurse == BC_Blessed); }
	bool			Cursed() { return (m_blesscurse == BC_Cursed); }
};



#endif //__OBJ_BASE_H__
