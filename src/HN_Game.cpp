#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "HN_Game.h"
#include "MAP_Hack.h"
#include "NET_Server.h"
#include "HN_Point.h"
#include "HN_Dungeon.h"
#include "ENT_Player.h"

hnPoint offsetVector[10];
hnGame * hnGame::s_instance = NULL;

#define LEVEL_WIDTH (78)
#define LEVEL_HEIGHT (20)
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
	offsetVector[0].Set(0,-1,0);	// north
	offsetVector[1].Set(1,-1,0);	// northeast
	offsetVector[2].Set(1,0,0);	// east
	offsetVector[3].Set(1,1,0);	// southeast
	offsetVector[4].Set(0,1,0);	// south
	offsetVector[5].Set(-1,1,0);	// southwest
	offsetVector[6].Set(-1,0,0);	// west
	offsetVector[7].Set(-1,-1,0);	// northwest
	offsetVector[8].Set(0,0,-1);	// up
	offsetVector[9].Set(0,0,1);	// down
	
	printf("Generating maps\n");
/*	for ( int i = 0; i < MAX_LEVELS; i++ )
	{
		printf(".");
		hnDungeon::GetLevel[i] = NULL;
		hnDungeon::GetLevel[i] = new mapHack(LEVEL_WIDTH, LEVEL_HEIGHT);
		hnDungeon::GetLevel[i]->Generate();
	}*/
	
	hnDungeon::Startup( MAX_LEVELS, LEVEL_WIDTH, LEVEL_HEIGHT );
	
	printf("\n");
}

