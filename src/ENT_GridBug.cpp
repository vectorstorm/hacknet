#include <assert.h>
#include <stdlib.h>
#include "ENT_GridBug.h"


entGridBug::entGridBug(const hnPoint & where, bool hasPlayer):
	entBase( ENTITY_GridBug, where, hasPlayer )
{
}

entGridBug::~entGridBug()
{
}

void
entGridBug::Think()
{
	// by 'think', we're actually just going to move in a random
	// direction, for now.
	
	hnDirection dir;
	
	switch( rand() % 4 )
	{
		case 0:
			dir = DIR_North;
			break;
		case 1:
			dir = DIR_West;
			break;
		case 2:
			dir = DIR_East;
			break;
		case 3:
			dir = DIR_South;
			break;
		default:
			assert(0);
			break;
	}

	Move(dir);
}


//--------------------------------------------------------------------
// The functionality of the following few lines of code required
// code in three source files and six separate functions in NetHack.
// This is one of the big reasons I chose to reimplement HackNet
// from scratch, rather than try to shoehorn networking into NetHack's
// existing codebase.  Hooray for clean code!
//--------------------------------------------------------------------
bool
entGridBug::IsValidMove( hnDirection dir )
{
	if ( dir == DIR_NorthWest || dir == DIR_NorthEast ||
		dir == DIR_SouthWest || dir == DIR_SouthEast )
	{
		return false;
	}

	return entBase::IsValidMove( dir );
}
