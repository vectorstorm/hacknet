#ifndef __HN_PLAYER_H__
#define __HN_PLAYER_H__

#include "HN_Types.h"
#include "HN_Enum.h"
#include "HN_Point.h"

class mapBase;
class entBase;
class hnGroup;

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
	
	mapBase **	m_map;
	int		m_mapCount;
	
	hnGroup *	m_group;				// pointer to the group we're a part of.
	
	queuedTurn	m_queuedTurn;
	
	int		m_lastSentGroupPlayerCount;
	int		m_lastSentGroupPlayerQueuedTurns;
	bool		m_statsChanged;
	bool		m_hitPointsChanged;
	bool		m_spellPointsChanged;
	
public:
			hnPlayer( int playerID, const hnPoint & where );
	virtual		~hnPlayer();
	
	void		SetName( char * name );
	void		SetGroup( hnGroup * group ) { m_group = group; }
	
	const hnPoint &	GetPosition();
	int		GetID() { return m_playerID; }
	char *		GetName();

	
	bool		HasQueuedTurn();	// are we ready to process a turn?
	virtual bool	IsValidMove( hnDirection dir );

	virtual void	Listen( const hnPoint & position, char * message );
	virtual void	Listen( char * message );
	
	virtual bool	CanSee( const hnPoint & position );
	
	virtual void	DoTurn();		// run our turn now.
	virtual void	RecalculateVision();	// calculate what we can see
	virtual void	UpdateVision();		// check again what's in the squares we previously calculated we could see.
	virtual void	SendUpdate();		// send all updates to client.

	virtual void	RefreshMap( int level );
	
	
	//  Queued Actions Beneath This Point ------------------------------------------
	virtual void	Move( hnDirection dir );
	virtual void	Wait( );
	
};

#endif