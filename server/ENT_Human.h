#ifndef __ENT_HUMAN_H__
#define __ENT_HUMAN_H__

#include "ENT_Base.h"

class entHuman : public entBase
{
	static char *		s_name;
public:
				entHuman( const hnPoint &, hnPlayer *player = NULL );
	virtual			~entHuman();
	
	virtual const char *	GetName() { return s_name; }
	
	virtual void		Think(); 			// Called to decide what to do each turn.
};

#endif // __ENT_GRIDBUG_H__
