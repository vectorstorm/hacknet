#ifndef __NET_SERVER_H__
#define __NET_SERVER_H__

// Network class for HackNet.  Our network is set up this way:
//
//  (Game) -> (netServer) -> [internet] -> (netClient) -> (Client)

#include "HN_Enum.h"
#include "NET_ServerConsts.h"
#include "NET_Packet.h"

class hnMapTile;
class hnGame;
class hnMap;
class hnPoint;
struct sockaddr_in;
struct netClientPacket;

struct clientData {
	
	int		socket;
	netClientPacket	packet;			// construction area for incoming packets
	char *		packetRecv;		// pointer into 'packet' member above, where incoming packet data should be appended.
	int		incomingPacketSize;	// how many more bytes of packet data to read before packet is finished
};

#define MAX_META_PACKET_SIZE (512)

class netServer
{
	int			m_socket;
	sockaddr_in *		m_localAddress;
	hnGame *		m_game;					// the game we're serving
	
	clientData		m_client[MAX_CLIENTS];				// socket fds for all our clients
	int			m_clientCount;					// how many clients have we got?
	
	netMetaPacket *		m_metaPacket;
	char			m_buffer[MAX_META_PACKET_SIZE];
	int			m_packetClientID;				// client we're currently making a metapacket for
	
	static netServer *	s_instance;
	
	
protected:
				netServer();					// constructs and starts listening on our port..
	virtual			~netServer();
	void		StartServer();
	bool		ProcessClientPacket(int clientID, const netClientPacket &packet); // false == invalid packet, so kill the connection.
public:
	static void		Startup();
	static void		Shutdown();
	static netServer * 	GetInstance();
	
	void		Go();						// wait for connections and send data to our game.
	
	void		DisconnectClientID(int id);			// disconnect the client with the given ID number
	void		ProcessMove();					// process an incoming packet of data
	
	bool		StartMetaPacket( int clientID );		// who will eventually receive this packet?
	bool		TransmitMetaPacket();				// send the metapacket we were constructing
	
	void		SendClientLocation( hnPoint *loc );	// tell the client where they are.
//	bool		SendMapUpdate(int clientID, hnMap *map);	// server sends move update to client
	void		SendMapTile( hnPoint *loc, const hnMapTile & floorType );  // new floortype
	void		SendMapUpdateBBox( netMapUpdateBBox *bbox );
	void		SendMapObjectList( hnPoint *loc, int numObjects, hnEntityType floorType );  // change to list of objects on this square.  # of objects in pile, and topmost object type are sent to client.
	
	void		SendQuitConfirm( int clientID );	// doesn't require a metapacket
	void		SendBadPacketNotice( int clientID );	// doesn't require a metapacket
};

#endif	//__HN_NETSERVER_H__