hnGame::~hnGame()
{
/*	for ( int i = 0; i < MAX_LEVELS; i++ )
	{
		delete hnDungeon::GetLevel[i];
	}*/

	hnDungeon::Shutdown();
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
hnGame::ClientJoined(int playerID)
{
	// a client has joined
	
	int x = 0;	
	int y = 0;	
	int z = 0;

	int count = 0;

	do {
		x = (rand() % LEVEL_WIDTH);
		y = (rand() % LEVEL_HEIGHT);

		if ( count++ > 500 )
		{
			netServer::GetInstance()->DisconnectClientID(playerID);
			return;			// AIEE!!!  Couldn't place the player!
		}
		
	}while ( hnDungeon::GetLevel(z)->WallAt(x,y) & WALL_Any || hnDungeon::GetLevel(z)->MapTile(x,y).entity != NULL );

	printf("Setting playerID %d initial position to: (%d,%d,%d)\n", playerID, x, y, z);
	m_player[playerID] = new entPlayer( playerID, hnPoint(x,y,z) );
	hnDungeon::GetLevel(z)->PutEntityAt( m_player[playerID], x, y );
	
	netServer::GetInstance()->StartMetaPacket( playerID );
	netServer::GetInstance()->SendClientLocation( m_player[playerID]->GetPosition() );
	mapBase *map = hnDungeon::GetLevel(z);
	netServer::GetInstance()->SendMapReset( map->GetWidth(), map->GetHeight() );
	netServer::GetInstance()->TransmitMetaPacket();	// all done!  Send it!
	
	m_player[playerID]->PostTurn();
}

void
hnGame::ClientName(int playerID, char * name)
{
	m_player[playerID]->SetName(name);
}

#define MAX_TALK_BUFFER (256)

void
hnGame::ClientTalk(int playerID, char * talk)
{
	int talkLength = strlen(talk);

	if ( talkLength > 0 )
	{	
		hnPoint position = m_player[playerID]->GetPosition();
		
		if ( talk[talkLength-1] == '!' )
		{
			// we're shouting.
			char buffer[MAX_TALK_BUFFER];
			char *name = m_player[playerID]->GetName();
			
			snprintf( buffer, MAX_TALK_BUFFER, "%s shouts, \"%s\"", name, talk );
			
			for ( int i = 0; i < MAX_CLIENTS; i++ )
				if ( m_player[i] )
					m_player[i]->Listen( buffer );
		}
		else
		{
			char buffer[MAX_TALK_BUFFER];
			char *name = m_player[playerID]->GetName();
			
			if ( talk[talkLength-1] == '?' )	// different message for questions.
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
hnGame::ClientQuit(int playerID)
{
	// when a client quits, we need to remove his entity.
	
	hnDungeon::GetLevel(m_player[playerID]->GetPosition().z)->RemoveEntity( m_player[playerID] );
	delete m_player[playerID];
	m_player[playerID] = NULL;
}


void
hnGame::ClientMove(int playerID, hnDirection dir)
{
	entPlayer *player = m_player[playerID];
	if ( dir >= 0 && dir < 10 )	// final sanity check
	{
		bool legalMove = false;
		bool blocked = false;
		//bool legalMove = true;
		
		if ( dir >= 0 && dir < 8 )
		{
			// north->northwest
			hnPoint potentialPos = offsetVector[dir] + player->GetPosition();
			
			if ( hnDungeon::GetLevel(potentialPos.z)->WallAt(potentialPos.x,potentialPos.y) & WALL_Passable )
				if ( hnDungeon::GetLevel(potentialPos.z)->MapTile(potentialPos.x,potentialPos.y).entity == NULL )
				{
					legalMove = true;
				}
				else
					blocked = true;
		}
		else
		{
			// up or down -- check for appropriate stairways here.
			hnPoint currentPos = player->GetPosition();
			if ( dir == DIR_Up )
				if ( hnDungeon::GetLevel(currentPos.z)->WallAt(currentPos.x,currentPos.y) & WALL_StairsUp )
				{
					hnPoint2D stairsPos = hnDungeon::GetLevel(currentPos.z-1)->GetDownStairs();
					if ( hnDungeon::GetLevel(currentPos.z-1)->MapTile(stairsPos.x, stairsPos.y).entity == NULL )
					{
						legalMove = true;	// nobody standing where we want to go.
					}
					else
						blocked = true;
				}
			if ( dir == DIR_Down )
				if ( hnDungeon::GetLevel(currentPos.z)->WallAt(currentPos.x,currentPos.y) & WALL_StairsDown )
				{
					hnPoint2D stairsPos = hnDungeon::GetLevel(currentPos.z+1)->GetUpStairs();
					if ( hnDungeon::GetLevel(currentPos.z+1)->MapTile(stairsPos.x, stairsPos.y).entity == NULL )
					{
						legalMove = true;	// nobody standing where we want to go.
					}
					else
						blocked = true;
				}
		}
		
		if ( blocked )
		{
			char *buffer = "Someone is blocking the way.";
			netServer::GetInstance()->StartMetaPacket( playerID );
			netServer::GetInstance()->SendMessage(buffer);
			netServer::GetInstance()->TransmitMetaPacket();
		}
		if ( legalMove )
		{
			// TODO:
			// I'd really like to move most (all?) of this code into hnPlayer::Move() and/or MoveTo().
			hnPoint iniPos = player->GetPosition();
			netMapUpdateBBox update;
			
			player->Move(dir);
			player->PostTurn();
			
			//-------------------------------------------------------------------------
			//  This next segment of code is temporary.  What it does is search for
			//  client who are near to the player who just moved, and sends them
			//  updated visibility information on that player, so that he moves
			//  correctly.  To be more correct, we should be synchronizing nearby
			//  players' turns, so that all their visibility information would have
			//  already been updated in their 'postturn' function, which would have
			//  been called above.  Until then, this is a HACK.   -- Trevor, 22/04/2001
			//-------------------------------------------------------------------------
			
			hnPoint endPos = player->GetPosition();

			for ( int i = 0; i < MAX_CLIENTS; i++ )
			{
				if ( i != playerID && m_player[i] != NULL )
				{
					if ( m_player[i]->CanSee(iniPos) || m_player[i]->CanSee(endPos) )
						m_player[i]->PostTurn();
				}
			}
		}
	}else{
		printf("Tried to move in an illegal direction: %d.\n", dir);
	}

}
