#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "ENT_Manager.h"
#include "HN_Game.h"
#include "MAP_Hack.h"
#include "NET_Server.h"
#include "HN_Point.h"
#include "HN_Dungeon.h"
#include "HN_Player.h"
#include "HN_Group.h"
#include "OBJ_Manager.h"
#include "OBJ_Base.h"


hnGame * hnGame::s_instance = NULL;

#define LEVEL_WIDTH (78)
#define LEVEL_HEIGHT (18)
#define MAX_LEVELS (5)

void
hnGame::Startup()
{
	assert(s_instance == NULL);
	s_instance = new hnGame;
}

void
hnGame::Shutdown()
{
	assert(s_instance);
	delete s_instance;
	s_instance = NULL;
}

hnGame *
hnGame::GetInstance()
{
	assert(s_instance);
	return s_instance;
}


hnGame::hnGame()
{
// TODO:  This should be implemented in an entirely different way.
//        I'm not sure what, yet.

	printf("Initialising objects...\n");
	objManager::Startup();
	printf("Initialising monsters...\n");
	entManager::Startup();
	printf("Generating maps...\n");
	hnDungeon::Startup( MAX_LEVELS, LEVEL_WIDTH, LEVEL_HEIGHT );
	hnGroupManager::Startup( MAX_CLIENTS );

	for ( int i = 0; i < MAX_CLIENTS; i++ )
		m_player[i] = NULL;
	
	printf("\n");
}

hnGame::~hnGame()
{
	hnGroupManager::Shutdown();
	hnDungeon::Shutdown();
	entManager::Shutdown();
	objManager::Shutdown();
}

char *
hnGame::GetPlayerName(int playerID)
{
	static char * unnamed = "Unnamed";
	
	if ( playerID < MAX_CLIENTS )
	{
		if ( m_player[playerID] )
		{
			return m_player[playerID]->GetName();
		}
	}

	return unnamed;
}

void
hnGame::SeenEvent( entBase *entity, char * message )
{
	hnPoint pos = entity->GetPosition();

	for ( int i = 0; i < MAX_CLIENTS; i++ )
	{
		if ( m_player[i] )
			m_player[i]->See(pos, entity, message);
	}
}

void
hnGame::ClientJoined(int playerID)
{
	// a client has joined, so try to find a place to put him/her.
	
	int x = 0;	
	int y = 0;	
	int z = 0;

	int count = 0;

	do {
		x = (rand() % LEVEL_WIDTH);
		y = (rand() % LEVEL_HEIGHT);

		if ( count++ > 500 )
		{
			netServer::GetInstance()->SendQuitConfirm(playerID);
			netServer::GetInstance()->DisconnectClientID(playerID);
			printf("ERROR:  Was unable to place client id %d.\n", playerID );
			return;			// AIEE!!!  Couldn't place the player!
		}
		
	}while ( hnDungeon::GetLevel(z)->WallAt(x,y) & WALL_Any || hnDungeon::GetLevel(z)->MapTile(x,y).entity != NULL );

	printf("Setting playerID %d initial position to: (%d,%d,%d)\n", playerID, x, y, z);
	m_player[playerID] = new hnPlayer( playerID, hnPoint(x,y,z) );
	
	// ----------------------------------------------------------------------------------
	//   Send client some setup data so he's ready to start playing.  First, we need
	//   to send a map reset, so he knows what size to create the map.  Then, we'll
	//   tell the player to calculate his field of view, update the contents of that
	//   field of view, and, finally, transmit the things he sees to his client.
	// ----------------------------------------------------------------------------------
	netServer::GetInstance()->StartMetaPacket( playerID );
	netServer::GetInstance()->SendDungeonReset( hnDungeon::GetInstance()->GetLevelCount() );
	mapBase *map = hnDungeon::GetLevel(z);
	netServer::GetInstance()->SendMapReset( map->GetWidth(), map->GetHeight(), z );
	
	// --- Transmit object information.. for now, send the whole object database.
	// --- In the future, we'll let the client request the objects he's interested
	// --- in, to minimize bandwidth requirements when joining.
	
	netServer::GetInstance()->SendObjectStats( objManager::GetInstance()->GetObjectCount() );

	for ( int i = 0; i < objManager::GetInstance()->GetObjectCount(); i++ )
	{
		const objPrototype &proto = objManager::GetInstance()->GetPrototype(i);
		if ( proto.name )
			netServer::GetInstance()->SendObjectName( i, proto.type, proto.name );
	}
	
	netServer::GetInstance()->SendClientLocation( m_player[playerID]->GetPosition() );
	
	netServer::GetInstance()->TransmitMetaPacket();	// all done!  Send it!
	
	hnGroupManager::GetInstance()->AddPlayer( m_player[playerID] );
	
	m_player[playerID]->RecalculateVision();
	m_player[playerID]->UpdateVision();
	m_player[playerID]->SendUpdate();
}

