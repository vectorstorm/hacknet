#ifndef __HN_STATUS_H__

#include "HN_Types.h"

class hnStatus
{
	uint8	m_strength;
	uint8	m_dexterity;
	uint8	m_constitution;
	uint8	m_intelligence;
	uint8	m_wisdom;
	uint8	m_charisma;

	sint16	m_hitPointMax;
	sint16	m_hitPoints;

	sint16	m_spellPointMax;
	sint16	m_spellPoints;

	uint8	m_level;
	uint16	m_experiencePoints;
	
	sint16	m_hunger;
	
	bool	m_confused;
	bool	m_stunned;
	bool	m_hallucinating;
	bool	m_paralyzed;
	bool	m_undead;	// are we a ghost/zombie/etc?

	bool	m_changedStatistics;
	bool	m_changedHitPoints;
	bool	m_changedSpellPoints;

	void		Initialise();

public:	
			hnStatus();
			hnStatus( uint8 level );
	virtual		~hnStatus();
	
	
	uint8		Strength() { return m_strength; }
	uint8		Dexterity() { return m_dexterity; }
	uint8		Constitution() { return m_constitution; }
	uint8		Intelligence() { return m_intelligence; }
	uint8		Wisdom() { return m_wisdom; }
	uint8		Charisma() { return m_charisma; }
	
	sint16		HitPointMax() { return m_hitPointMax; }
	sint16		HitPoints() { return m_hitPoints; }

	sint16		SpellPointMax() { return m_spellPoints; }
	sint16		SpellPoints() { return m_spellPoints; }
	
	uint8		Level() { return m_level; }
	uint16		ExperiencePoints() { return m_experiencePoints; }
	
	bool		Dead();			// returns true if I've been destroyed or if I'm undead.
	bool		Destroyed();		// returns true if I've been destroyed.
	bool		Confused() { return m_confused; }
	bool		Satiated();
	bool		Hungry();
	bool		Weak();
	bool		Fainting();
	bool		Hallucinating() { return m_hallucinating; }
	bool		Paralyzed() { return m_paralyzed; }

	bool		ChangedStatistics() { return m_changedStatistics; }
	bool		ChangedHitPoints() { return m_changedHitPoints; }
	bool		ChangedSpellPoints() { return m_changedSpellPoints; }
	
	void		TakeDamage( sint16 points );
	void		TurnPassed();	// a turn has passed.  Do whatever needs to be done.
	
	void		ResetChanged();	// reset our 'changed' flags.
};


#endif
