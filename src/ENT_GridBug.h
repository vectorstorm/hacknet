#ifndef __ENT_GRIDBUG_H__
#define __ENT_GRIDBUG_H__

#include "ENT_Base.h"

class entGridBug : public entBase
{
public:
				entGridBug( const hnPoint & );
	virtual			~entGridBug();
	
	virtual void		Think(); 			// Called to decide what to do each turn.
	virtual bool		IsValidMove( hnDirection dir );
};

#endif // __ENT_GRIDBUG_H__
