#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HN_Display.h"
#include "NET_Client.h"
#include "HN_DisplayTTY.h"

int main( int argc, char *argv[] )
{

	// parse arguments..
#define MAX_ARG_SIZE (256)
	char host[MAX_ARG_SIZE] = "localhost";
	char name[MAX_ARG_SIZE] = "Unknown";
	char * username = getenv("USER");
	bool showVersionOnly = false;
	//bool legalArgs = false;
	
	if ( !username )
		username = getenv("LOGNAME");
	
	if ( username )
	{
		strncpy(name,username,MAX_ARG_SIZE);
		name[MAX_ARG_SIZE-1] = '\0';
	}
	
	if ( argc > 1 )
	{
		for (int i = 1; i < argc; i++ )
		{
			// parse options here
			if ( strncmp( argv[i], "-u", 2 ) == 0 )
			{
				if ( strlen( argv[i] ) > 2 )	// if the name is in this argument (no space)
					strncpy(name,argv[i] + 2, MAX_ARG_SIZE);
				else				// if it's in the next argument (with space)
					strncpy(name,argv[++i], MAX_ARG_SIZE);
				name[MAX_ARG_SIZE-1] = '\0';
			}
			else if ( strncmp( argv[i], "-v", 2 ) == 0 ||
				strncmp( argv[i], "--version", 9 ) == 0 ||
				strncmp( argv[i], "-h", 2 ) == 0 ||
				strncmp( argv[i], "--help", 6 ) == 0 )
			{
				showVersionOnly = true;
			}
			else if ( i == argc-1 )
			{
				strncpy(host,argv[i],MAX_ARG_SIZE);
				host[MAX_ARG_SIZE-1] = '\0';
	//			legalArgs = true;
			}
		}
	}
	
	if ( showVersionOnly )
	{
		printf("HackNet client version %s\n", VERSION);
		printf("Usage: hacknet [-u Name] host\n");
		exit(1);
	}	
	else
	{
		printf("Starting hacknet client...\n");
		printf("Contacting %s...\n", host);
		hnDisplay *display = NULL;
	
		display = new hnDisplayTTY(name);

		if ( display )
		{
			netClient *client = new netClient(display, host);
			client->Go();
			delete client;
		}
	
		delete display;
	}
	return 0;
}
