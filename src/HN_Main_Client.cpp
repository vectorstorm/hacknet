#include <stdio.h>
#include <string.h>
#include "HN_Config_Client.h"
#include "HN_Display.h"
#include "NET_Client.h"

#ifdef TTY_DISPLAY
#include "HN_DisplayTTY.h"
#endif

int main( int argc, char *argv[] )
{
	printf("HackNet Client starting up...\n");

	// parse arguments..

	char * host = "localhost";
	if ( argc > 0 )
	{
		//for (int i = 1; i < argc-1; i++ )
		//{
		//	// parse options here
		//}
		
		host = argv[argc-1];
	}


	printf("Starting hacknet client...\n");
	
	hnDisplay *display = NULL;
#ifdef TTY_DISPLAY
	display = new hnDisplayTTY;
#endif

	if ( display )
	{
		netClient *client = new netClient(display, host);
		client->Go();
		delete client;
	}
	
	delete display;

	return 0;
}
