#ifndef HN_RANDOM_H
#define HN_RANDOM_H

#include "HN_Types.h"

class hnRandom
{
	static hnRandom *	s_instance;
	
				hnRandom( int randomSeed );
				~hnRandom();
public:
	static hnRandom *	GetInstance();
		
	static void		Startup( int randomSeed );
	static void		Shutdown();
	
	uint32			Get( uint32 max );			// returns random number in [0..max-1]
	uint32			GetAndAdd( uint32 max, uint32 add );	// returns sum of 'add' and random number in [0..max-1]
	uint32			GetRange( uint32 min, uint32 max );	// returns random number in [min..max-1]

	uint32			Dice( uint32 count, uint32 sides );	// roll count sides-sided dice and return result.
};

#endif // HN_RANDOM_H
