#ifndef __HN_GAME_H__
#define __HN_GAME_H__

// toplevel instance of a game.

#include "HN_Config_Server.h"
#include "HN_Enum.h"
#include "HN_Point.h"

//#define MAX_LEVELS	(50)
#define MAX_LEVELS (2)

class mapBase;
class netServer;
class entPlayer;

struct playerData
{
	entPlayer	*entity;
	hnPoint		pos;
	mapBase		*m_visionMap[MAX_LEVELS];
};

// hnGame is a singleton.

class hnGame
{
	netServer 		*m_server;
	playerData		m_player[MAX_CLIENTS];
	mapBase			*m_levelMap[MAX_LEVELS];

	static hnGame *		s_instance;
	
				hnGame();
	virtual			~hnGame();
public:
	
	static void		Startup();
	static void		Shutdown();
	static hnGame *		GetInstance();
	

	
	void			SetServer(netServer *server) { m_server = server; }
	
	void			ClientJoined(int clientID);
	void			ClientQuit(int clientID);
	void			ClientMove(int clientID, hnDirection dir);
	void			ClientAttack(int clientID, hnDirection dir);
};

#endif
