
#include "HN_Status.h"
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
	m_strength = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1);
	m_dexterity = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1);
	m_intelligence = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1);
	m_constitution = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1);
	m_wisdom = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1);
	m_charisma = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1);

	m_hitPoints = level * ((rand() % 6) + 6);
	m_hitPointMax = m_hitPoints;

	m_spellPoints = level * ((rand() % 6) + 6);
	m_spellPointMax = m_spellPoints;
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
	m_changedHitPoints = true;
}

void
hnStatus::HealDamage( sint16 pointsDamage )
{
	m_hitPoints += pointsDamage;

	if ( m_hitPoints > m_hitPointMax )
		m_hitPoints = m_hitPointMax;
	
	m_changedHitPoints = true;
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

		stats.strength = Strength();
		stats.dexterity = Dexterity();
		stats.intelligence = Intelligence();
		stats.constitution = Constitution();
		stats.wisdom = Wisdom();
		stats.charisma = Charisma();
		
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
	
	switch( type )
	{
		case SPT_ClientStatistics:
			packet->ClientStatistics(stat);
			m_strength = stat.strength;
			m_dexterity = stat.dexterity;
			m_intelligence = stat.intelligence;
			m_constitution = stat.constitution;
			m_wisdom = stat.wisdom;
			m_charisma = stat.charisma;
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
