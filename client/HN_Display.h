#ifndef __HN_DISPLAY_H__
#define __HN_DISPLAY_H__

#include "HN_Enum.h"
#include "HN_Point.h"
#include "MAP_Client.h"
#include "ENT_Types.h"
#include "HN_Status.h"

#define MAX_NAME_BYTES (128)

class netClient;

class hnDisplay
{
protected:
	char		m_name[MAX_NAME_BYTES];
	netClient *	m_client;
	hnPoint		m_position;
	mapClient **	m_map;	// this is our map of all the levels...
	int		m_levelCount;

	hnStatus *	m_status;	// status of our player.

	int		m_groupMemberCount;		// how many members in my group.
	int		m_groupMemberTurnCount;		// how many members of my group have submitted turns so far.
	bool		m_submittedTurn;		// have I submitted my turn yet?
	bool		m_moved;			// true if we've moved since the last time we refreshed.
public:
			hnDisplay( char * name );
	virtual		~hnDisplay();

	void		SetClient( netClient *client ) { m_client = client; }
	virtual bool 	Go() = 0;  	// main event loop goes in here.  Return
					// true if we're alive and saving, false
					// if otherwise.
	
	bool		isMapReady( int mapID ) { return (m_map[mapID] != NULL); }
	
	void		MoveCommand( hnDirection dir );	// send move or attack, as appropriate
	void		WaitCommand();			// send wait command.
	virtual void	PostTurnSubmit() {}		// called after submittting a turn.

	virtual void	Refresh();
	
	virtual void	DungeonReset( sint8 levelCount );
	virtual void	MapReset( sint8 width, sint8 height, sint8 depth );
	virtual void	UpdateLocation( const hnPoint &point );
	virtual void	UpdateMapTile( const hnPoint &point, const mapClientTile & tile );
	virtual void	UpdateMapCreature( const hnPoint &point, entType type );
	virtual void	UpdateGroupData( int groupMemberCount, int groupMemberTurnCount, bool submittedTurn );
	virtual void	TextMessage( char * message ) {}

	char *		GetName() { return m_name; }

	hnStatus *	GetStatus() { return m_status; }
};

#endif
