#include "OBJ_Registry.h"
#include "OBJ_Types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

objRegistry * objRegistry::s_instance = NULL;

objRegistry::objRegistry(uint16 objectCount):
	m_nameCount(objectCount)
{
	m_objectName = new (char *)[m_nameCount];
	m_objectType = new (objType)[m_nameCount];

	for( int i = 0; i < m_nameCount; i++ )
	{
		m_objectType[i] = Illegal;
		m_objectName[i] = NULL;
	}
}

objRegistry::~objRegistry()
{
	for( int i = 0; i < m_nameCount; i++ )
		delete [] m_objectName[i];
	delete [] m_objectName;
	delete [] m_objectType;

	m_nameCount = 0;
}

void
objRegistry::Startup(uint16 objectCount)
{
	assert( s_instance == NULL );

	s_instance = new objRegistry(objectCount);
}

void
objRegistry::Shutdown()
{
	assert( s_instance != NULL );

	delete s_instance;

	s_instance = NULL;
}

objRegistry *
objRegistry::GetInstance()
{
	assert( s_instance != NULL );

	return s_instance;
}

void
objRegistry::SetName(uint16 i, char *name)
{
	int length = strlen(name);

	delete [] m_objectName[i];
	m_objectName[i] = new char[length+1];
	strncpy(m_objectName[i],name,length+1);
}

char *
objRegistry::GetName(uint16 i)
{
	char * result = NULL;
	
	if ( i < m_nameCount )
		result = m_objectName[i];
	
	return result;
}

void
objRegistry::SetType(uint16 i, objType type)
{
	if ( i < m_nameCount )
		m_objectType[i] = type;
}


objType
objRegistry::GetType(uint16 i)
{
	objType result = Illegal;

	if ( i < m_nameCount )
		result = m_objectType[i];
	
	return result;
}

void
objRegistry::GetObjectDescriptionText(const objDescription &desc, char *buffer, uint16 bufferlength)
{
	const char * name = GetName(desc.type);
	
	if ( desc.count == 0 )
		buffer[0]='\0';
	else if ( desc.count == 1 )
	{
		if ( name[0] == 'a' || name[0] == 'i' || name[0] == 'u' ||
			name[0] == 'o' )
			snprintf(buffer, bufferlength, "an %s", name);
		else
			snprintf(buffer, bufferlength, "a %s", name);
	}
	else
	{
		snprintf(buffer, bufferlength, "%d %ss", desc.count, name);
	}
}
