#ifndef __ENT_GRIDBUG_H__
#define __ENT_GRIDBUG_H__

#include "ENT_Base.h"

class entGridBug : public entBase
{
	static char		*s_name;
public:
				entGridBug( const hnPoint &, hnPlayer * player = NULL );
	virtual			~entGridBug();
	
	virtual const char *	GetName() { return s_name; }
	
	virtual void		Think(); 			// Called to decide what to do each turn.
	virtual bool		IsValidMove( hnDirection dir );
};

#endif // __ENT_GRIDBUG_H__
