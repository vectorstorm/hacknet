#ifndef __ENT_BASE_H__
#define __ENT_BASE_H__

#include "HN_Enum.h"
#include "HN_Point.h"
#include "ENT_Types.h"

#define MAX_NAME_BYTES (128)

class entBase
{
	entType			m_type;
	hnPoint			m_position;
	
	
protected:	
	char 			m_name[MAX_NAME_BYTES];
	bool			m_changedLevel;	// needs to refresh all our vision
	
public:
				entBase( entType type, const hnPoint & );
	virtual			~entBase();
	
	void			SetName( char * name );
	char *			GetName();
	
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

#endif // __ENT_BASE_H__
