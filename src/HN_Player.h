#ifndef __HN_PLAYER_H__
#define __HN_PLAYER_H__

#include "HN_Types.h"
#include "HN_Enum.h"
#include "HN_Point.h"

class mapBase;
class entBase;

struct queuedTurn
{
	enum turnType{
		None,
		Move,
		Wait
	};
	int type;

	union{
		struct{
			hnDirection direction;
		} move;
	};
};

class hnPlayer
{
	int		m_playerID;				// the ID the server assigns to us.  NOT TO BE SAVED.
	entBase *	m_entity;

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

	queuedTurn	m_queuedTurn;
	
public:
			hnPlayer( int playerID, const hnPoint & where );
	virtual		~hnPlayer();
	
	bool		HasQueuedTurn();	// are we ready to process a turn?
	virtual bool	IsValidMove( hnDirection dir );

	virtual void	Listen( const hnPoint & position, char * message );
	virtual void	Listen( char * message );
	
	const hnPoint &	GetPosition();
	
	virtual bool	CanSee( const hnPoint & position );
	
	void		SetName( char * name );
	char *		GetName();
	
	virtual void	DoTurn();	// run our turn now.
	virtual void	PostTurn();	// send results of our turn now.

	virtual void	RefreshMap( int level );

	
	//  Queued Actions Beneath This Point ------------------------------------------
	virtual void	Move( hnDirection dir );
	
};

#endif
