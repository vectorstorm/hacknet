#ifndef HN_STATUS_H
#define HN_STATUS_H


#include "HN_Types.h"

class netMetaPacket;
class netMetaPacketInput;
class netMetaPacketOutput;

class hnStatus
{
public:
	
	enum{
		Strength,
		Dexterity,
		Constitution,
		Intelligence,
		Wisdom,
		Charisma,
		MAX_STATISTICS
	};
	typedef int statisticType;
private:
	uint8	m_statistic[MAX_STATISTICS];
	uint8	m_exercise[MAX_STATISTICS];

	sint16	m_hitPointMax;
	sint16	m_hitPoints;

	sint16	m_spellPointMax;
	sint16	m_spellPoints;

	uint16	m_experiencePoints;
	uint8	m_level;
	
	sint16	m_hunger;

	uint16	m_sick;
	uint16	m_blind;
	uint16	m_hallucinating;
	
	bool	m_confused;
	bool	m_stunned;
	bool	m_paralyzed;
	bool	m_polymorphed;
	bool	m_undead;	// are we a ghost/zombie/etc?

	bool	m_changedStatistics;
	bool	m_changedHitPoints;
	bool	m_changedSpellPoints;
	bool	m_changedExperiencePoints;

	void		Initialise();

public:	
			hnStatus();
			hnStatus( uint8 level );
	virtual		~hnStatus();
	
	
	uint8		GetStrength() { return m_statistic[Strength]; }
	uint8		GetDexterity() { return m_statistic[Dexterity]; }
	uint8		GetConstitution() { return m_statistic[Constitution]; }
	uint8		GetIntelligence() { return m_statistic[Intelligence]; }
	uint8		GetWisdom() { return m_statistic[Wisdom]; }
	uint8		GetCharisma() { return m_statistic[Charisma]; }
	uint8		GetStatistic(statisticType type) { return m_statistic[type]; }
	
	void		InitialiseHitPoints( uint16 points );
	sint16		HitPointMax() { return m_hitPointMax; }
	sint16		HitPoints() { return m_hitPoints; }

	void		InitialiseSpellPoints( uint16 points );
	sint16		SpellPointMax() { return m_spellPoints; }
	sint16		SpellPoints() { return m_spellPoints; }
	
	uint8		Level() { return m_level; }
	uint16		ExperiencePoints() { return m_experiencePoints; }
	
	bool		Alive();		// returns true if I'm not destroyed and I'm not undead.
	bool		Dead();			// returns true if I've been destroyed or if I'm undead.
	bool		Destroyed();		// returns true if I've been destroyed.
	bool		Confused() { return m_confused; }
	bool		Satiated();
	bool		Hungry();
	bool		Weak();
	bool		Fainting();
	bool		Polymorphed() { return m_polymorphed; }
	bool		Paralyzed() { return m_paralyzed; }

	void		Eat( sint16 nutrition );
	
	uint16		Hallucinating() { return m_hallucinating; }

	void		TakeDamage( sint16 points );
	void		Heal( uint32 points, uint32 extraPoints, bool cureSick, bool cureBlind );
	void		AdjustSpellPoints( uint32 points );
	void		AdjustSpellPointMax( uint32 points );

	void		ExerciseStatistic( statisticType type, bool up );
	bool		IncrementStatistic( statisticType type, bool up ); // return success
	uint16		HallucinatingTime( uint16 timer ) { m_hallucinating = timer; }
	void		RegainLevel();
	
	void		TurnPassed();	// a turn has passed.  Do whatever needs to be done.

	
	void		SendChanges( netMetaPacketOutput *packet );
	void		ReceiveChanges( netMetaPacketInput *packet );
};


#endif  // HN_STATUS_H
