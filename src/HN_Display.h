#ifndef __HN_DISPLAY_H__
#define __HN_DISPLAY_H__

#include "HN_Enum.h"
#include "HN_Point.h"
#include "HN_Map.h"

class netClient;
class hnMap;

class hnDisplay
{
protected:
	netClient 	*m_client;
	hnPoint		m_position;
	hnMap		*m_map;		// this is our map of the current level...
public:
			hnDisplay();
	virtual		~hnDisplay();

	void		SetClient( netClient *client ) { m_client = client; }
	virtual bool 	Go() = 0;  	// main event loop goes in here.  Return
					// true if we're alive and saving, false
					// if otherwise.
	
	virtual void	Refresh() {}
	
	virtual void	UpdateLocation( const hnPoint &point ) { m_position = point; }
	virtual void	UpdateMapTile( sint8 x, sint8 y, const hnMapTile & tile );
	virtual void	UpdateMapEntity( sint8 x, sint8 y, hnObjectType type );
};

#endif
