#ifndef CURSESINC
#include <ncurses.h>
#else
#include CURSESINC
#endif
#include <pthread.h>
#include "HN_DisplayTTY.h"
#include "NET_Client.h"
#include "HN_Enum.h"
#include "HN_Consts.h"

//#define DEBUGGING


hnDisplayTTY::hnDisplayTTY():
	m_needsRefresh(false)
{
#ifndef DEBUGGING
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
#ifndef DEBUGGING
	refresh();
	endwin();
#endif
}

bool
hnDisplayTTY::Go()
{
	pthread_t ioThread;
	
	// create a thread to run the event loop...
	
	pthread_create( &ioThread, NULL, EventLoop, (void *)m_client );
	
	return 0;
}

void * EventLoop(void *arg)
{
	netClient *client = (netClient *)arg;
	
#ifdef DEBUGGING
	client->SendMove(DIR_West);
#endif	

	bool done = false;
	while ( !done )
	{
		int commandkey = getch();
		if ( commandkey == 'q' ){
			//printf("Got quit key.. trying to disconnect.\n");
			client->SendQuit(false);
			done = true;
		}else{
			switch( commandkey )
			{
				case 'h':
					//printf("Got move west.\n");
					client->SendMove(DIR_West);
					break;
				case 'j':
					//printf("Got move south.\n");
					client->SendMove(DIR_South);
					break;
				case 'k':
					//printf("Got move north.\n");
					client->SendMove(DIR_North);
					break;
				case 'l':
					//printf("Got move east.\n");
					client->SendMove(DIR_East);
					break;
				case 'u':
					client->SendMove(DIR_NorthEast);
					break;
				case 'y':
					client->SendMove(DIR_NorthWest);
					break;
				case 'n':
					client->SendMove(DIR_SouthEast);
					break;
				case 'b':
					client->SendMove(DIR_SouthWest);
					break;
				default:
					//printf("Got unknown keypress.\n");
					break;
			}
		}
	}

	client->Disconnect();

	return NULL;
}

void
hnDisplayTTY::UpdateLocation( const hnPoint &point )
{
	hnDisplay::UpdateLocation(point);

#ifndef DEBUGGING
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
#ifndef DEBUGGING
	
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
	
	if ( x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT )
	{
		hnMapTile & tile = m_map->MapTile(x,y);

		if ( tile.entity != NULL )		// if someone is standing here...
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
hnDisplayTTY::UpdateMapTile(sint8 x, sint8 y, const hnMapTile &tile)
{
	hnDisplay::UpdateMapTile(x,y,tile);
	PlotSquare(x,y);
}


void
hnDisplayTTY::UpdateMapEntity( sint8 x, sint8 y, hnObjectType type )
{
	hnDisplay::UpdateMapEntity(x,y,type);
	PlotSquare(x,y);
}

void
hnDisplayTTY::Refresh()
{
	if ( m_needsRefresh )
	{
#ifndef DEBUGGING
		refresh();
#endif
		m_needsRefresh = false;
	}
}
