#include "OBJ_Potion.h"
#include "OBJ_Definition.h"
#include "OBJ_Manager.h"
#include "HN_Player.h"
#include "HN_Properties.h"
#include "HN_Random.h"
#include <stdio.h>
#include <assert.h>

objPotion::objPotion(uint32 id) : 
	objBase(id),
	m_diluted(false)
{
}

objPotion::~objPotion()
{
}

bool
objPotion::Quaff(entBase *entity, hnPlayer *player)
{
	assert(entity);		// Well _somebody_ must be quaffing me!
	
	const objPrototype &proto = objManager::GetInstance()->GetPrototype( GetItemID() );
	char buffer[128] = "That was refreshing!";
	
	hnStatus *status = entity->GetStatus();
	
	switch( proto.property )
	{
		case PROP_Gain_Ability:
			if ( Cursed() )
				sprintf(buffer,"Ulch!  That potion tasted foul!");
			else
			{
				// TODO: adjust attributes
				sprintf(buffer,"Attributes increasing...");
			}
			break;
		case PROP_Restore_Ability:
			if ( Cursed() )
				sprintf(buffer,"Ulch!  This makes you feel mediocre!");
			else
			{
				sprintf(buffer,"Wow, this makes you feel %s!",Blessed()?"great":"good");
				// TODO: adjust attributes here.
			}
			break;
		case PROP_Paralyse:
			break;
		case PROP_Heal:
			sprintf(buffer,"You feel better.");
			status->Heal( 
				hnRandom::GetInstance()->Dice(6+2*BlessCurseSign(), 4), 
				!Cursed() ? 1 : 0, 	// extra points to raise max
				!Blessed(), !Cursed() 
				);
			status->ExerciseStatistic( hnStatus::Constitution, true );
			break;
		case PROP_Extra_Heal:
			sprintf(buffer,"You feel much better.");
			status->Heal( 
				hnRandom::GetInstance()->Dice(6+2*BlessCurseSign(), 8), 
				Blessed() ? 5 : (!Cursed() ? 2 : 0), 	// extra points to raise max
				!Blessed(), !Cursed() 
				);
			status->HallucinatingTime(0.0f);
			status->ExerciseStatistic( hnStatus::Strength, true );
			status->ExerciseStatistic( hnStatus::Constitution, true );
			break;
		case PROP_Full_Heal:
			sprintf(buffer,"You feel completely healed.");
			status->Heal(
				400, 
				4 + 4 * BlessCurseSign(),
				!Cursed(),				// heal sickness unless we're cursed
				true					// heal blindness
				);
			if ( Blessed() )
				status->RegainLevel();
			status->HallucinatingTime(0.0f);
			status->ExerciseStatistic( hnStatus::Strength, true );
			status->ExerciseStatistic( hnStatus::Constitution, true );
			break;
		case PROP_Gain_Level:
			break;
		case PROP_Gain_Energy:
			break;
		case PROP_Enlightenment:
			break;
		case PROP_Detect_Objects:
			break;
		case PROP_Sleep:
			break;
		case PROP_Booze:
			break;
		case PROP_Sickness:
			break;
	}
	





	
	if ( player )
	{
		// player did it, so send a message to him.
		player->Listen(buffer);
	}
	else
	{
		// TODO: monster quaffed us, so register a 'see' event.
	}

	m_count--;
	
	return true;
}
