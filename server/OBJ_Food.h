#ifndef OBJ_FOOD_H
#define OBJ_FOOD_H

#include "OBJ_Base.h"

class objFood : public objBase
{
public:
			objFood(uint32 id);
			~objFood();
	
	virtual bool	Eat( entBase *entity, hnPlayer *player );
};

#endif // OBJ_AMULET_H
