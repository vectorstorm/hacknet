#include <stdio.h>
#include <string.h>
#include "config.h"
#include "HN_Display.h"
#include "NET_Client.h"

#ifdef TTY_DISPLAY
#include "HN_DisplayTTY.h"
#endif

int main( int argc, char *argv[] )
{
	printf("HackNet Client starting up...\n");

	printf("Starting hacknet client...\n");
		
	hnDisplay *display = NULL;
#ifdef TTY_DISPLAY
	display = new hnDisplayTTY;
#endif

	if ( display )
	{
		netClient *client = new netClient(display, "localhost");
		client->Go();
		delete client;
	}
	
	delete display;

	return 0;
}
