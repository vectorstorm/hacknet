#include <assert.h>
#include <stdio.h>
#include "HN_Group.h"
#include "HN_Player.h"
#include "NET_Server.h"

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

void
hnGroup::ProcessTurn()
{
	if ( m_playerCount > 0 )
	{	
		bool everyoneHasATurn = true;
	
		for ( int i = 0; i < m_maxPlayerCount; i++ )
			if ( m_player[i] != NULL )
				if ( !m_player[i]->HasQueuedTurn() )
					everyoneHasATurn = false;
		
		if ( everyoneHasATurn )
		{
			for ( int i = 0; i < m_maxPlayerCount; i++ )
				if ( m_player[i] )
					m_player[i]->DoTurn();
			
			for ( int i = 0; i < m_maxPlayerCount; i++ )
				if ( m_player[i] )
					m_player[i]->PostTurn();
		}
	}
}

int
hnGroup::DistanceFromGroup( hnPlayer * player )
{
	int distance = 0;	// return -1 if not eligible to enter group.
	bool firstTest = true;
	
	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] != NULL && m_player[i] != player )
		{
			hnPoint offset = m_player[i]->GetPosition() - player->GetPosition();
			
			if ( firstTest && offset.z != 0 )	// if we're not on the same level as the first guy
				return -1;			// in this group, we're not allowed in this group.
			
			firstTest = false;
			distance += (offset.x*offset.x) + (offset.y*offset.y);	// this isn't correct, technically.
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
			m_playerCount++;
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
		{
			m_player[i] = NULL;
			m_playerCount--;
		}
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

	m_player = new (hnPlayer *)[m_maxGroupCount];
	for ( int i = 0; i < m_maxGroupCount; i++ )
		m_player[i] = NULL;
}

hnGroupManager::~hnGroupManager()
{
	for ( int i = 0; i < m_maxGroupCount; i++ )
		delete m_group[i];
	delete [] m_group;

	delete [] m_player;
}

void
hnGroupManager::AddPlayer( hnPlayer * player )
{
	// add player into our list, if he's not already there.
	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		if ( m_player[i] == player )
		{
			assert(0);
			return;
		}
	}

	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		if ( m_player[i] == NULL )
		{
			m_player[i] = player;

			// now put the player into a group.
			PutPlayerInGroup(i);
			return;
		}
	}
	assert( 0 );
}

#define GROUP_DISTANCE_LEEWAY (100)

void
hnGroupManager::PutPlayerInGroup(int id)
{
	hnPlayer *player = m_player[id];

	int minDistance = 20000;	// arbitrary large integer
	int minGroupID = -1;
	int distance;

	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		m_group[i]->RemovePlayer(player);
		
		if ( m_group[i]->GetPlayerCount() > 0 )
		{
			distance = m_group[i]->DistanceFromGroup(player);

			if ( distance > 0 && distance < minDistance )	// if we're allowed to join this group...
			{						// and it's a better match than anything else...
				minDistance = distance;
				minGroupID = i;
			}
		}
	}

	if ( minDistance < GROUP_DISTANCE_LEEWAY )
	{
		char buffer[128];
		sprintf(buffer, "You're sharing group %d.", minGroupID);
		netServer::GetInstance()->StartMetaPacket( id );
		netServer::GetInstance()->SendMessage(buffer);
		netServer::GetInstance()->TransmitMetaPacket();

		m_group[minGroupID]->AddPlayer(player);
		return;
	}
	else	// no better group for me, so put me in a group all by myself.
	{
		for ( int i = 0; i < m_maxGroupCount; i++ )
		{
			if ( m_group[i]->GetPlayerCount() == 0 )
			{
				char buffer[128];
		sprintf(buffer, "You're in group %d.", i);
		netServer::GetInstance()->StartMetaPacket( id );
		netServer::GetInstance()->SendMessage(buffer);
		netServer::GetInstance()->TransmitMetaPacket();

				m_group[i]->AddPlayer( player );
				return;
			}
		}
	}
	assert(0); // we should never get here!
}

void
hnGroupManager::RemovePlayer( hnPlayer *player )
{
	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		m_group[i]->RemovePlayer(player);
	}
	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		if ( m_player[i] == player )
			m_player[i] = NULL;
	}
}

void
hnGroupManager::ProcessTurn()
{
	// check each group to see if they're ready to run a turn, and do so if they are.

	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		m_group[i]->ProcessTurn();
	}
}

void
hnGroupManager::UpdateGroups()
{
	// reassign players to groups.
	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		if ( m_player[i] != NULL )
		{
			PutPlayerInGroup(i);
		}
	}
}
