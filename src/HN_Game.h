#ifndef __HN_GAME_H__
#define __HN_GAME_H__

// toplevel instance of a game.

#include "HN_Config_Server.h"
#include "HN_Enum.h"
#include "HN_Point.h"

//#define MAX_LEVELS	(50)
//#define MAX_LEVELS (2)

//class mapBase;
class netServer;
class entPlayer;

// hnGame is a singleton.

class hnGame
{
	entPlayer *		m_player[MAX_CLIENTS];

	static hnGame *		s_instance;
	
				hnGame();
	virtual			~hnGame();
public:
	
	static void		Startup();
	static void		Shutdown();
	static hnGame *		GetInstance();
	
	char *			GetPlayerName(int clientID);	// utility function for server

	
	void			ClientJoined(int clientID);
	void			ClientName(int clientID, char * name);
	void			ClientTalk(int clientID, char * talk);
	void			ClientRequestRefresh( int clientID, int level );
	void			ClientQuit(int clientID);
	void			ClientMove(int clientID, hnDirection dir);
	void			ClientAttack(int clientID, hnDirection dir);
};

#endif
