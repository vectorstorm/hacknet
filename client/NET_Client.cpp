#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <assert.h>
#include <string.h> 
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include "NET_Packet.h"
#include "NET_Client.h"
#include "HN_Display.h"
#include "HN_Enum.h"

#define HACKNET_PORT 		(9274)
#define MAX_DATA_SIZE		(100)	// how much data can we grab at once

//#define __DEBUGGING_NETWORK__
//#define __DISPLAY_PACKET_CONTENT__

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
#if HAS_HERROR
		herror("gethostbyname");
#endif
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
	bzero((char *)&(m_serverAddress->sin_zero),8);				// zero out the rest of the struct

	// okay, we're all ready to go now!  (But we haven't yet connected)

	if ( signal( SIGPIPE, SIG_IGN ) == SIG_ERR )
	{
		perror("signal");
		exit(1);
	}
}

void
netClient::Go()
{
	mapClientTile 	tile;				// for use in map updates
	netMapTile tileData;
	netMapEntity entityData;
	netMapUpdateBBox bbox;
	netMapReset mapReset;
	netGroupData groupData;
	netClientLocation clientLoc;
	hnPoint	point;
	sint8	levelCount;
	netInventory inven;
	netInventoryItem item;
	objDescription *objList;
	uint16	objectID;
	objType objectType;
	
	
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
		
		short 	readSizeBytesLeft = sizeof(sint16);
		short 	incomingBytes;
		char *	incomingBuffer = (char *)&incomingBytes;
		
		//  Emulate functionality of MSG_WAITALL, 'cause some architectures don't support it.
		while ( readSizeBytesLeft > 0 )
		{
#ifdef __DEBUGGING_NETWORK__
			printf("Waiting for %d bytes of packet size data..\n", readSizeBytesLeft);
#endif
			int bytesRead = recv( m_socket, incomingBuffer, readSizeBytesLeft, 0 );
#ifdef __DEBUGGING_NETWORK__
			printf("Received %d bytes.\n",bytesRead);
#endif
			if ( bytesRead == -1 )
			{
				perror("recv");
				cleanexit(1);
			}
			
			incomingBuffer += bytesRead;
			readSizeBytesLeft -= bytesRead;
		}
		incomingBytes = ntohs(incomingBytes);
		int  remainingBytes = incomingBytes;
		//printf("Receiving %d bytes...\n", incomingBytes);
		char buffer[incomingBytes];
		char *bufferPointer = buffer;
		
		//  Emulate functionality of MSG_WAITALL, 'cause some architectures don't support it.
		while ( remainingBytes > 0 )
		{
#ifdef __DEBUGGING_NETWORK__
			printf("Waiting for %d bytes of packet data..\n", remainingBytes );
#endif
			int bytesRead = recv( m_socket, bufferPointer, remainingBytes, 0 );
#ifdef __DEBUGGING_NETWORK__
			printf("Received %d bytes.\n", bytesRead);
#endif
			if ( bytesRead == -1 )
			{
				perror("recv");
				cleanexit(1);
			}
			bufferPointer += bytesRead;
			remainingBytes -= bytesRead;
		}
#ifdef __DISPLAY_PACKET_CONTENT__	
		printf("Packet of %d bytes:\n", incomingBytes );
		char *bufferstart = buffer;
		
		for ( int i = 0; i < incomingBytes; i++ )	
		{
			printf("Value: %d\n",*(bufferstart + i));
		}
		printf("\n");
		
#endif
		netMetaPacketInput *packet = new netMetaPacketInput(buffer, incomingBytes);
		
		while ( !packet->Done() )
		{
			sint8 type = packet->PeekSint8();
#define MAX_MESSAGE_BYTES	(256)
			char	messageBuffer[MAX_MESSAGE_BYTES];
			sint16 	messageBufferLength = MAX_MESSAGE_BYTES;
			
			switch ( type )
			{
				case SPT_ClientLocation:
					packet->ClientLocation(clientLoc);
					m_display->UpdateLocation( clientLoc.loc );
					break;
				case SPT_GroupData:
					packet->GroupData(groupData);
					m_display->UpdateGroupData(groupData.memberCount, groupData.memberTurns, groupData.haveTurnFromClient );
					break;
				case SPT_ClientStatistics:
				case SPT_ClientHitPoints:
				case SPT_ClientSpellPoints:
				case SPT_ClientExperience:
					m_display->GetStatus()->ReceiveChanges( packet );
					break;
				case SPT_MapTile:
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
				case SPT_ObjectStats:
					packet->ObjectStats(objectID);
					m_display->SetObjectStats(objectID);
					break;
				case SPT_ObjectName:
					packet->ObjectName(objectID, objectType, messageBuffer, messageBufferLength);
					m_display->SetObjectName(objectID, objectType, messageBuffer);
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
					packet->MapUpdateBBox(bbox);

					//  If we haven't prepared this map yet, ask for a full refresh of
					//  this map, so we can initialise our map structures.
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
							tile.objectCount = bbox.objectCount[i+(j*bbox.width)];
							tile.object = new objDescription[tile.objectCount];
							for ( int k = 0; k < tile.objectCount; k++ )
								tile.object[k] = bbox.object[i+(j*bbox.width)][k];
							m_display->UpdateMapTile( hnPoint(bbox.loc.x+i, bbox.loc.y+j, bbox.loc.z), tile);
							delete [] tile.object;
						}

					break;
				case SPT_Inventory:
					packet->Inventory(inven);
					objList = new objDescription[inven.GetObjectCount()];
					for ( int i = 0; i < inven.GetObjectCount(); i++ )
						objList[i] = inven.GetObject(i);
					m_display->UpdateInventory(inven.GetObjectCount(), objList);
					delete [] objList;
					break;
				case SPT_InventoryItem:
					packet->InventoryItem(item);
					m_display->UpdateInventoryItem(item.object, item.inventorySlot);
				case SPT_TakenItem:
					packet->TakenItem(item);
					m_display->TakenItem(item.object, item.inventorySlot);
					break;
				case SPT_DroppedItem:
					packet->DroppedItem(item);
					m_display->DroppedItem(item.object);
					break;
				case SPT_WieldedItem:
					packet->WieldedItem(item);
					m_display->WieldedItem(item.object, item.inventorySlot);
					break;
				case SPT_BadPacketNotice:
					packet->BadPacketNotice();
					delete m_display;
					printf("Server killed us for sending an unknown packet.  Version mismatch?\n");
					m_done = true;
					//exit(1);
					break;
				case SPT_QuitConfirm:
					packet->QuitConfirm();
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
	
	if ( send(m_socket, &metapacketdatalength, sizeof(sint16), 0 ) == -1 )
		perror("send");

	if ( send(m_socket, m_packet->GetBuffer(), m_packet->GetBufferLength(), 0 ) == -1 )
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
netClient::SendAttack(hnDirection dir)
{
	sint8 direction = dir;
	StartMetaPacket();
	m_packet->ClientAttack(direction);
	TransmitMetaPacket();
}

void
netClient::SendWait()
{
	StartMetaPacket();
	m_packet->ClientWait();
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
netClient::SendTake( objDescription *obj, uint8 stackID )
{
	netClientTake packet;

	packet.object = *obj;
	packet.stackID = stackID;

	StartMetaPacket();
	m_packet->ClientTake(packet);
	TransmitMetaPacket();
}

void
netClient::SendDrop( objDescription *object, uint8 inventoryID )
{
	netInventoryItem packet;

	packet.object = *object;
	packet.inventorySlot = inventoryID;

	StartMetaPacket();
	m_packet->ClientDrop(packet);
	TransmitMetaPacket();
}

void
netClient::SendWield( objDescription *object, uint8 inventoryID )
{
	netInventoryItem packet;

	packet.object = *object;
	packet.inventorySlot = inventoryID;

	StartMetaPacket();
	m_packet->ClientWield(packet);
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
