#include "OBJ_Manager.h"
#include "OBJ_Definition.h"
#include <stdio.h>

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
	
	for ( int i = 0; i < OBJ_TYPE_MAX; i++ )
		m_objectTotalProbability[i] = 0;

	printf("Object list debug information:\n---\n");
	for ( int i = 0; i < m_objectCount; i++ )
	{
		if ( m_objectDef[i].name )
			printf("%s  -  %d\n", m_objectDef[i].name, m_objectDef[i].probability);
		else
			printf("Unnamed - %d\n", m_objectDef[i].probability);
		m_objectTotalProbability[m_objectDef[i].type] += m_objectDef[i].probability;
	}

	for ( int i = 0; i < OBJ_TYPE_MAX; i++ )
	{
		printf("Item class %d - %d total.\n", i, m_objectTotalProbability[i]);
	}
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

struct itemConstructionProbability{
	int probability;
	objType type;
};

const struct itemConstructionProbability g_objProbs[] = {
	{10, OBJ_TYPE_Weapon},
	{10, OBJ_TYPE_Armour},
	{20, OBJ_TYPE_Food},
	{8, OBJ_TYPE_Tool},
	{8, OBJ_TYPE_Gem},
	{16, OBJ_TYPE_Potion},
	{16, OBJ_TYPE_Scroll},
	{4, OBJ_TYPE_Spellbook},
	{4, OBJ_TYPE_Wand},
	{3, OBJ_TYPE_Ring},
	{1, OBJ_TYPE_Amulet}
};		// probabilities MUST total 100!


objBase *
objManager::RandomItem(uint8 level, objType type)
{
	objBase *result = NULL;
	
	if ( type == OBJ_TYPE_Random )
	{
		int prob = rand() % 100;
		int entry = 0;
		bool done = false;

		while ( !done )
		{
			prob -= g_objProbs[entry].probability;
			if ( prob > 0 )
				entry++;
			else
				done = true;
		}
		type = g_objProbs[entry].type;
	}

	int value = rand() % m_objectTotalProbability[type];
	
	// walk through our list of objects until we find the one referred to by 'value'.

	for ( int i = 0; i < m_objectCount; i++ )
	{
		if ( m_objectDef[i].type == type )
		{
			if ( value < m_objectDef[i].probability )
			{
				// create this object
				result = new objWeapon(i);
				break;
			}
			value -= m_objectDef[i].probability;
		}
	}
	
	return result;
}
