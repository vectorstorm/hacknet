	
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <assert.h>
#include <string.h> 
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <arpa/inet.h>
#include "NET_Server.h"
#include "NET_Packet.h"
#include "HN_Game.h"
#include "MAP_Base.h"

#define HACKNET_PORT 		(9274)
#define MAX_CONNECTIONS		(16)

netServer * netServer::s_instance = NULL;

void
netServer::Startup()
{
	assert( s_instance == NULL );
	s_instance = new netServer;
}

void
netServer::Shutdown()
{
	assert( s_instance );
	delete s_instance;
	s_instance = NULL;
}

netServer *
netServer::GetInstance()
{
	assert( s_instance );
	return s_instance;
}

netServer::netServer():
	m_clientCount(0),
	m_metaPacket(NULL)
{
	m_game = hnGame::GetInstance();

	for ( int i = 0; i < MAX_CLIENTS; i++ )
	{
		m_client[i].socket = -1;
		m_client[i].packetRecv = (char *)&m_client[i].packet;
		m_client[i].incomingPacketSize = 0;
	}
	
	m_localAddress = new sockaddr_in;
	
	StartServer();
}

netServer::~netServer()
{
	delete m_localAddress;
}


void
netServer::StartServer()
{
	//int sock_fd;
	//sockaddr_in my_addr;
	
	printf("Getting socket...\n");
	if ( (m_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("socket");
		exit(1);
	}

	m_localAddress->sin_family = AF_INET;			// host byte order
	m_localAddress->sin_port = htons(HACKNET_PORT);		// short, network byte order
	m_localAddress->sin_addr.s_addr = INADDR_ANY;		// auto-fill with my IP
	bzero(&(m_localAddress->sin_zero),8);			// zero out the rest of the struct
	
	printf("Binding socket to port %d...\n", HACKNET_PORT);
	if ( bind( m_socket, (sockaddr *)m_localAddress, sizeof(sockaddr)) < 0 )
	{
		perror("bind");
		exit(1);
	}
	
	printf("Listening to socket..\n");
	if ( listen( m_socket, MAX_CONNECTIONS ) == -1 )
	{
		perror("listen");
		exit(1);
	}
}

void
netServer::Go()
{
	int newSocket;
	sockaddr_in their_addr;
	// now we start listening for connections on our port, and send data to the game..
	bool exiting = false;
	fd_set open_sockets;
	timeval waitTime;
	
	
	while ( !exiting )
	{
		int retval = 0;
		while ( retval <= 0 )
		{
			waitTime.tv_sec = 60;			// wait 60 seconds, or until somebody sends us some data...
			waitTime.tv_usec = 0;
			int maxOpenSocket = m_socket + 1;
		
			FD_ZERO(&open_sockets);
			FD_SET(m_socket, &open_sockets);
			for ( int i = 0; i < MAX_CLIENTS; i++ )
			{
				if ( m_client[i].socket != -1 )
				{
					FD_SET( m_client[i].socket, &open_sockets);
					if ( m_client[i].socket >= maxOpenSocket )
						maxOpenSocket = m_client[i].socket+1;
				}
			}
		
		
			retval = select(maxOpenSocket, &open_sockets, NULL, NULL, &waitTime);
			
			
			if ( retval == -1 )
			{
				printf("select error caught: %d", errno);
			}
		}
		
		unsigned int sin_size = sizeof(sockaddr_in);
		//printf("Got socket stuff... \n");
		if ( FD_ISSET(m_socket, &open_sockets) )	// we have a new client
		{
			if ( m_clientCount >= MAX_CLIENTS )
			{
				printf("Server got connect from %s", inet_ntoa(their_addr.sin_addr));
				printf("but too many clients connected!\n");
				int refusedSocket = accept( m_socket, (sockaddr *)&their_addr, &sin_size );
				close(refusedSocket);
			}
			else 
			{
				bool okay = false;
				int i;
				for (i = 0; i < MAX_CLIENTS; i++ )
				{
					if ( m_client[i].socket == -1 )
					{
						okay = true;
						break;
					}
				}
				if ( !okay )
				{
					printf("No sockets available??? Exiting...\n");
					exit(1);
				}
				else if ( (m_client[i].socket = accept( m_socket, (sockaddr *)&their_addr, &sin_size )) == -1 )
				{
					perror("accept");
					printf("Something went wrong while trying to accept connection.  Ignoring it.\n");
					//exit(1);
				}
				else
				{
					printf("Server got connect from %s\n", inet_ntoa(their_addr.sin_addr));
					m_game->ClientJoined(i);
					m_clientCount++;
				}
			}
		}

		for ( int i = 0; i < MAX_CLIENTS; i++ )
		{
			if ( m_client[i].socket != -1 && FD_ISSET(m_client[i].socket, &open_sockets) )
			{
				//printf("Received packet from client ID %d, waiting for %d bytes of data.\n",i, m_client[i].incomingPacketSize);
				
				if ( m_client[i].incomingPacketSize == 0 )	// new incoming packet
				{
					char packetSize;
					recv(m_client[i].socket, &packetSize, 1, 0);	// first a byte saying how many bytes of data are coming..
				//	printf("  Next packet will have size %d.\n", packetSize);
				//	printf("  Max legal packet size is: %d.\n", sizeof(netServerPacket) );
					if (packetSize > sizeof(netClientPacket))
					{
						// aiee!  Illegal packet!  Kill the client!
						printf("  Packet size data illegal!  Killing client.\n");
						SendBadPacketNotice(i);
						m_game->ClientQuit(i);
						DisconnectClientID(i);
					}
					else
					{
						m_client[i].incomingPacketSize = packetSize;
					}
				}
				else
				{
					int numBytesReceived = recv(m_client[i].socket, m_client[i].packetRecv, m_client[i].incomingPacketSize, 0);
					
					if ( numBytesReceived <= 0 )
					{
						// aiee!  Illegal packet!  Kill the client!
						printf("  Error receiving packet!  Killing client.\n");
						SendBadPacketNotice(i);
						m_game->ClientQuit(i);
						DisconnectClientID(i);
					}
					else
					{
					
					m_client[i].packetRecv += numBytesReceived;
					m_client[i].incomingPacketSize -= numBytesReceived;
					// we've gotten a packet from one of our clients...
					
				//	printf("  Received %d bytes of data.  %d bytes remain to be received.\n", numBytesReceived, m_client[i].incomingPacketSize );
					
					if ( m_client[i].incomingPacketSize <= 0 )
					{
						assert(m_client[i].incomingPacketSize == 0);
						m_client[i].incomingPacketSize = 0;
						m_client[i].packetRecv = (char *)&m_client[i].packet;
				//		printf("  Processing packet.\n");
						if ( !ProcessClientPacket(i, m_client[i].packet) )
						{
							// aiee!  Illegal packet!  Kill the client!
							printf("  Packet data illegal!  Killing client.\n");
							SendBadPacketNotice(i);
							m_game->ClientQuit(i);
							DisconnectClientID(i);
						}
					}
					}
				}
			}
		}
	}
}

bool
netServer::ProcessClientPacket(int clientID, const netClientPacket &packet)
{
	bool okay = false;
	assert(clientID >= 0 && clientID < MAX_CLIENTS);
	
	sint8 type = packet.type;
	
	switch(type)
	{
		case CPT_Move:
			m_game->ClientMove(clientID, (hnDirection)packet.data.move.direction);
			okay = true;
			break;
		case CPT_Save:	// no saving code yet -- just quit.
		case CPT_Quit:
			SendQuitConfirm(clientID);
			m_game->ClientQuit(clientID);
			DisconnectClientID(clientID);
			printf("Disconnected client %d\n",clientID);
			okay = true;
			break;
		default:
			printf("Received unknown packet type %d from client %d.\n", type, clientID);
			break;
	}

	return okay;
}


void
netServer::SendClientLocation( const hnPoint & loc )
{
	assert( m_metaPacket );
	netClientLocation packet;
	packet.loc = loc;
	m_metaPacket->ClientLocation(packet);
}

void
netServer::SendMapTile( const hnPoint & loc, const mapTile & tile )
{
	assert( m_metaPacket );
	netMapTile packet;
	packet.loc = loc;
	packet.material = tile.material;
	packet.wall = tile.wall;
	m_metaPacket->MapTile(packet);
}

void
netServer::SendMapReset( int width, int height )
{
	netMapReset packet;
	packet.width = width;
	packet.height = height;
	m_metaPacket->MapReset( packet );
}

void
netServer::SendMapUpdateBBox( netMapUpdateBBox & bbox )
{
	// note -- netMapUpdateBBox isn't 'const' here because it gets passed verbatim
	// to the metapacket code, and the netInputMetapacket needs non-const references
	// in order to read data out of the packet..  yes, we're using a netOutputMetaPacket
	// here, but in order to use the same piece of code for reading data in and out
	// of the packet, it still needs to be non-const.  Deal with it.  ;)
	//
	// Regardless, we do NOT want to change the contents of this structure!
	
	assert( m_metaPacket );
	m_metaPacket->MapUpdateBBox(bbox);
}

//  remnant of old object list format -- # objects on square and type of top object
void
netServer::SendMapObjectList( const hnPoint & loc, int numObjects, entType objecttype )
{
	/*
	Unimplemented
	
	assert(m_meatPacket);
	netMapObjectList packet;
	packet.loc = loc;
	packet.numObjects = numObjects;
	packet.objectType = objecttype;
	
	m_metaPacket->MapObjectList(packet);*/
}


void
netServer::SendQuitConfirm(int clientID)
{
	StartMetaPacket(clientID);
	m_metaPacket->QuitConfirm();
	TransmitMetaPacket();
}



void
netServer::SendBadPacketNotice(int clientID)
{
	StartMetaPacket(clientID);
	m_metaPacket->BadPacketNotice();
	TransmitMetaPacket();
}



void
netServer::DisconnectClientID(int clientID)
{
	m_client[clientID].socket = -1;
	m_client[clientID].incomingPacketSize = 0;
	m_client[clientID].packetRecv = (char *)&m_client[clientID].packet;
	m_clientCount--;
}

bool
netServer::StartMetaPacket(int clientID)
{
	bool success = false;
	assert( m_metaPacket == NULL );		// we should never be constructing two at once!

	if ( !m_metaPacket )
	{
//		static char buffer[MAX_META_PACKET_SIZE];
		m_metaPacket = new netMetaPacketOutput( m_buffer, MAX_META_PACKET_SIZE );
		m_packetClientID = clientID;
		success = true;
	}

	return success;
}

bool
netServer::TransmitMetaPacket()
{
	bool success = false;
	assert( m_metaPacket );

	if ( m_metaPacket )
	{
		// here we go -- send our netMetaPacket to the given client!
		short metapacketdatalength = htons(m_metaPacket->GetBufferLength());	
		
		printf("Sending %d byte metapacket to client %d...\n", m_metaPacket->GetBufferLength(), m_packetClientID);
		
		
		if ( send(m_client[m_packetClientID].socket, &metapacketdatalength, sizeof(sint16), MSG_NOSIGNAL) == -1 )
			perror("send");
		
/*		for ( int i = 0; i < m_metaPacket->GetBufferLength(); i++ )
		{
			printf("Value: %d\n", m_metaPacket->GetBuffer()[i]);
		}*/
		
		if ( send(m_client[m_packetClientID].socket, m_metaPacket->GetBuffer(), m_metaPacket->GetBufferLength(), MSG_NOSIGNAL) == -1 )
			perror("send");
		
		delete m_metaPacket;
		m_metaPacket = NULL;

		success = true;
	}
	
	return success;
}
