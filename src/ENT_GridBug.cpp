#include "ENT_GridBug.h"


entGridBug::entGridBug(const hnPoint & where):
	entBase( ENTITY_GridBug, where )
{
}

entGridBug::~entGridBug()
{
}

void
entGridBug::Think()
{
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
