#ifndef OBJ_POTION_H
#define OBJ_POTION_H

#include "OBJ_Base.h"

class objPotion : public objBase
{	
	bool		m_diluted;
	
public:
			objPotion(uint32 id);
	virtual		~objPotion();

	bool		Quaff(entBase *entity, hnPlayer *player);
};


#endif // OBJ_POTION_H
