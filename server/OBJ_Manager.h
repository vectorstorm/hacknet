#ifndef __OBJ_MANAGER_H__
#define __OBJ_MANAGER_H__


#include "HN_Types.h"
#include "OBJ_Definition.h"

class objBase;


class objManager
{
	static objManager *	s_instance;
	
	objPrototype *		m_objectDef;
	uint16			m_objectCount;
	uint16			m_objectTotalProbability;

				objManager();
				~objManager();
public:
	static void		Startup();
	static void		Shutdown();

	static objManager *	GetInstance();

	uint16			GetObjectCount() {return m_objectCount;}

	const objPrototype &	GetPrototype(uint16 id);

	objBase *		RandomItem(uint8 level);
};


#endif //__OBJ_MANAGER_H__
