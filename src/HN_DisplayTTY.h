#ifndef __HN_TTYDISPLAY_H__
#define __HN_TTYDISPLAY_H__

#include "HN_Display.h"

class hnDisplayTTY : public hnDisplay
{
	enum inputMode{
		MODE_Normal,	// business as usual
		MODE_Talking	// we're in the middle of trying to speak.
	};

	inputMode	m_mode;

#define MAX_TALK_BYTES		(128)

#define MAX_MESSAGE_LINES	(3)
#define MAX_MESSAGE_BYTES	(256)
	
	char		m_talkBuffer[MAX_TALK_BYTES];
	int		m_talkLength;
	
	char		m_messageBuffer[MAX_MESSAGE_LINES][MAX_MESSAGE_BYTES];
	int		m_messageLines;
	
	bool		m_needsRefresh;
	bool		m_done;
	
public:
			hnDisplayTTY(char * name);
	virtual		~hnDisplayTTY();
	virtual bool	Go();
	
	void		EventLoop();
	void		HandleKeypressNormal( int key );
	void		HandleKeypressTalking( int key );
	
	virtual void	Refresh();
	
	virtual void	PlotSquare( sint8 x, sint8 y );
	virtual void	UpdateLocation( const hnPoint &point );
	virtual void	UpdateMapTile( sint8 x, sint8 y, const mapClientTile &tile );
	virtual void	UpdateMapCreature( sint8 x, sint8 y, entType type );

	virtual void	TextMessage( char * message );
};

void *	StartEventLoop(void *);		// for use by our thread to check for keypresses

#endif //__PH_TTYDISPLAY_H__
