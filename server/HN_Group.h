#ifndef __HN_GROUP__
#define __HN_GROUP__

//  Group of players and monsters that are 'synched' together.

#include "HN_Point.h"

class hnPlayer;
class entBase;

#define MAX_MONSTERS_IN_GROUP	(64)

class hnGroup
{
	hnPlayer **		m_player;
	entBase *		m_monster[MAX_MONSTERS_IN_GROUP];
	int			m_monsterCount;
	int			m_playerCount;
	int			m_maxPlayerCount;

public:
				hnGroup( int maxPlayers );
				~hnGroup();
	
	int			DistanceFromGroup( hnPlayer * player );	// returns distance from player to this group,
									// not including the player as part of the group.
	int			DistanceFromGroup( const hnPoint & where );	// distance from this point to group.
	
	void			AddPlayer( hnPlayer * player );
	void			RemovePlayer( hnPlayer * player );
	void			RemoveEntity( entBase * entity );

	void			ScanForMonsters();			// search the map for nearby monsters to join us.
	
	int			GetPlayerCount() { return m_playerCount; }
	int			QueuedTurnCount();			// how many players with queued turns?
	
	bool			ProcessTurn();				// returns true if we actually ran a turn.
};

class hnGroupManager
{
	hnPlayer **		m_player;				// pointers to all players.  Note that this array is NOT
									// necessarily in the same order as the authoritative
									// one on the hnGame object!
	hnGroup **		m_group;
	int			m_maxGroupCount;
	
	static int		s_maxGroupDistanceSq;
	static hnGroupManager *	s_instance;

				hnGroupManager( int maxPlayers );
				~hnGroupManager();

	void			PutPlayerInGroup( int id );

	
public:
	static void		Startup( int maxPlayers );
	static void		Shutdown();
	
	static hnGroupManager *	GetInstance();

	static void		SetMaxGroupDistance(int dist) { s_maxGroupDistanceSq = dist * dist; }
	
	bool			ProcessTurn();	// somebody's submitted a turn.  Check, and if any group is ready, do them!  Return true if we actually processed a turn.
	void			UpdateGroups();

	void			AddPlayer( hnPlayer * player );
	void			RemovePlayer( hnPlayer * player );
	void			RemoveEntity( entBase * entity );
};

#endif // __HN_GROUP__
