#ifndef CURSESINC
#include <ncurses.h>
#else
#include CURSESINC
#endif
#include <pthread.h>
#include "HN_DisplayTTY.h"
#include "NET_Client.h"
#include "HN_Enum.h"

//#define __DEBUGGING_NETWORK__


hnDisplayTTY::hnDisplayTTY( char * name ):
	hnDisplay(name),
	m_mode(MODE_Normal),
	m_talkLength(0),
	m_needsRefresh(false),
	m_done(false)
{
#ifndef __DEBUGGING_NETWORK__
	initscr();
	keypad(stdscr,true); // enable keyboard mapping
	nonl();
	cbreak();
	noecho();

	if ( has_colors() )
	{
		start_color();
		init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
		init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
		init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
		init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
		init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	}
	
	bool done = false;
	//mvaddch(posy,posx,'@');
	refresh();	
	//setvbuf( stdin, NULL, _IONBF, 0 );
#endif
	
}

hnDisplayTTY::~hnDisplayTTY()
{
#ifndef __DEBUGGING_NETWORK__
	refresh();
	endwin();
#endif
}

bool
hnDisplayTTY::Go()
{
	pthread_t ioThread;
	
	// create a thread to run the event loop...
	
	pthread_create( &ioThread, NULL, StartEventLoop, (void *)this );
	
	return 0;
}

void * StartEventLoop(void *arg)
{
	hnDisplayTTY *display = (hnDisplayTTY *)arg;
	
	display->EventLoop();

	return 0;
}

void
hnDisplayTTY::EventLoop()
{
	
#ifdef __DEBUGGING_NETWORK__
	SendMove(DIR_West);
#endif	

	while ( !m_done )
	{
		int commandkey = getch();

		switch( m_mode )
		{
			case MODE_Normal:
				HandleKeypressNormal( commandkey );
				break;
			case MODE_Talking:
				HandleKeypressTalking( commandkey );
				break;
		}
		Refresh();
	}

	m_client->Disconnect();
}

void
hnDisplayTTY::HandleKeypressNormal(int commandkey)
{
	switch( commandkey )
	{
		case 'q':
			m_client->SendQuit(false);
			m_done = true;
			break;
		case '"':
			m_mode = MODE_Talking;
			m_needsRefresh = true;		// need to print prompt on screen
			break;
		case 'h':
			//printf("Got move west.\n");
			m_client->SendMove(DIR_West);
			break;
		case 'j':
			//printf("Got move south.\n");
			m_client->SendMove(DIR_South);
			break;
		case 'k':
			//printf("Got move north.\n");
			m_client->SendMove(DIR_North);
			break;
		case 'l':
			//printf("Got move east.\n");
			m_client->SendMove(DIR_East);
			break;
		case 'u':
			m_client->SendMove(DIR_NorthEast);
			break;
		case 'y':
			m_client->SendMove(DIR_NorthWest);
			break;
		case 'n':
			m_client->SendMove(DIR_SouthEast);
			break;
		case 'b':
			m_client->SendMove(DIR_SouthWest);
			break;
		default:
			//printf("Got unknown keypress.\n");
			break;
	}
}

void
hnDisplayTTY::HandleKeypressTalking( int commandKey )
{
	switch ( commandKey )
	{
		case '\r':				// other characters I should be checking for here?
		case KEY_ENTER:
			m_mode = MODE_Normal;		 
			m_client->SendTalk( m_talkBuffer );
			m_talkLength = 0;
			m_talkBuffer[0]='\0';
			m_needsRefresh = true;		// get rid of prompts
			// transmit the string now!
			break;
		case KEY_BACKSPACE:
			if ( m_talkLength > 0 )
			{
				m_talkLength--;
				m_talkBuffer[m_talkLength] = '\0';
			}
			m_needsRefresh = true;
			break;
		default:
			// TODO:  Check commandkey is a valid alphanumeric character!
			
			if ( m_talkLength < MAX_TALK_BYTES-1 )
			{
				m_talkBuffer[m_talkLength] = commandKey;
				m_talkBuffer[m_talkLength+1] = '\0';
				m_talkLength++;
				m_needsRefresh = true;
			}
			break;
	}
}

void
hnDisplayTTY::UpdateLocation( const hnPoint &point )
{
	hnDisplay::UpdateLocation(point);

#ifndef __DEBUGGING_NETWORK__
	//color_set( COLOR_WHITE,NULL);
	//mvaddch(m_position.y,m_position.x,'@');
	//move(m_position.y,m_position.x);
	// now we need to update our display appropriately...
//	refresh();
#endif
}

void
hnDisplayTTY::PlotSquare(sint8 x, sint8 y)
{
	hnMaterialType floorType = m_map->MaterialAt(x,y);
	hnWallType wallType = m_map->WallAt(x,y);
	
	const char floorTileChar[] ={
		'?',
		'.',
		' ',
		'#',
		'.',
		'{',
		'^',
		' '
	};

	const char floorTileColor[] ={
		COLOR_WHITE,
		COLOR_WHITE,
		COLOR_WHITE,
		COLOR_WHITE,
		COLOR_BLUE,
		COLOR_BLUE,
		COLOR_RED,
		COLOR_WHITE
	};

	// todo:  Adjust display for walls
#ifndef __DEBUGGING_NETWORK__
	
	char theChar = ' ';

	switch (wallType)
	{
		case WALL_Vertical:
			theChar = '|';
			break;
		case WALL_Horizontal:
			theChar = '-';
			break;
		case WALL_Cross:
			theChar = '-';
			break;
		case WALL_Corridor:
			theChar = '#';
			break;
		case WALL_Room:
		case WALL_Doorway:
			theChar = '.';
			break;
		default:
			theChar = wallType;
			break;
	}
	
	if ( x >= 0 && x < m_map->GetWidth() && y >= 0 && y < m_map->GetHeight() )
	{
		mapClientTile & tile = m_map->MapTile(x,y);

		if ( tile.entity == ENTITY_Player )	// if someone is standing here...
			theChar = '@';			// draw '@' instead of ground.  This is a HACK!
		
		color_set( floorTileColor[floorType],NULL);
		//mvaddch(y,x,floorTileChar[floorType]);
		mvaddch(y,x,theChar);
		move(m_position.y,m_position.x);

		m_needsRefresh = true;
	}
#endif
//	refresh();
}

void
hnDisplayTTY::UpdateMapTile(sint8 x, sint8 y, const mapClientTile &tile)
{
	hnDisplay::UpdateMapTile(x,y,tile);
	m_needsRefresh = true;
	//PlotSquare(x,y);
}


void
hnDisplayTTY::UpdateMapCreature( sint8 x, sint8 y, entType type )
{
	hnDisplay::UpdateMapCreature(x,y,type);
	m_needsRefresh = true;
	//PlotSquare(x,y);
}

void
hnDisplayTTY::Refresh()
{
	if ( m_needsRefresh )
	{
#ifndef __DEBUGGING_NETWORK__
		// redraw screen -- this is hackish.. I ought to make a single function that does this,
		// instead of repeatedly calling a single function for every point on the screen.
		for ( int j = 0; j < m_map->GetHeight(); j++ )
			for ( int i = 0; i < m_map->GetWidth(); i++ )
			{
				PlotSquare(i,j);
			}
		
		// do prompts over the map, if required.
		if ( m_mode == MODE_Talking )
		{
			// draw our string in the top few lines..
			
			move( 1, 1 );
			printw("Say: %s", m_talkBuffer);
		}
	
		refresh();
#endif
		m_needsRefresh = false;
	}
}
