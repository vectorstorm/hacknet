#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "HN_Game.h"
#include "NET_Server.h"

int main( int argc, char *argv[] )
{
	printf("HackNet starting up...\n");
	
	printf("Initialising random number generator...\n");
	srand((int)time(NULL));
	
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
