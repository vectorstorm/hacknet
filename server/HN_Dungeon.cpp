#include "HN_Types.h"
#include "MAP_Base.h"
#include "MAP_Hack.h"
#include "HN_Dungeon.h"

#include "assert.h"

hnDungeon * hnDungeon::s_instance = NULL;

void
hnDungeon::Startup( int levelCount, int width, int height )
{
	assert(s_instance == NULL);

	s_instance = new hnDungeon( levelCount, width, height );
}

void
hnDungeon::Shutdown()
{
	assert(s_instance != NULL);

	delete s_instance;
	s_instance = NULL;
}

mapBase *
hnDungeon::GetLevel( int n )
{
	mapBase *result = NULL;
	
	assert(s_instance);
	
	if ( s_instance && n < s_instance->m_levelCount && n >= 0 )
		result = s_instance->m_levelMap[n];
	
	return result;
}

hnDungeon::hnDungeon(int levelCount, int width, int height):
	m_levelMap(NULL),
	m_levelCount(levelCount)
{
	m_levelMap = new (mapBase *)[levelCount];
	for ( int i = 0; i < levelCount; i++ )
	{
		// make Hack-style maps for now.
		m_levelMap[i] = new mapHack(width, height, i);
		m_levelMap[i]->Generate();

		// every map except for the bottom one has stairs down.
		if ( i < levelCount - 1 )
			m_levelMap[i]->GenerateStairsDown();
		
		// every map has stairs up.
		m_levelMap[i]->GenerateStairsUp();
	}
}

hnDungeon::~hnDungeon()
{
	for ( int i = 0; i < m_levelCount; i++ )
	{
		delete m_levelMap[i];
	}
	delete [] m_levelMap;
}


hnDungeon *
hnDungeon::GetInstance()
{
	assert( s_instance );

	return s_instance;
}
