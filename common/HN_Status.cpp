
#include "HN_Status.h"
#include "HN_Random.h"
#include "NET_Packet.h"
#include <stdlib.h>

hnStatus::hnStatus()
{
	Initialise();
}

hnStatus::hnStatus( uint8 level )
{
	Initialise();
	m_level = level;

	// generate stats here based upon level..  and other info?
	
	for ( int i = 0; i < MAX_STATISTICS; i++ )
		m_statistic[i] = hnRandom::GetInstance()->Dice(3,6);

	m_hitPoints = level * hnRandom::GetInstance()->GetRange(7,13);
	m_hitPointMax = m_hitPoints;

	m_spellPoints = level * hnRandom::GetInstance()->GetRange(7,13);
	m_spellPointMax = m_spellPoints;
}

hnStatus::~hnStatus()
{
}

void
hnStatus::Initialise()
{
	for ( int i = 0; i < MAX_STATISTICS; i++ )
	{
		m_statistic[i] = 3;
		m_exercise[i] = 0;
	}
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
hnStatus::Alive()
{
	return !Dead();
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
	if ( GetStrength() < 3 )
		return true;
	if ( GetIntelligence() < 3 )
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
hnStatus::InitialiseHitPoints( uint16 points )
{
	m_hitPoints = points;
	m_hitPointMax = points;
	m_changedHitPoints = true;
}

void
hnStatus::TakeDamage( sint16 pointsDamage )
{
	m_hitPoints -= pointsDamage;
	m_changedHitPoints = true;
}

void
hnStatus::Heal( uint32 pointsHealed, uint32 extraPoints, bool cureSick, bool cureBlind )
{
	if ( pointsHealed )
	{
		m_hitPoints += pointsHealed;
		if ( m_hitPoints > m_hitPointMax )	// if we're over max...
			m_hitPointMax += extraPoints;	// then add a few points to max hit points
	}
	
	if ( cureBlind )
		m_blind = 0;
	if ( cureSick )
		m_sick = 0;
	
	if ( m_hitPoints > m_hitPointMax )		// clamp our health to our max health.
		m_hitPoints = m_hitPointMax;
	
	m_changedHitPoints = true;
}

#define AVAL		(50) // tune value for attribute exercise

void
hnStatus::ExerciseStatistic( statisticType type, bool up )
{
	// can't exercise intelligence or charisma.
	if ( type == Intelligence || type == Charisma )
		return;
	
	// don't exercise physical stats if we're polymorphed.
	if ( Polymorphed() && type != Wisdom )
		return;

	if ( abs( m_exercise[type] ) < AVAL )
	{
		if ( up && hnRandom::GetInstance()->Get(19) > m_statistic[type] )
			m_exercise[type] ++;
		else if ( !up )
			m_exercise[type] -= hnRandom::GetInstance()->Get(2);
	}
}

void
hnStatus::RegainLevel()
{
}

void
hnStatus::TurnPassed()
{
	// make various status ailments wear off here..

	m_hunger++;
}

void
hnStatus::SendChanges( netMetaPacketOutput * packet )
{
	if ( m_changedStatistics )
	{
		netClientStatistics stats;
		
		for ( int i = 0; i < MAX_STATISTICS; i++ )
			stats.statistic[i] = GetStatistic(i);
		
		packet->ClientStatistics(stats);
		
		m_changedStatistics = false;
	}
	if ( m_changedHitPoints )
	{
		netClientHitPoints hp;

		hp.hitPoints = m_hitPoints;
		hp.maxHitPoints = m_hitPointMax;

		packet->ClientHitPoints(hp);
		
		m_changedHitPoints = false;
	}
	if ( m_changedSpellPoints )
	{
		netClientSpellPoints sp;

		sp.spellPoints = m_spellPoints;
		sp.maxSpellPoints = m_spellPointMax;

		packet->ClientSpellPoints(sp);
		
		m_changedSpellPoints = false;
	}
	if ( m_changedExperiencePoints )
	{
		netClientExperience ep;

		ep.experiencePoints = m_experiencePoints;
		ep.level = m_level;

		packet->ClientExperience(ep);

		m_changedExperiencePoints = false;
	}
}

void
hnStatus::ReceiveChanges( netMetaPacketInput * packet )
{
	sint8 type = packet->PeekSint8();
	
	netClientStatistics stat;
	netClientHitPoints hp;
	netClientSpellPoints sp;
	netClientExperience ep;
	int i;
	
	switch( type )
	{
		case SPT_ClientStatistics:
			packet->ClientStatistics(stat);
			for ( i = 0; i < MAX_STATISTICS; i++ )
				m_statistic[i] = stat.statistic[i];
			break;
		case SPT_ClientHitPoints:
			packet->ClientHitPoints(hp);
			m_hitPoints = hp.hitPoints;
			m_hitPointMax = hp.maxHitPoints;
			break;
		case SPT_ClientSpellPoints:
			packet->ClientSpellPoints(sp);
			m_spellPoints = sp.spellPoints;
			m_spellPointMax = sp.maxSpellPoints;
			break;
		case SPT_ClientExperience:
			packet->ClientExperience(ep);
			m_experiencePoints = ep.experiencePoints;
			m_level = ep.level;
			break;
	}
}