void
hnGame::ClientRequestRefresh(int playerID, int level)
{
	//-----------------------------------------------------------------
	//  The client has requested a full map refresh, so first send a
	//  map reset packet, then send a full vision refresh.
	//
	//  A properly written client should never request a full map
	//  refresh, except after restoring a saved game (which we
	//  currently don't do anyway), at which point a refresh
	//  is requested to reinitialise the client's locally cached
	//  vision maps.
	//-----------------------------------------------------------------
	
	netServer::GetInstance()->StartMetaPacket( playerID );
	mapBase *map = hnDungeon::GetLevel(level);
	
	netServer::GetInstance()->SendMapReset( map->GetWidth(), map->GetHeight(), level );
	netServer::GetInstance()->TransmitMetaPacket();
	
	// now send all data we have on this map.
	
	printf("Sending map refresh for level %d.\n", level);
	m_player[playerID]->RefreshMap( level );
	
}

void
hnGame::ClientName(int playerID, char * name, int /*nameBufferLength*/)
{
	//------------------------------------------------------------------
	//   The client has just told us his name, so set it on the player.
	//------------------------------------------------------------------
	m_player[playerID]->SetName(name);
}

#define MAX_TALK_BUFFER (256)

void
hnGame::ClientTalk(int playerID, char * talk, int talkBufferLength)
{
	//------------------------------------------------------------------
	//   The client has just tried to say something.  We want to
	//   process the 'talk' string and check to see if it's a shout
	//   or just regular speech.  Create an appropriate string to
	//   send to each client, and then send the message to each
	//   player object.  The player objects may then each send the
	//   messages to their clients, if appropriate.
	//------------------------------------------------------------------
	int talkLength = strlen(talk);

	if ( talkLength > 0 )
	{	
		hnPoint position = m_player[playerID]->GetPosition();
		char name[128];
		m_player[playerID]->GetFullName(name, 128);
		
		if ( talk[talkLength-1] == '!' )
		{
			// we're shouting.
			char buffer[MAX_TALK_BUFFER];
			
			snprintf( buffer, MAX_TALK_BUFFER, "%s shouts, \"%s\"", name, talk );
			
			for ( int i = 0; i < MAX_CLIENTS; i++ )
				if ( m_player[i] )
					m_player[i]->Listen( buffer );
		}
		else
		{
			char buffer[MAX_TALK_BUFFER];
			
			//----------------------------------------------------------
			//  Use a different verb if the client is asking a question.
			//----------------------------------------------------------
			if ( talk[talkLength-1] == '?' )
				snprintf( buffer, MAX_TALK_BUFFER, "%s asks, \"%s\"", name, talk );
			else
				snprintf( buffer, MAX_TALK_BUFFER, "%s says, \"%s\"", name, talk );
	
			for ( int i = 0; i < MAX_CLIENTS; i++ )
				if ( m_player[i] )
					m_player[i]->Listen( position, buffer );
		}
	}
}

void
hnGame::ClientTake(int playerID, objDescription &desc, uint8 stackID)
{
	m_player[playerID]->Take( desc, stackID );
	ClientTurn();
}

void
hnGame::ClientDrop(int playerID, objDescription &desc, uint8 inventorySlot)
{
	printf("Player %d requests drop of inventory slot %d.\n", playerID, inventorySlot );
	m_player[playerID]->Drop( desc, inventorySlot );
	ClientTurn();
}

void
hnGame::ClientWield(int playerID, objDescription &desc, uint8 inventorySlot)
{
	m_player[playerID]->Wield( desc, inventorySlot );
	ClientTurn();
}

void
hnGame::ClientWear(int playerID, objDescription &desc, uint8 inventorySlot)
{
	m_player[playerID]->Wear( desc, inventorySlot );
	ClientTurn();
}

