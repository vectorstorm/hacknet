#include "OBJ_Manager.h"
#include "OBJ_Definition.h"

#include "OBJ_Weapon.h"

//---------  Temporary #includes while setting up new object system
//#include "OBJ_LongSword.h"
//#include "OBJ_Dagger.h"

#include <stdlib.h>
#include <assert.h>

objManager * objManager::s_instance = NULL;

extern objPrototype g_objectPrototype[];

objManager::objManager()
{
	assert(s_instance == NULL);
	s_instance = this;

	m_objectDef = g_objectPrototype;
	m_objectCount = GetDefinitionCount();

	m_objectTotalProbability = 0;

	for ( int i = 0; i < m_objectCount; i++ )
		m_objectTotalProbability += m_objectDef[i].probability;
}


objManager::~objManager()
{
	assert(s_instance == this);
	s_instance = NULL;

	m_objectDef = NULL;
}

void
objManager::Startup()
{
	assert(s_instance == NULL);
	new objManager;
}

void
objManager::Shutdown()
{
	assert(s_instance);
	delete s_instance;
}

objManager *
objManager::GetInstance()
{
	assert(s_instance);

	return s_instance;
}

const objPrototype &
objManager::GetPrototype(uint16 id)
{
	if ( id < m_objectCount )
		return m_objectDef[id];
	
	assert(0);
	return m_objectDef[0];
}

objBase *
objManager::RandomItem(uint8 level)
{
	objBase *result = NULL;
	int value = rand() % m_objectTotalProbability;
	
	// walk through our list of objects until we find the one referred to by 'value'.

	for ( int i = 0; i < m_objectCount; i++ )
	{
		if ( value < m_objectDef[i].probability )
		{
			// create this object
			result = new objWeapon(i);
			break;
		}
		value -= m_objectDef[i].probability;
	}
	
	return result;
}
