#include <assert.h>
#include <stdio.h>
#include "HN_Group.h"
#include "HN_Dungeon.h"
#include "HN_Player.h"
#include "NET_Server.h"
#include "ENT_Base.h"
#include "MAP_Base.h"

#define GROUP_DISTANCE_LEEWAY 		(15)
#define GROUP_DISTANCE_LEEWAY_SQ 	(GROUP_DISTANCE_LEEWAY * GROUP_DISTANCE_LEEWAY)

hnGroupManager * hnGroupManager::s_instance = NULL;
int		hnGroupManager::s_maxGroupDistanceSq = GROUP_DISTANCE_LEEWAY_SQ;

hnGroup::hnGroup( int maxPlayers ):
	m_player(NULL),
	m_playerCount(0),
	m_maxPlayerCount(maxPlayers)
{
	//---------------------------------------------------------
	//  Prepare an array of player pointers, to store members
	//  of our group.  WE DO NOT OWN THESE POINTERS, SO WE DO
	//  NOT DELETE THEM IN OUR DESTRUCTOR.
	//---------------------------------------------------------
	
	m_player = new (hnPlayer *)[m_maxPlayerCount];

	for ( int i = 0; i < m_maxPlayerCount; i++ )
		m_player[i] = NULL;
}

hnGroup::~hnGroup()
{
	delete [] m_player;
}

bool
hnGroup::ProcessTurn()
{
	bool didATurn = false;
	
	//--------------------------------------------------------
	//  Early out, if we're an empty group
	//
	//  TODO:  Early out if the newly submitted turn (which
	//   caused ProcessTurn to be called) wasn't submitted by
	//   a member of this group.  This will reduce the number
	//   of packets being sent, in addition to saving CPU
	//   time on the server!  (BIG SAVING)
	//--------------------------------------------------------
	
	if ( m_playerCount > 0 )
	{	
		bool everyoneHasATurn = true;
	
		for ( int i = 0; i < m_maxPlayerCount; i++ )
			if ( m_player[i] != NULL )
				if ( !m_player[i]->HasQueuedTurn() )
					everyoneHasATurn = false;
		
		if ( everyoneHasATurn )
		{
			didATurn = true;
			
			for ( int i = 0; i < m_maxPlayerCount; i++ )
				if ( m_player[i] )
					m_player[i]->DoAction();
			
			for ( int i = 0; i < m_maxPlayerCount; i++ )
				if ( m_player[i] )
					m_player[i]->DoMove();
					
			// now our monsters get to move.
			for ( int i = 0; i < m_monsterCount; i++ )
			{
				m_monster[i]->Think();
			}
			
			// now rebuild our monster list.
			ScanForMonsters();
		}
		else	// some group members still haven't submitted turns.
		{
			//----------------------------------------------------------------------
			// send all group members updated group information on how many people
			// have submitted turns.
			//----------------------------------------------------------------------
			for ( int i = 0; i < m_maxPlayerCount; i++ )
				if ( m_player[i] )
				{
					m_player[i]->SendUpdate();
					//int groupMembers = GetPlayerCount();
					//int groupMembersWithTurns = QueuedTurnCount();
					
					//netServer::GetInstance()->StartMetaPacket( m_player[i]->GetID() );
					//netServer::GetInstance()->SendGroupData( groupMembers, groupMembersWithTurns, m_player[i]->HasQueuedTurn() );
					//netServer::GetInstance()->TransmitMetaPacket();
				}
		}
	}
	return didATurn;
}

void
hnGroup::ScanForMonsters()
{
	m_monsterCount = 0;
	
	// -------------------------------------------------
	//   Scan the map for monsters near to this group.
	//   Make a list of all of those monsters, so we can
	//   let those monsters take a move every time the
	//   players in this group make a move.
	// -------------------------------------------------

	if ( m_playerCount <= 0 )	// early out
		return;

	// -------------------------------------------------
	//   By definition, a group does not span multiple
	//   levels, so find the first player in this group
	//   and check what level he's on.  We will only
	//   look for monsters on that level.
	// -------------------------------------------------
	int level = -1;
	
	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] )
		{
			level = m_player[i]->GetPosition().z;
			break;
		}
	}

	if ( level != -1 )	// this should always be true.  But it never hurts to check.
	{
		mapBase *map = hnDungeon::GetLevel( level );
		assert(map);

		// now we grab all the monsters within 'x' units of our group.

		for ( int y = 0; y < map->GetHeight(); y++ )
			for ( int x = 0; x < map->GetWidth(); x++ )
			{
#define MONSTER_ATTRACT_DISTANCE (15)
#define MONSTER_ATTRACT_DISTANCE_SQ (MONSTER_ATTRACT_DISTANCE*MONSTER_ATTRACT_DISTANCE)
				if ( DistanceFromGroup( hnPoint(x,y,level) ) <= MONSTER_ATTRACT_DISTANCE_SQ )
				{
					entBase *entity = map->MapTile(x,y).entity;
					
					//  Add this entity to our list, if the entity isn't a player.
					if ( entity && !entity->IsAPlayer() )
						m_monster[m_monsterCount++] = entity;
				}
			}
	}
}

int
hnGroup::QueuedTurnCount()
{
	//--------------------------------------------------------
	//  Return the number of players in our group who have
	//  submitted a turn, and so are ready for the group to
	//  process the next turn.
	//--------------------------------------------------------

	int turnCount = 0;
	
	for ( int i = 0; i < m_maxPlayerCount; i++ )
		if ( m_player[i] != NULL )
			if ( m_player[i]->HasQueuedTurn() )
				turnCount++;
	return turnCount;
}

int
hnGroup::DistanceFromGroup( hnPlayer * player )
{
	//--------------------------------------------------------
	//  Find the minimum distance from any of the members of
	//  this group to the passed player.  Obviously, we don't
	//  include the distance from the player to himself, if
	//  the player is already part of this group.
	//--------------------------------------------------------
	
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

int
hnGroup::DistanceFromGroup( const hnPoint & where )
{
	//--------------------------------------------------------
	//  Find the minimum distance from any of the members of
	//  this group to the passed point.
	//--------------------------------------------------------
	int distance = 0;	// return -1 if not eligible to enter group.
	bool firstTest = true;
	
	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] != NULL )
		{
			hnPoint offset = m_player[i]->GetPosition() - where;
			
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
	//------------------------------------------------------
	//  Add a player to our list of members.  Sanity check
	//  first that the player isn't already one of our
	//  members.
	//------------------------------------------------------
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
			player->SetGroup(this);
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
	//------------------------------------------------------
	//  Remove a player to our list of members.
	//------------------------------------------------------
	for ( int i = 0; i < m_maxPlayerCount; i++ )
	{
		if ( m_player[i] == player )
		{
			player->SetGroup(NULL);
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

	if ( minDistance <= s_maxGroupDistanceSq )
	{
		m_group[minGroupID]->AddPlayer(player);
		return;
	}
	else	// no better group for me, so put me in a group all by myself.
	{
		for ( int i = 0; i < m_maxGroupCount; i++ )
		{
			if ( m_group[i]->GetPlayerCount() == 0 )
			{
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
	bool ranATurn = false;
	
	for ( int i = 0; i < m_maxGroupCount; i++ )
	{
		if ( m_group[i]->ProcessTurn() )
			ranATurn = true;
	}

	if ( ranATurn )
	{
		// update vision of everyone to correspond with activities of other groups.

		for ( int i = 0; i < m_maxGroupCount; i++ )
		{
			if ( m_player[i] != NULL )
			{
				m_player[i]->UpdateVision();
				m_player[i]->SendUpdate();
			}
		}
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
