#ifndef __NET_CLIENT_H__
#define __NET_CLIENT_H__

// Network class for HackNet.  Our network is set up this way:
//
//  (Game) -> (netClient) -> [internet] -> (netClient) -> (Client)

#include "HN_Enum.h"

class hnDisplay;
class netMetaPacket;
struct sockaddr_in;

#define MAX_BUFFER_SIZE (128)

class netClient
{
	hnDisplay *	m_display;
	int		m_socket;
	sockaddr_in *	m_serverAddress;
	
	netMetaPacket *	m_packet;
	char		m_buffer[MAX_BUFFER_SIZE];
	
	bool		m_done;
protected:
	
	void		cleanexit(int id);  // wrapper for 'exit()' to be sure our display has a chance to clean up after itself.
	void		StartClient(char * serverAddress);
	
public:
			netClient(hnDisplay *display, char * serverAddress);
			~netClient();
	
	void		Disconnect();
	
	void		StartMetaPacket();
	void		TransmitMetaPacket();
	
	void		Go();
	void		SendMove( hnDirection whichWay );
	void		SendQuit( bool save );
};

#endif	//__HN_NETCLIENT_H__
