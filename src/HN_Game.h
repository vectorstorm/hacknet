#ifndef __HN_GAME_H__
#define __HN_GAME_H__

// toplevel instance of a game.

#include "NET_ServerConsts.h"
#include "HN_Enum.h"
#include "HN_Point.h"

//#define MAX_LEVELS	(50)
#define MAX_LEVELS (2)

class mapBase;
class netServer;
class hnEntity;

struct playerData
{
	hnEntity	*entity;
	hnPoint		pos;
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