void
hnGame::ClientRemove(int playerID, objDescription &desc, uint8 inventorySlot)
{
	m_player[playerID]->Remove( desc, inventorySlot );
	ClientTurn();
}

void
hnGame::ClientQuaff(int playerID, objDescription &desc, uint8 inventorySlot)
{
	m_player[playerID]->Quaff( desc, inventorySlot );
	ClientTurn();
}

void
hnGame::ClientEat(int playerID, objDescription &desc, uint8 inventorySlot)
{
	m_player[playerID]->Eat( desc, inventorySlot );
	ClientTurn();
}

void
hnGame::ClientQuit(int playerID)
{
	// --------------------------------------------------------------
	//  The client quit, so we need to remove him from his group,
	//  then delete his player.
	// --------------------------------------------------------------
	hnGroupManager::GetInstance()->RemovePlayer( m_player[playerID] );
	
	delete m_player[playerID];
	m_player[playerID] = NULL;
}

void
hnGame::ClientTurn()
{
	// we've received a turn from somebody and done whatever
	// was required for it.. now tell the group manager to
	// check to see if it needs to run a turn.
	
	if ( hnGroupManager::GetInstance()->ProcessTurn() )
	{
		hnGroupManager::GetInstance()->UpdateGroups();

		// check all players to be sure they're still alive.

		for ( int i = 0; i < MAX_CLIENTS; i++ )
		{
			if ( m_player[i] )
			{
				if ( !m_player[i]->IsAlive() )
				{
					printf("%s (id %d) died!  Killing client...\n", GetPlayerName(i), i);
					netServer::GetInstance()->SendQuitConfirm(i);
				}
			}

			// I should now check all monsters, too, to see if
			// any of them are dead.. but it's sort of clumsy
			// to be doing that from here.  Perhaps that belongs
			// in the attack code?
		}
	}
}

void
hnGame::ClientMove(int playerID, hnDirection dir)
{
	// -------------------------------------------------------------
	//  The player has submitted a 'move' command.  Do some basic
	//  error checking to be sure the direction passed in is legal,
	//  and then pass it on to the player object.
	// 
	//  THE PLAYER WILL NOT ACTUALLY MOVE.  The player will queue
	//  the move command, and will eventually perform the queued
	//  function when his group finally gets to process a turn.
	//
	//  For this reason, after we send the command to the player
	//  object, we tell the GroupManager to check whether any
	//  groups can process a turn, and then to check to see whether
	//  as a result of this move, the distribution of players
	//  within groups has changed.
	// -------------------------------------------------------------
	hnPlayer *player = m_player[playerID];
	if ( dir >= 0 && dir < DIR_MAX )	// sanity check
	{	
		player->Move(dir);
		ClientTurn();
	}else{
		printf("Tried to move in an illegal direction: %d.\n", dir);
		netServer::GetInstance()->SendQuitConfirm(playerID);
		netServer::GetInstance()->DisconnectClientID(playerID);
	}
}

void
hnGame::ClientAttack(int playerID, hnDirection dir)
{
	// -------------------------------------------------------------
	//  The player has submitted an 'attack' command.  Do some basic
	//  error checking to be sure the direction passed in is legal,
	//  and then pass it on to the player object.
	// 
	//  THE PLAYER WILL NOT ACTUALLY ATTACK.  The player will queue
	//  the attack command, and will eventually perform the queued
	//  function when his group finally gets to process a turn.
	//
	//  For this reason, after we send the command to the player
	//  object, we tell the GroupManager to check whether any
	//  groups can process a turn, and then to check to see whether
	//  as a result of this move, the distribution of players
	//  within groups has changed.
	// -------------------------------------------------------------
	
	hnPlayer *player = m_player[playerID];
	if ( dir >= 0 && dir < DIR_Up )	// sanity check
	{	
		player->Attack(dir);
		ClientTurn();
	}else{
		printf("Tried to attack in an illegal direction: %d.\n", dir);
		netServer::GetInstance()->SendQuitConfirm(playerID);
		netServer::GetInstance()->DisconnectClientID(playerID);
	}
}

void
hnGame::ClientWait( int playerID )
{
	hnPlayer *player = m_player[playerID];

	player->Wait();
	ClientTurn();
}
