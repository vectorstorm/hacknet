#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HN_Game.h"
#include "HN_Random.h"
#include "NET_Server.h"
#include "HN_Group.h"

int main( int argc, char *argv[] )
{
	// parse arguments

	bool legalArgs = true;
	bool useGroups = false;

	if ( argc > 1 )
	{
		for ( int i = 1; i < argc; i++ )
		{
			if ( strncmp( argv[i], "--with-groups", 13 ) == 0 ||
				strncmp( argv[i], "--withgroups", 12 ) == 0 ||
				strncmp( argv[i], "--wg", 4 ) == 0 )
			{
				useGroups = true;
			}
			else
				legalArgs = false;
		}
	}

	if ( !legalArgs )
	{
		printf("HackNet Server version %s\n", VERSION );
		printf("Usage: hacknetd [--with-groups]\n");
		exit(1);
	}

	if ( !useGroups )
		hnGroupManager::SetMaxGroupDistance(0);
	
	printf("HackNet version %s starting up...\n", VERSION );
	
	printf("Initialising random number system...\n");
	hnRandom::Startup( time(NULL) );
	
	printf("Initialising game...\n");
	hnGame::Startup();
	hnGame *game = hnGame::GetInstance();

	if ( game )
	{
		printf("Starting server...\n");
		netServer::Startup();
		netServer *server = netServer::GetInstance();
		
		server->Go();
		
		printf("Server exiting.\n");
		netServer::Shutdown();
	}
		
	printf("Game ending...\n");
	hnGame::Shutdown();
}
