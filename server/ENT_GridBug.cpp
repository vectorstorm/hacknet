#include "HN_Random.h"
#include "ENT_Types.h"
#include "ENT_GridBug.h"
#include <assert.h>

char * entGridBug::s_name = "grid bug";

entGridBug::entGridBug(const hnPoint & where, hnPlayer *player):
	entBase( Entity_Xan, where, player )
{
	// gridbugs have 1 or 2 hit points.
	int points = hnRandom::GetInstance()->GetAndAdd( 2,  1 );
	
	GetStatus()->InitialiseHitPoints(points);
	
	//SetName("grid bug");
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
	
	switch( hnRandom::GetInstance()->Get(4) )
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
