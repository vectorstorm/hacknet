#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <assert.h>
#include <string.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include "NET_Packet.h"
#include "NET_Client.h"
#include "HN_Display.h"
#include "ENT_Base.h"
#include "HN_Enum.h"

#define HACKNET_PORT 		(9274)
#define MAX_DATA_SIZE		(100)	// how much data can we grab at once

//#define __DEBUGGING_NETWORK__

netClient::netClient(hnDisplay *display, char *serverAddress):
	m_display(display), 
	m_packet(NULL),
	m_done(false)
{
	m_serverAddress = new sockaddr_in;
	m_display->SetClient(this);
	StartClient(serverAddress);
}

netClient::~netClient()
{
	delete m_serverAddress;
}


void
netClient::StartClient( char * serverAddress )
{
	//int sock_fd;
	hostent 	*he;
	
	//printf("Doing host lookup...\n");
	if ( ( he = gethostbyname(serverAddress) ) == NULL ) // getting host information...
	{
		herror("gethostbyname");
		cleanexit(1);
	}
	
	
	//printf("Opening socket...\n");
	if ( (m_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("socket");
		cleanexit(1);
	}

	m_serverAddress->sin_family = AF_INET;				// host byte order
	m_serverAddress->sin_port = htons(HACKNET_PORT);		// short, network byte order
	m_serverAddress->sin_addr = *((in_addr *)he->h_addr);
	bzero(&(m_serverAddress->sin_zero),8);				// zero out the rest of the struct

	// okay, we're all ready to go now!  (But we haven't yet connected)
}

void
netClient::Go()
{
	mapClientTile 	tile;				// for use in map updates
	netMapTile tileData;
	netMapEntity entityData;
	netMapUpdateBBox bbox;
	netMapReset mapReset;
	netClientLocation clientLoc;
	hnPoint	point;
	sint8	levelCount;
	
	m_display->TextMessage("Trying to connect to server...\n");
	if ( connect( m_socket, (sockaddr *)m_serverAddress, sizeof(sockaddr) ) == -1 )
	{
		perror("connect");
		cleanexit(1);
	}
	m_display->TextMessage("Connected!\n");
	
	// Now that we're connected, send the server our name.

	SendName( m_display->GetName() );
	
	// now that we've connected, we wait for packets from the server or a key from our hnClient...
	
	
	m_display->Go();
	
	while(!m_done){
		// do stuff here until we quit.
		
		//netServerPacket packet;
		short incomingBytes;

		if ( recv( m_socket, &incomingBytes, sizeof(sint16), MSG_WAITALL ) == -1 )
		{
			perror("recv");
			cleanexit(1);
		}
		incomingBytes = ntohs(incomingBytes);

		//printf("Receiving %d bytes...\n", incomingBytes);
		char buffer[incomingBytes];
		
		if ( recv( m_socket, buffer, incomingBytes, MSG_WAITALL ) == -1 )
		{
			perror("recv");
			cleanexit(1);
		}
#ifdef __DEBUGGING_NETWORK__	
		printf("Packet of %d bytes:\n", incomingBytes);
		char *bufferstart = buffer;
		
		for ( int i = 0; i < incomingBytes; i++ )	
		{
			printf("Value: %d\n",*(bufferstart + i));
		}
		printf("\n");
		
//		if ( incomingBytes == 7 )
//			while ( 1 ) {}
#endif
		netMetaPacketInput *packet = new netMetaPacketInput(buffer, incomingBytes);
		
		while ( !packet->Done() )
		{
			sint8 type = packet->PeekChar();
#define MAX_MESSAGE_BYTES	(256)
			char	messageBuffer[MAX_MESSAGE_BYTES];
			sint16 	messageBufferLength = MAX_MESSAGE_BYTES;
			
			switch ( type )
			{
				case SPT_ClientLocation:
					packet->ClientLocation(clientLoc);
					m_display->UpdateLocation( clientLoc.loc );
					//printf("Client location packet\n");
					//printf("loc: %d,%d,%d\n",clientLoc.loc.x,clientLoc.loc.y,clientLoc.loc.z);
					break;
				case SPT_MapTile:
					//printf("Map tile packet\n");
					packet->MapTile(tileData);
					if ( m_display->isMapReady( tileData.loc.z ) )
					{
						tile.material = (hnMaterialType)tileData.material;
						tile.wall = (hnWallType)tileData.wall;
						m_display->UpdateMapTile(tileData.loc, tile);
					}
					break;
				case SPT_Message:
					packet->TextMessage(messageBuffer, messageBufferLength);
					m_display->TextMessage(messageBuffer);
					break;
				case SPT_MapEntity:
					packet->MapEntity(entityData);
					
					if ( m_display->isMapReady( bbox.loc.z ) )
						m_display->UpdateMapCreature(entityData.loc, entityData.objectType);
					break;
				case SPT_DungeonReset:
					packet->DungeonReset(levelCount);
					m_display->DungeonReset(levelCount);
				case SPT_MapReset:
					packet->MapReset(mapReset);
					m_display->MapReset(mapReset.width, mapReset.height, mapReset.depth);
					break;
				case SPT_MapUpdateBBox:
					//printf("Map bbox update packet\n");
					packet->MapUpdateBBox(bbox);
					//mapClientTile tile;
					
					if ( !m_display->isMapReady( bbox.loc.z ) )
					{
						SendRefreshRequest( bbox.loc.z );
						break;
					}
					
					
					for ( int i = 0; i < bbox.width; i++ )
						for ( int j = 0; j < bbox.height; j++ )
						{
							tile.material = bbox.material[i+(j*bbox.width)];
							tile.wall = bbox.wall[i+(j*bbox.width)];
							point.Set(bbox.loc.x+i, bbox.loc.y+j, 0);
							tile.entity = bbox.entityType[i+(j*bbox.width)];
							m_display->UpdateMapTile( hnPoint(bbox.loc.x+i, bbox.loc.y+j, bbox.loc.z), tile);
						}

					delete [] bbox.material;
					delete [] bbox.wall;
					delete [] bbox.entityType;
					break;
				case SPT_BadPacketNotice:
					delete m_display;
					packet->Char(type);
					printf("Server killed us for sending an unknown packet.  Version mismatch?\n");
					m_done = true;
					//exit(1);
					break;
				case SPT_QuitConfirm:
					packet->Char(type);
					m_done = true;
					break;
				default:
					printf("Unknown packet type %d!!  :(\n", type);
					break;
			}
		}
		m_display->Refresh();	// do a screen refresh if we need it.
	}
	close( m_socket );
}

void
netClient::StartMetaPacket()
{
	assert( m_packet == NULL );
	m_packet = new netMetaPacketOutput( m_buffer, MAX_BUFFER_SIZE );
}

void
netClient::TransmitMetaPacket()
{
	assert( m_packet );

	short metapacketdatalength = htons(m_packet->GetBufferLength());
	
	if ( send(m_socket, &metapacketdatalength, sizeof(sint16), MSG_NOSIGNAL) == -1 )
		perror("send");

	if ( send(m_socket, m_packet->GetBuffer(), m_packet->GetBufferLength(), MSG_NOSIGNAL ) == -1 )
		perror("send");

	delete m_packet;
	m_packet = NULL;
}

void
netClient::SendQuit( bool save )
{	
	StartMetaPacket();
	m_packet->ClientQuit();
	TransmitMetaPacket();
}

void
netClient::SendMove(hnDirection dir)
{
	sint8 direction = dir;
	StartMetaPacket();
	m_packet->ClientMove(direction);
	TransmitMetaPacket();
}

void
netClient::SendName(char * name)
{
	sint16 nameLength = strlen(name);
	StartMetaPacket();
	m_packet->ClientName(name, nameLength);
	TransmitMetaPacket();
}

void
netClient::SendRefreshRequest( sint8 level )
{
	StartMetaPacket();
	m_packet->ClientRequestRefresh(level);
	TransmitMetaPacket();
}

void
netClient::SendTalk(char * talk)
{
	sint16 talkLength = strlen(talk);
	StartMetaPacket();
	m_packet->ClientTalk(talk, talkLength);
	TransmitMetaPacket();
}

void
netClient::Disconnect()
{
	//printf("Disconnecting...\n");
	m_done = true;
}

void netClient::cleanexit(int id)
{
	delete m_display;
	exit(id);
}
