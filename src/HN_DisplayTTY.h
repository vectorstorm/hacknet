#ifndef __HN_TTYDISPLAY_H__
#define __HN_TTYDISPLAY_H__

#include "HN_Enum.h"
#include "HN_Display.h"

class hnDisplayTTY : public hnDisplay
{
	bool		m_needsRefresh;
public:
			hnDisplayTTY();
	virtual		~hnDisplayTTY();
	virtual bool	Go();

	virtual void	Refresh();
	
	virtual void	PlotSquare( sint8 x, sint8 y );
	virtual void	UpdateLocation( const hnPoint &point );
	virtual void	UpdateMapTile( sint8 x, sint8 y, const hnMapTile &tile );
	virtual void	UpdateMapCreature( sint8 x, sint8 y, hnEntityType type );
};

void *	EventLoop(void *);	// for use by our thread to check for keypresses

#endif //__PH_TTYDISPLAY_H__
