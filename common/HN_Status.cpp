
#include "HN_Status.h"

hnStatus::hnStatus()
{
	Initialise();
}

hnStatus::hnStatus( uint8 level )
{
	Initialise();
	m_level = level;

	// generate stats here based upon level..  and other info?
}

hnStatus::~hnStatus()
{
}

void
hnStatus::Initialise()
{
	m_strength = 3;
	m_dexterity = 3;
	m_constitution = 3;
	m_intelligence = 3;
	m_wisdom = 3;
	m_charisma = 3;

	m_hitPointMax = 1;
	m_hitPoints = 1;

	m_spellPointMax = 1;
	m_spellPoints = 1;

	m_level = 1;
	m_experiencePoints = 0;

	m_hunger = 0;

	m_confused = false;
	m_stunned = false;
	m_hallucinating = false;
	m_paralyzed = false;

	m_changedStatistics = true;
	m_changedHitPoints = true;
	m_changedSpellPoints = true;
}

bool
hnStatus::Dead()
{
	if ( Destroyed() )
		return true;
	if ( m_undead )
		return true;
	
	return false;
}

#define HUNGER_SATIATED ( 100 )
#define HUNGER_HUNGRY	( 200 )
#define HUNGER_WEAK	( 300 )
#define HUNGER_FAINTING	( 400 )
#define HUNGER_STARVED	( 500 )

bool
hnStatus::Destroyed()
{	
	if ( m_hitPoints <= 0 )
		return true;
	if ( m_strength < 3 )
		return true;
	if ( m_intelligence < 3 )
		return true;
	if ( m_hunger >= HUNGER_STARVED )
		return true;
	
	return false;
}


bool
hnStatus::Satiated()
{
	if ( m_hunger < HUNGER_SATIATED )
		return true;
	
	return false;
}

bool
hnStatus::Hungry()
{
	if ( m_hunger >= HUNGER_HUNGRY )
		return true;
	
	return false;
}

bool
hnStatus::Weak()
{
	if ( m_hunger >= HUNGER_WEAK )
		return true;
	
	return false;
}

bool
hnStatus::Fainting()
{
	if ( m_hunger >= HUNGER_FAINTING )
		return true;
	
	return false;
}

void
hnStatus::TakeDamage( sint16 pointsDamage )
{
	m_hitPoints -= pointsDamage;
}

void
hnStatus::TurnPassed()
{
	// make various status ailments wear off here..

	m_hunger++;
}
