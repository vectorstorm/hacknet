#ifndef __ENT_HUMAN_H__
#define __ENT_HUMAN_H__

#include "ENT_Base.h"

class entHuman : public entBase
{
public:
				entHuman( const hnPoint &, hnPlayer *player = NULL );
	virtual			~entHuman();
	
	virtual void		Think(); 			// Called to decide what to do each turn.
};

#endif // __ENT_GRIDBUG_H__
