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
		m_objectType[i] = OBJ_TYPE_Illegal;
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
	if ( i < m_nameCount )
	{
		int length = strlen(name);

		delete [] m_objectName[i];
		m_objectName[i] = new char[length+1];
		strncpy(m_objectName[i],name,length+1);
	}
}

void
objRegistry::GetName(uint16 i, char *result, int bufferLength)
{
	if ( i < m_nameCount )
	{
		snprintf(result, bufferLength, "%s", m_objectName[i]);
	}
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
	objType result = OBJ_TYPE_Illegal;

	if ( i < m_nameCount )
		result = m_objectType[i];
	
	return result;
}

void
objRegistry::GetObjectDescriptionText(const objDescription &desc, char *buffer, uint16 bufferlength)
{
#define NAMEBUFFER_LEN (64)
	char name[NAMEBUFFER_LEN] = "";
	char adjective[NAMEBUFFER_LEN] = "";
	char scrap[NAMEBUFFER_LEN] = "";
	objType type = m_objectType[desc.itemID];
	
	GetName(desc.itemID, scrap, NAMEBUFFER_LEN);

	switch( type )
	{
		case OBJ_TYPE_Potion:
			snprintf(name, NAMEBUFFER_LEN, "potion");
			snprintf(adjective, NAMEBUFFER_LEN, " of %s", scrap );
			break;
		case OBJ_TYPE_Scroll:
			snprintf(name, NAMEBUFFER_LEN, "scroll");
			snprintf(adjective, NAMEBUFFER_LEN, " of %s", scrap );
			break;
		case OBJ_TYPE_Wand:
			snprintf(name, NAMEBUFFER_LEN, "wand");
			snprintf(adjective, NAMEBUFFER_LEN, " of %s", scrap );
			break;
		case OBJ_TYPE_Spellbook:
			snprintf(name, NAMEBUFFER_LEN, "spellbook");
			snprintf(adjective, NAMEBUFFER_LEN, " of %s", scrap );
			break;
		case OBJ_TYPE_Ring:
			snprintf(name, NAMEBUFFER_LEN, "ring");
			snprintf(adjective, NAMEBUFFER_LEN, " of %s", scrap );
			break;
		default:
			name = scrap;
			break;
	}
	
	if ( desc.count == 0 )
		buffer[0]='\0';
	else if ( desc.count == 1 )
	{
		if ( name[0] == 'a' || name[0] == 'i' || name[0] == 'u' ||
			name[0] == 'o' )
			snprintf(buffer, bufferlength, "an %s%s", name, adjective);
		else
			snprintf(buffer, bufferlength, "a %s%s", name, adjective);
	}
	else
	{
		snprintf(buffer, bufferlength, "%d %ss%s", desc.count, name, adjective);
	}
}
