	
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <assert.h>
#include <signal.h>
#include <string.h> 
#include <strings.h>
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
#include "OBJ_Base.h"

#define HACKNET_PORT 		(9274)
#define MAX_CONNECTIONS		(16)

//#define __DEBUG_NETWORKING__
//#define __DETAIL_DEBUG_NETWORKING__
//#define __DISPLAY_PACKET_CONTENT__

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
		m_client[i].incomingPacketSizeSize = 0;
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
	bzero((char *)&(m_localAddress->sin_zero),8);			// zero out the rest of the struct
	
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

	if ( signal( SIGPIPE, SIG_IGN ) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
}

void
netServer::Go()
{
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
		
		socklen_t sin_size = sizeof(sockaddr_in);
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
					if ( m_client[i].incomingPacketSizeSize == 0 )
						m_client[i].incomingPacketSizeSize = sizeof(sint16);
					
					char * packetSizeBufferPointer = (char *)&m_client[i].incomingPacketSizeBuffer;

#ifdef __DETAIL_DEBUG_NETWORKING__
					printf("Waiting for %d bytes of packet size data.\n", m_client[i].incomingPacketSizeSize);
#endif

					int bytesRead = recv(m_client[i].socket, packetSizeBufferPointer, m_client[i].incomingPacketSizeSize, 0);	// first a short saying how many bytes of data are coming..
					
#ifdef __DETAIL_DEBUG_NETWORKING__
					printf("Received %d bytes of packet size data.\n", bytesRead);
#endif
					packetSizeBufferPointer += bytesRead;
					m_client[i].incomingPacketSizeSize -= bytesRead;

					if ( m_client[i].incomingPacketSizeSize <= 0 )
					{
						m_client[i].packetFullSize = m_client[i].incomingPacketSize = ntohs( m_client[i].incomingPacketSizeBuffer );
#ifdef __DETAIL_DEBUG_NETWORKING__						
						printf("We're going to wait for %d bytes of data in total.\n",m_client[i].incomingPacketSize);
#endif		
						m_client[i].incomingPacketSizeSize = 0;
					
					
					//	printf("  Next packet will have size %d.\n", packetSize);
					//	printf("  Max legal packet size is: %d.\n", sizeof(netServerPacket) );
						if ( m_client[i].incomingPacketSize > MAX_CLIENT_PACKET_SIZE )
						{
							// aiee!  Illegally large packet!  Kill the client!
							printf("  Packet size data illegal!  Killing client.\n");
							SendBadPacketNotice(i);
							DisconnectClientID(i);
						}
					}
				}
				else
				{

#ifdef __DETAIL_DEBUG_NETWORKING__
					printf("Waiting for %d bytes of packet data.\n", m_client[i].incomingPacketSize);
#endif
					int numBytesReceived = recv(m_client[i].socket, m_client[i].packetRecv, m_client[i].incomingPacketSize, 0);
					
					if ( numBytesReceived <= 0 )
					{
						printf("  Error:  recv returned %d!\n", numBytesReceived);
						// aiee!  Illegal packet!  Kill the client!
						printf("  Error receiving packet!  Killing client.\n");
						SendBadPacketNotice(i);
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
							m_client[i].packetRecv = m_client[i].packet;
#ifdef __DEBUG_NETWORKING__
							printf("  Received %d byte packet from %s (client id %d).\n", m_client[i].packetFullSize, m_game->GetPlayerName(i), i);
#endif
							if ( !ProcessClientPacket(i, m_client[i].packet, m_client[i].packetFullSize) )
							{
								// aiee!  Illegal packet!  Kill the client!
								printf("  Packet data illegal!  Killing client.\n");
								SendBadPacketNotice(i);
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
netServer::ProcessClientPacket(int clientID, char *buffer, short incomingBytes)
{
	bool okay = true;

#define MAX_NAME_BYTES (128)
	char localbuffer[MAX_NAME_BYTES];
	sint16 bufferSize = MAX_NAME_BYTES;
	netClientTake take;
	netInventoryItem item;
	
	assert(clientID >= 0 && clientID < MAX_CLIENTS);
#ifdef __DISPLAY_PACKET_CONTENT__
	printf("Received %d bytes.\n", incomingBytes );

	for ( int i = 0; i < incomingBytes; i++ )
	{
		printf("Byte %d: %d\n", i, buffer[i] );
	}
#endif
	netMetaPacketInput *packet = new netMetaPacketInput(buffer, incomingBytes);
	
	while ( !packet->Done() && okay )
	{
		sint8 type = packet->PeekSint8();
		sint8 direction;
		sint8 level;
		
		switch(type)
		{
			case CPT_Move:
				okay = packet->ClientMove(direction);
				m_game->ClientMove(clientID, (hnDirection)direction);
				break;
			case CPT_Attack:
				okay = packet->ClientAttack(direction);
				m_game->ClientAttack(clientID, (hnDirection)direction);
				break;
			case CPT_Wait:
				okay = packet->ClientWait();
				m_game->ClientWait(clientID);
				break;
			case CPT_Talk:
				okay = packet->ClientTalk(localbuffer, bufferSize);
				m_game->ClientTalk(clientID, localbuffer, bufferSize);
				break;
			case CPT_TakeObject:
				okay = packet->ClientTake(take);
				m_game->ClientTake(clientID, take.object, take.stackID);
				break;
			case CPT_DropObject:
				okay = packet->ClientDrop(item);
				m_game->ClientDrop(clientID, item.object, item.inventorySlot);
				break;
			case CPT_WieldObject:
				okay = packet->ClientWield(item);
				m_game->ClientWield(clientID, item.object, item.inventorySlot);
				break;
			case CPT_WearObject:
				okay = packet->ClientWear(item);
				m_game->ClientWear(clientID, item.object, item.inventorySlot);
				break;
			case CPT_RemoveObject:
				okay = packet->ClientRemove(item);
				m_game->ClientRemove(clientID, item.object, item.inventorySlot);
				break;
			case CPT_QuaffObject:
				okay = packet->ClientQuaff(item);
				m_game->ClientQuaff(clientID, item.object, item.inventorySlot);
				break;
			case CPT_EatObject:
				okay = packet->ClientEat(item);
				m_game->ClientEat(clientID, item.object, item.inventorySlot);
				break;
			case CPT_Name:
				okay = packet->ClientName(localbuffer, bufferSize);
				m_game->ClientName(clientID, localbuffer, bufferSize);
				printf("Client %d calls himself %s\n", clientID, localbuffer);
				break;
			case CPT_RequestRefresh:
				okay = packet->ClientRequestRefresh(level);
				printf("Client %d requests refresh of level %d.\n", clientID, level );
				m_game->ClientRequestRefresh(clientID, level);
				break;
			case CPT_Save:	// no saving code yet -- just quit.
				okay = packet->ClientSave();
				SendQuitConfirm(clientID);
				DisconnectClientID(clientID);
				printf("Disconnected client %d.\n", clientID);
				break;
			case CPT_Quit:
				okay = packet->ClientQuit();
				SendQuitConfirm(clientID);
				DisconnectClientID(clientID);
				printf("Disconnected client %d.\n", clientID);
				break;
			default:
				okay = false;
				printf("Received unknown packet type %d from %s (id %d).\n", type, m_game->GetPlayerName(clientID), clientID);
				break;
		}
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
netServer::SendStatus( hnStatus * status )
{
	assert( m_metaPacket );
	status->SendChanges( m_metaPacket );
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
netServer::SendInventory( netInventory &inven )
{
	assert( m_metaPacket );

	m_metaPacket->Inventory(inven);
}

void
netServer::SendInventoryItem( const objDescription &desc, int id )
{
	assert( m_metaPacket );
	
	netInventoryItem packet;
	packet.object = desc;
	packet.inventorySlot = id;
	
	m_metaPacket->InventoryItem(packet);
}

void
netServer::SendGroupData( int groupMemberCount, int groupMemberTurns, bool clientSubmitted )
{
	netGroupData packet;
	packet.memberCount = groupMemberCount;
	packet.memberTurns = groupMemberTurns;
	packet.haveTurnFromClient = clientSubmitted;
	m_metaPacket->GroupData( packet );
}

void
netServer::SendDungeonReset( sint8 levelCount )
{
	m_metaPacket->DungeonReset( levelCount );
}

void
netServer::SendMapReset( int width, int height, int depth )
{
	netMapReset packet;
	packet.width = width;
	packet.height = height;
	packet.depth = depth;
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
netServer::SendDroppedItem( const objDescription &desc )
{
	netInventoryItem packet;
	packet.object = desc;
	packet.inventorySlot = 0;

	m_metaPacket->DroppedItem(packet);
}

void
netServer::SendTakenItem( const objDescription &desc, int inventorySlot )
{
	netInventoryItem packet;
	packet.object = desc;
	packet.inventorySlot = inventorySlot;

	m_metaPacket->TakenItem(packet);
}

void
netServer::SendWieldedItem( const objDescription &desc, int inventorySlot )
{
	netInventoryItem packet;
	packet.object = desc;
	packet.inventorySlot = inventorySlot;

	m_metaPacket->WieldedItem(packet);
}


void
netServer::SendMessage( char * message )
{
	sint16 messageLength = strlen(message);
	m_metaPacket->TextMessage( message, messageLength );
}

void
netServer::SendObjectStats( uint16 objectCount )
{
	m_metaPacket->ObjectStats( objectCount );
}

void
netServer::SendObjectName( uint16 objectID, uint16 type, char * name )
{
	sint16 messageLength = strlen(name);
	m_metaPacket->ObjectName(objectID, type, name, messageLength);
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
	m_game->ClientQuit(clientID);
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
		if ( m_metaPacket->GetBufferLength() > 0 )
		{
			// here we go -- send our netMetaPacket to the given client!
			short metapacketdatalength = htons(m_metaPacket->GetBufferLength());	

#ifdef __DEBUG_NETWORKING__
			printf("Sending %ld byte metapacket to %s (id %d)...\n", m_metaPacket->GetBufferLength(), 
									m_game->GetPlayerName(m_packetClientID), m_packetClientID);
#endif
		
			if ( send(m_client[m_packetClientID].socket, &metapacketdatalength, sizeof(sint16), 0 ) == -1 )
			{
			// something's gone wrong here -- presumably the client has disconnected from us
			// without warning, so first be sure our socket is closed, then disconnect.
				close( m_client[m_packetClientID].socket );
				DisconnectClientID(m_packetClientID);
			}

#ifdef __DISPLAY_PACKET_CONTENT__
			for ( int i = 0; i < m_metaPacket->GetBufferLength(); i++ )
			{
				printf("Value: %d\n", m_metaPacket->GetBuffer()[i]);
			}
#endif
			if ( send(m_client[m_packetClientID].socket, m_metaPacket->GetBuffer(), m_metaPacket->GetBufferLength(), 0 ) == -1 )
			{
				// something's gone wrong here -- presumably the client has disconnected from us
				// without warning, so first be sure our socket is closed, then disconnect.
				close( m_client[m_packetClientID].socket );
				DisconnectClientID(m_packetClientID);
			}
			success = true;
		}
		else
		{
		//	We don't actually care about this case..  it's perfectly legal to try to send a
		//	zero-length metapacket -- we'll just quietly not send it.  (This makes network
		//	logic a lot simpler elsewhere in the codebase)
		//	
		//	printf("Illegal metapacket length %d!\n",m_metaPacket->GetBufferLength());
		}
		
		delete m_metaPacket;
		m_metaPacket = NULL;
	}
	else
	{
		printf("Illegal request to send non-existant metapacket!\n");
	}
	
	return success;
}
