#if HAVE_LIBNCURSES
  #include <ncurses.h>
#else
  #if HAVE_LIBCURSES
    #include <curses.h>
  #else
    #error "No curses library found!"
  #endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <strings.h>
#include "HN_DisplayTTY.h"
#include "NET_Client.h"
#include "HN_Enum.h"

//#define __DEBUGGING_NETWORK__

#if !HAS_COLOR_SET
	// if we don't have a 'color_set' function in curses, provide something for the source code's calls
	// to link against.
	int color_set(short,void * ){}
#endif

hnDisplayTTY::hnDisplayTTY( char * name ):
	hnDisplay(name),
	m_mode(MODE_Normal),
	m_talkLength(0),
	m_messageLines(0),
	m_messageDisplayLine(0),
	m_awaitingMore(false),
	m_needsRefresh(false),
	m_done(false)
{
	m_talkBuffer[0] = '\0';

	for ( int i = 0; i < MAX_MESSAGE_SCROLLBACK; i++ )
		m_messageBuffer[i][0] = '\0';

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
	
	refresh();	
#endif
	
}

hnDisplayTTY::~hnDisplayTTY()
{
#ifndef __DEBUGGING_NETWORK__
	refresh();
	endwin();
#endif

	// put any debugging output here, if needed -- curses has been shut down and printf is safe again.
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
	MoveCommand(DIR_West);
	//m_client->SendTalk("Hoi!");
#endif	

	while ( !m_done )
	{
		int commandkey = getch();

		switch( m_mode )
		{
			case MODE_Normal:
				if ( m_awaitingMore )
					HandleKeypressMore( commandkey );
				else
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
//----------------------------------------------------------------------
//  TODO:  There must be a cleaner way to do this than using this massive
//    switch statement!  (We haven't even got 10% of the eventual list of
//    commands yet, even!)
//----------------------------------------------------------------------

	switch( commandkey )
	{ 
		case 'p':
			m_messageDisplayLine = (m_messageDisplayLine==0)?0:m_messageDisplayLine-1;
			m_needsRefresh = true;
			break;
		case 'q':
			m_client->SendQuit(false);
			m_done = true;
			break;
		case '"':
			m_mode = MODE_Talking;
			m_needsRefresh = true;		// need to print prompt on screen
			break;
		case 'h':
		case '4':
			MoveCommand(DIR_West);
			break;
		case 'j':
		case '2':
			MoveCommand(DIR_South);
			break;
		case 'k':
		case '8':
			MoveCommand(DIR_North);
			break;
		case '6':
		case 'l':
			MoveCommand(DIR_East);
			break;
		case '9':
		case 'u':
			MoveCommand(DIR_NorthEast);
			break;
		case '7':
		case 'y':
			MoveCommand(DIR_NorthWest);
			break;
		case '3':
		case 'n':
			MoveCommand(DIR_SouthEast);
			break;
		case '1':
		case 'b':
			MoveCommand(DIR_SouthWest);
			break;
		case '>':
			MoveCommand(DIR_Down);
			break;
		case '<':
			MoveCommand(DIR_Up);
			break;
		case '.':
		case ' ':
			WaitCommand();
			break;
		default:
			//printf("Got unknown keypress.\n");
			break;
	}
}

void
hnDisplayTTY::HandleKeypressMore( int commandKey )
{
	//----------------------------------------------------------
	//  Should we be checking to see what key is hit?
	//  Currently, ANY key will advance to the next line.
	//----------------------------------------------------------

	if ( ++m_messageDisplayLine < m_messageLines-1 )
	{
		//----------------------------------------------------------
		// we have yet another 'more' line.
		// this line doesn't need to be here, since it must be
		// true in order for us to get here.. but it's here for
		// clarity.  :)
		//----------------------------------------------------------
		m_awaitingMore = true;
	}
	else
	{
		m_awaitingMore = false;
	}
	m_needsRefresh = true;		// need to show next line.
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

//--------------------------------------------------------------------
//  This function sets the location where we are within the dungeon.
//  TODO:  We probably need to change this function name to make it
//  more obvious what it's doing.
//--------------------------------------------------------------------
void
hnDisplayTTY::UpdateLocation( const hnPoint &point )
{
	hnDisplay::UpdateLocation(point);
}


//--------------------------------------------------------------------
//  Plot an individual square of the map to the screen.
//--------------------------------------------------------------------
void
hnDisplayTTY::PlotSquare(sint8 x, sint8 y)
{
	hnMaterialType floorType = m_map[m_position.z]->MaterialAt(x,y);
	hnWallType wallType = m_map[m_position.z]->WallAt(x,y);
	
	
	//  TODO:  Make this whole function smarter!  Fewer tables, fewer switch statements!

	/*const char floorTileChar[] ={
		'?',
		'.',
		' ',
		'#',
		'.',
		'{',
		'^',
		' '
	};*/

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
		case WALL_StairsUp:
			theChar = '<';
			break;
		case WALL_StairsDown:
			theChar = '>';
			break;
		default:
			theChar = wallType;
			break;
	}
	
	if ( x >= 0 && x < m_map[m_position.z]->GetWidth() && y >= 0 && y < m_map[m_position.z]->GetHeight() )
	{
		mapClientTile & tile = m_map[m_position.z]->MapTile(x,y);
		color_set( floorTileColor[floorType],NULL);
		
		if ( tile.entity == ENTITY_Human )	// if someone is standing here...
		{
			color_set( COLOR_WHITE, NULL );
			theChar = '@';			// draw '@' instead of ground.  This is a HACK!
		}
		else if ( tile.entity == ENTITY_GridBug )
		{
			color_set( COLOR_MAGENTA, NULL );
			theChar = 'x';
		}
		else if ( tile.objectCount > 0 )
		{
			color_set( COLOR_WHITE, NULL );
			theChar = ')';
		}
		mvaddch(y + 3,x,theChar);
		move(m_position.y + 3,m_position.x);

		m_needsRefresh = true;
	}
#endif
}

void
hnDisplayTTY::UpdateMapTile(const hnPoint &point, const mapClientTile &tile)
{
	hnDisplay::UpdateMapTile(point,tile);
	m_needsRefresh = true;
}

//---  This function isn't actually used any more.  TODO:  Consider removing it.
void
hnDisplayTTY::UpdateMapCreature( const hnPoint &point, entType type )
{
	hnDisplay::UpdateMapCreature(point,type);
	m_needsRefresh = true;
}

//--  Stick the passed message into our message buffer.  TODO:  Consider making this more like NetHack's
//    message buffer, with cntrl-p to scroll back through previous messages.
void
hnDisplayTTY::TextMessage( char * message )
{
	//----------------------------------------------------------------
	//  If we're currently looking at a line, consider appending the
	//  new message to whatever's the most recent line.  Otherwise, go
	//  through the cases below, and just make this message a new line.
	//----------------------------------------------------------------
	if ( m_messageDisplayLine < m_messageLines )
	{
		size_t messageLength = strlen( message );
		size_t currentLineLength = strlen( m_messageBuffer[m_messageLines-1] );
	
		if ( currentLineLength + messageLength <= 78 )
		{
			sprintf( m_messageBuffer[m_messageLines-1], "%s  %s", m_messageBuffer[m_messageLines-1], message );
			m_needsRefresh = true;
			return;
		}
	}
	
	if ( m_messageLines < MAX_MESSAGE_SCROLLBACK )
	{
		// we haven't filled up our set of lines yet, so just add us...
		strncpy( m_messageBuffer[m_messageLines], message, MAX_MESSAGE_BYTES );
		m_messageLines++;
	}
	else
	{
		// we're full, so scroll the lines up, then add us at the bottom.
		for ( int i = 0; i < m_messageLines-1; i++ )
			strncpy( m_messageBuffer[i], m_messageBuffer[i+1], MAX_MESSAGE_BYTES );
		strncpy( m_messageBuffer[m_messageLines-1], message, MAX_MESSAGE_BYTES );	
	}
	
	// if we're looking at a line further along than the new one
	// (which means that we're not actually looking at anything),
	// then reset us to look at this new line.
	// otherwise, set our 'more' prompt.
	if ( m_messageDisplayLine >= m_messageLines-1 )
		m_messageDisplayLine = m_messageLines-1;
	else
		m_awaitingMore = true;
	
	m_needsRefresh = true;
}

void
hnDisplayTTY::UpdateGroupData( int groupMemberCount, int groupMemberTurnCount, bool submittedTurn )
{
	hnDisplay::UpdateGroupData( groupMemberCount, groupMemberTurnCount, submittedTurn );

	if ( (groupMemberCount != m_groupMemberCount) || (m_groupMemberCount > 1) )
		m_needsRefresh = true;
}

void
hnDisplayTTY::Refresh()
{
	if ( m_needsRefresh )
	{
#ifndef __DEBUGGING_NETWORK__
		// redraw screen -- this is hackish.. TODO: Make a single function that does a full redraw,
		// instead of repeatedly calling a single function for every point on the screen.
		for ( int j = 0; j < m_map[m_position.z]->GetHeight(); j++ )
			for ( int i = 0; i < m_map[m_position.z]->GetWidth(); i++ )
			{
				PlotSquare(i,j);
			}
		
		// clear upper prompt area.
		int maxy, maxx;
		getmaxyx(stdscr, maxy, maxx);
		for ( int j = 0; j < 3; j++ )
		{
			for ( int i = 0; i < maxx; i++ )
			{
				mvaddch(j,i,' ');
			}
		}
		// clear lower prompt area
		for ( int j = 23; j < 24; j++ )
		{
			for ( int i = 0; i < maxx; i++ )
			{
				mvaddch(j,i,' ');
			}
		}
		
		// do status bar

		move( 22, 0 );
		printw("%s     St:%d Dx:%d Co:%d In:%d Wi:%d Ch:%d", m_name,
			m_status->Strength(), m_status->Dexterity(), 
			m_status->Constitution(), m_status->Intelligence(),
			m_status->Wisdom(), m_status->Charisma() );
		move( 23, 0 );
		printw("Dlvl: %d  $:%d  HP:%d(%d) Pw:%d(%d) AC:%d Xp:%d/%d",
			m_position.z, 0, m_status->HitPoints(),
			m_status->HitPointMax(), m_status->SpellPoints(),
			m_status->SpellPointMax(), 10, 
			m_status->ExperiencePoints(), m_status->Level() );
		
		if ( m_groupMemberCount > 1 )
		{
			printw("  Group Turns: %d/%d", m_groupMemberTurnCount, m_groupMemberCount);
			if ( !m_submittedTurn && m_groupMemberTurnCount > 0 )	// if somebody's entered a turn and I haven't..
				printw("  (Waiting for you)");
		}

		// do upper prompts.
		
		/*for ( int i = 0; i < MAX_MESSAGE_LINES; i++ )
		{
			move( i, 0 );
			printw("%s", m_messageBuffer[i]);
		}*/
		if ( m_messageDisplayLine < m_messageLines )
		{
			move( 0, 0 );
			printw("%s", m_messageBuffer[m_messageDisplayLine]);
			if ( m_awaitingMore )
			{
				move( 1, 0 );
				printw("--more--");
			}
		}
		
		if ( m_mode == MODE_Talking )
		{
			// draw our string in the top few lines..
			
			move( 2, 2 );
			printw("Say: %s", m_talkBuffer);
		}
		else
		{
			// now put our cursor back onto us, when in normal mode.
			move(m_position.y + 3,m_position.x);
		}
	
		refresh();
#endif
		m_needsRefresh = false;
	}
}

void
hnDisplayTTY::PostTurnSubmit()
{
	//--------------------------------------------------------
	//  We've just submitted a turn, so if we received a message
	//  during the last turn, push them back in the buffer.
	//--------------------------------------------------------

	m_messageDisplayLine = m_messageLines;
}
