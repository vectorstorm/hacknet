#ifndef __HN_GROUP__
#define __HN_GROUP__

//  Group of players and monsters that are 'synched' together.

class hnPlayer;

class hnGroup
{
	hnPlayer **		m_player;
	int			m_playerCount;
	int			m_maxPlayerCount;

public:
				hnGroup( int maxPlayers );
				~hnGroup();
	
	int			DistanceFromGroup( hnPlayer * player );	// returns distance from player to this group,
									// not including the player as part of the group.
	void			AddPlayer( hnPlayer * player );
	void			RemovePlayer( hnPlayer * player );
};

class hnGroupManager
{
	hnGroup **		m_group;
	int			m_maxGroupCount;
	
				hnGroupManager( int maxPlayers );
				~hnGroupManager();
	
	static hnGroupManager *	s_instance;
public:
	static void		Startup( int maxPlayers );
	static void		Shutdown();
	
	hnGroupManager *	GetInstance();
	
	void			ProcessTurn();	// somebody's submitted a turn.  Check, and if any group is ready, do them!
	void			UpdateGroups();
};

#endif // __HN_GROUP__
