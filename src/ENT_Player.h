#ifndef __ENT_PLAYER_H__
#define __ENT_PLAYER_H__

#include "ENT_Base.h"

class mapBase;

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
	
	mapBase **	m_map;
	int		m_mapCount;
public:
			entPlayer( int playerID, const hnPoint & where );
			~entPlayer();
	
	virtual void	Move( hnDirection dir );
	virtual void	Listen( const hnPoint & position, char * message );
	virtual void	Listen( char * message );

	virtual bool	CanSee( const hnPoint & position );
	
	virtual void	PostTurn();
};

#endif
