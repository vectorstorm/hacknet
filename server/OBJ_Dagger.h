#ifndef __OBJ_DAGGER_H__
#define __OBJ_DAGGER_H__

#include "OBJ_Base.h"

class objDagger : public objBase
{
	static char *	s_name;
public:
			objDagger( const hnPoint &where );
	virtual 	~objDagger();

	virtual const char *	GetName() { return s_name; }
};

#endif // __OBJ_DAGGER_H__
