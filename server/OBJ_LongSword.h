#ifndef __OBJ_LONGSWORD_H__
#define __OBJ_LONGSWORD_H__

#include "OBJ_Base.h"

class objLongSword : public objBase
{
	static char *	s_name;
public:
			objLongSword( const hnPoint &where );
	virtual 	~objLongSword();

	virtual sint16		RollDamage();

	virtual const char *	GetName() { return s_name; }
};

#endif // __OBJ_LONGSWORD_H__
