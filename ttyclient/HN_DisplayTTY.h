#ifndef __HN_TTYDISPLAY_H__
#define __HN_TTYDISPLAY_H__

#include "HN_Display.h"

class hnDisplayTTY : public hnDisplay
{
	enum inputMode{
		MODE_Normal,		// business as usual
		MODE_FloorObjectDisplay,// displaying the objects on the floor
		MODE_FloorObjectSelect,// displaying the objects on the floor
		MODE_InventoryDisplay,	// looking at our inventory
		MODE_InventorySelect,	// selecting something from our inventory
		MODE_Talking		// we're in the middle of trying to speak.
	};
	enum inventorySelectMode{
		ISM_None,
		ISM_Take,
		ISM_Drop,
		ISM_Wield
	};

	inputMode		m_mode;
	inventorySelectMode	m_inventoryMode;

#define MAX_TALK_BYTES		(128)

#define MAX_MESSAGE_SCROLLBACK	(32)
#define MAX_MESSAGE_BYTES	(256)
	
	char		m_talkBuffer[MAX_TALK_BYTES];
	int		m_talkLength;
	
	char		m_messageBuffer[MAX_MESSAGE_SCROLLBACK][MAX_MESSAGE_BYTES];
	int		m_messageLines;
	int		m_messageDisplayLine;	// current line being displayed.  If > MAX_MESSAGE_SCROLLBACK, we display nothing.

	bool		m_awaitingMore;		// if true, we're waiting for a keypress to display the next line of text.
						// this is NOT an inputmode -- We only grab the next keypress if we're in
						// Normal mode.. otherwise, let the player finish whatever they're doing
						// before we steal a keypress from them!

	bool		m_needsRefresh;
	bool		m_done;

public:
			hnDisplayTTY(char * name);
	virtual		~hnDisplayTTY();
	virtual bool	Go();

	void		EventLoop();
	void		HandleKeypressNormal( int key );
	void		HandleKeypressMore( int key );
	void		HandleKeypressTalking( int key );
	void		HandleKeypressInventoryDisplay( int key );
	void		HandleKeypressInventorySelect( int key );

	void		HandleTake();
	void		HandleDrop();
	void		HandleWield();
	void		HandleInventory();

	void		DrawObjectArray(objDescription *array, uint8 count, bool inventory);
	
	virtual void	PostTurnSubmit();
	
	virtual void	Refresh();
	virtual void	DisplayItems();
	
	virtual void	PlotSquare( sint8 x, sint8 y );
	virtual void	UpdateLocation( const hnPoint &point );
	virtual void	UpdateMapTile( const hnPoint &point, const mapClientTile &tile );
	virtual void	UpdateMapCreature( const hnPoint &point, entType type );
	virtual void	UpdateGroupData( int groupMembers, int groupMembersWithTurns, bool haveISubmitted );

	virtual void	TextMessage( char * message );
};

void *	StartEventLoop(void *);		// for use by our thread to check for keypresses

#endif //__PH_TTYDISPLAY_H__
