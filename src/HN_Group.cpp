#include <assert.h>
#include "HN_Group.h"
#include "HN_Player.h"


hnGroupManager * hnGroupManager::s_instance = NULL;

hnGroup::hnGroup( int maxPlayers ):
	m_player(NULL),
	m_playerCount(0),
	m_maxPlayerCount(maxPlayers)
{
	m_player = new (hnPlayer *)[m_maxPlayerCount];

	for ( int i = 0; i < m_maxPlayerCount; i++ )
		m_player[i] = NULL;
}

hnGroup::~hnGroup()
{
	delete [] m_player;
}

int
hnGroup::DistanceFromGroup( hnPlayer * player )
{
	int distance = 0;
	
	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] != player )
		{
			hnPoint offset = m_player[i]->GetPosition() - player->GetPosition();
			distance += offset.x + offset.y;	// this isn't correct, but it's fast.  TODO: Fix!
		}
	}
	
	return distance;
}


void
hnGroup::AddPlayer( hnPlayer * player )
{
	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] == player )
		{
			assert(0);
			return;
		}
	}

	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] == NULL )
		{
			m_player[i] = player;
			return;
		}
	}
	// we should never get here!
	assert(0);
}

void
hnGroup::RemovePlayer( hnPlayer * player )
{
	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] == player )
			m_player[i] = NULL;
	}
}

//---------------------------------------------------------

void
hnGroupManager::Startup( int maxPlayers )
{
	assert( s_instance == NULL );

	s_instance = new hnGroupManager( maxPlayers );
}

void
hnGroupManager::Shutdown()
{
	assert( s_instance != NULL );

	delete s_instance;
	s_instance = NULL;
}

hnGroupManager *
hnGroupManager::GetInstance()
{
	assert( s_instance != NULL );
	return s_instance;
}

hnGroupManager::hnGroupManager( int maxPlayers ):
	m_group(NULL),
	m_maxGroupCount(maxPlayers)
{
	m_group = new (hnGroup *)[m_maxGroupCount];
	
	for ( int i = 0; i < m_maxGroupCount; i++ )
		m_group[i] = new hnGroup(maxPlayers);
}

hnGroupManager::~hnGroupManager()
{
	for ( int i = 0; i < m_maxGroupCount; i++ )
		delete m_group[i];
	delete [] m_group;
}

void
hnGroupManager::ProcessTurn()
{
	// check each group to see if they're ready to run a turn, and do so if they are.
}

void
hnGroupManager::UpdateGroups()
{
	// reassign players to groups.
}
