#ifndef __HN_GAME_H__
#define __HN_GAME_H__

#include "HN_Config_Server.h"
#include "HN_Enum.h"
#include "HN_Point.h"
#include "OBJ_Types.h"

class netServer;
class entBase;
class hnPlayer;

// hnGame is a singleton.
class hnGame
{
	hnPlayer *		m_player[MAX_CLIENTS];

	static hnGame *		s_instance;
	
				hnGame();
	virtual			~hnGame();
public:
	
	static void		Startup();
	static void		Shutdown();
	static hnGame *		GetInstance();
	
	char *			GetPlayerName(int clientID);	// utility function for server

	void			SeenEvent( entBase *entity, char * message );
	void			SeenEvent( const hnPoint & where, char * message );

	void			ClientTurn();
	void			ClientJoined(int clientID);
	void			ClientName(int clientID, char * name, int nameBufferLen);
	void			ClientTalk(int clientID, char * talk, int talkBufferLen);
	void			ClientTake(int clientID, objDescription &desc, uint8 stackID );
	void			ClientDrop(int clientID, objDescription &desc, uint8 inventorySlot );
	void			ClientWield(int clientID, objDescription &desc, uint8 inventorySlot );
	void			ClientWear(int clientID, objDescription &desc, uint8 inventorySlot );
	void			ClientRemove(int clientID, objDescription &desc, uint8 inventorySlot );
	void			ClientQuaff(int clientID, objDescription &desc, uint8 inventorySlot );
	void			ClientRequestRefresh( int clientID, int level );
	void			ClientQuit(int clientID);
	void			ClientMove(int clientID, hnDirection dir);
	void			ClientWait(int clientID);
	void			ClientAttack(int clientID, hnDirection dir);
};

#endif
