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

	int			GetPlayerCount() { return m_playerCount; }

	void			ProcessTurn();
};

class hnGroupManager
{
	hnPlayer **		m_player;				// pointers to all players.  Note that this array is NOT
									// necessarily in the same order as the authoritative
									// one on the hnGame object!
	hnGroup **		m_group;
	int			m_maxGroupCount;
	
	static hnGroupManager *	s_instance;

				hnGroupManager( int maxPlayers );
				~hnGroupManager();

	void			PutPlayerInGroup( int id );

	
public:
	static void		Startup( int maxPlayers );
	static void		Shutdown();
	
	static hnGroupManager *	GetInstance();
	
	void			ProcessTurn();	// somebody's submitted a turn.  Check, and if any group is ready, do them!
	void			UpdateGroups();

	void			AddPlayer( hnPlayer * player );
	void			RemovePlayer( hnPlayer * player );
};

#endif // __HN_GROUP__
