#ifndef __ENT_PLAYER_H__
#define __ENT_PLAYER_H__

#include "ENT_Base.h"

class entPlayer : public entBase
{
public:
		entPlayer( const hnPoint & where );
		~entPlayer();
};

#endif
