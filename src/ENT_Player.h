#ifndef __ENT_PLAYER_H__
#define __ENT_PLAYER_H__

#include "ENT_Base.h"

class entPlayer : public entBase
{
	int		m_playerID;				// the ID the server assigns to us.  NOT TO BE SAVED.

	int		m_strength;
	int		m_dexterity;
	int		m_constitution;
	int		m_intelligence;
	int		m_wisdom;
	int		m_charisma;

	int		m_hitPointMax;
	int		m_hitPoints;

	int		m_spellPointMax;
	int		m_spellPoints;

	int		m_level;

	int		m_experiencePoints;
	
public:
			entPlayer( int playerID, const hnPoint & where );
			~entPlayer();
	
	virtual void	Move( hnDirection dir );
	virtual void	PostTurn();
};

#endif
