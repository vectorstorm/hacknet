#ifndef __OBJ_REGISTRY_H__
#define __OBJ_REGISTRY_H__

#include "HN_Types.h"
#include "OBJ_Types.h"

class objRegistry
{
	char **			m_objectName;
	objType *		m_objectType;
	uint16			m_nameCount;

	static objRegistry	*s_instance;

				objRegistry(uint16 objectCount);
				~objRegistry();
public:
	static void		Startup(uint16 objectCount);
	static void		Shutdown();

	static objRegistry *	GetInstance();
	
	void			SetName(uint16 i, char *name);
	void			GetName(uint16 i, char *namebuffer, int bufferLength);
	void			SetType(uint16 i, objType type);
	objType			GetType(uint16 i);

	void			GetObjectDescriptionText(const objDescription &, char *buffer, uint16 bufferLength);
};

#endif //__OBJ_REGISTRY_H__

