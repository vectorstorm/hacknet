#ifndef __HN_ENTITY_H__
#define __HN_ENTITY_H__

#include "HN_Enum.h"
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
	entType	m_type;
	hnPoint		m_position;
	
public:
				entBase( entType type, const hnPoint & );
	virtual			~entBase();
	
	const hnPoint &		GetPosition();
	entType			GetType() { return m_type; }
	
	void			SetPosition( const hnPoint & );
	
	virtual void		Think() {}			// Called to decide what to do each turn.
	virtual void		Move( hnDirection direction );
	
	virtual void		PreTurn() {}			// Called before processing a turn
	virtual void		PostTurn() {}			// Called after processing a turn
	
	//virtual void		Move( const hnPoint & offset ) {}
	//virtual void		MoveTo( const hnPoint & position ) {}
	
protected:
	
	void			Unlink();
};

#endif
