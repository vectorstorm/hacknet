#include "OBJ_Ring.h"
#include "OBJ_Types.h"
#include "OBJ_Definition.h"
#include "OBJ_Manager.h"
#include <stdio.h>
#include <assert.h>

objRing::objRing(uint32 id) : 
	objBase(id)
{
	if ( !(m_flags & FLAG_Worn) )		// if we're not already worn..
		m_flags |= FLAG_Wearable;	// set us as wearable.
}

objRing::~objRing()
{
}

bool
objRing::SetWorn(bool worn)
{
	bool success = false;
	
	if ( FLAG_Worn )
	{
		bool alreadyWorn = ((m_flags & FLAG_Worn) != 0);
		
		if ( alreadyWorn != worn )
		{
			if ( worn )
			{
				m_flags |= FLAG_Worn;		// worn
				m_flags &= ~FLAG_Wearable;	// no longer wearable
			}
			else
			{
				m_flags &= ~FLAG_Worn;		// not worn
				m_flags |= FLAG_Wearable;	// wearable again
			}
			success = true;
		}
	}

	return success;
}
