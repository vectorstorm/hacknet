#include "OBJ_Armour.h"
#include "OBJ_Types.h"
#include "OBJ_Definition.h"
#include "OBJ_Manager.h"
#include <stdio.h>
#include <assert.h>

objArmour::objArmour(uint32 id) : 
	objBase(id),
	m_wearFlag(0)
{
	const objPrototype &proto = objManager::GetInstance()->GetPrototype( GetItemID() );
	
	switch ( proto.subtype )
	{
		case ARM_Suit:
			m_wearFlag = FLAG_Worn_Suit;
			break;
		case ARM_Helm:
			m_wearFlag = FLAG_Worn_Helm;
			break;
		case ARM_Shield:
			m_wearFlag = FLAG_Worn_Shield;
			break;
		case ARM_Gloves:
			m_wearFlag = FLAG_Worn_Gloves;
			break;
		case ARM_Boots:
			m_wearFlag = FLAG_Worn_Boots;
			break;
		case ARM_Cloak:
			m_wearFlag = FLAG_Worn_Cloak;
			break;
		case ARM_Shirt:
			m_wearFlag = FLAG_Worn_Shirt;
			break;
		default:
			printf("Error:  Armour created with id %d and unknown subtype %d!\n", GetItemID(), proto.subtype );
			assert(0);
	}
	
	if ( !(m_flags & m_wearFlag) )		// if we're not already worn..
		m_flags |= FLAG_Wearable;	// set us as wearable.
}

objArmour::~objArmour()
{
}

bool
objArmour::SetWorn(bool worn)
{
	bool success = false;
	
	if ( m_wearFlag )
	{
		bool alreadyWorn = ((m_flags & m_wearFlag) != 0);
		
		if ( alreadyWorn != worn )
		{
			if ( worn )
			{
				m_flags |= m_wearFlag;		// worn
				m_flags &= ~FLAG_Wearable;	// no longer wearable
			}
			else
			{
				m_flags &= ~m_wearFlag;		// not worn
				m_flags |= FLAG_Wearable;	// wearable again
			}
			success = true;
		}
	}

	return success;
}
