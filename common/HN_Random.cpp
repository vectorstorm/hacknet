#include "HN_Random.h"
#include <stdlib.h>
#include <assert.h>

hnRandom * hnRandom::s_instance = NULL;

hnRandom::hnRandom( int seed )
{
	srand(seed);
}

hnRandom::~hnRandom()
{
}

void
hnRandom::Startup( int seed )
{
	assert( s_instance == NULL );
	
	s_instance = new hnRandom(seed);
}

void
hnRandom::Shutdown()
{
	assert( s_instance );

	delete s_instance;
	s_instance = NULL;
}

hnRandom *
hnRandom::GetInstance()
{
	assert( s_instance );

	return s_instance;
}

uint32
hnRandom::Get(uint32 max)
{
	return (rand() % max);
}

uint32
hnRandom::GetAndAdd(uint32 max, uint32 add)
{
	return (Get(max) + add);
}

uint32
hnRandom::GetRange(uint32 min, uint32 max)
{
	uint32 diff = max - min;

	return Get(diff) + min;
}

uint32
hnRandom::Dice(uint32 count, uint32 sides)
{
	int result = 0;
	
	for ( int i = 0; i < count; i++ )
		result += Get(sides) + 1;

	return result;
}
