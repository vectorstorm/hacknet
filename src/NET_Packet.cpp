#include <netinet/in.h>
#include "NET_Packet.h"
#include <stdio.h>

netMetaPacket::netMetaPacket( char *packet, uint32 packetSize ):
	m_bufferLength(packetSize),
	m_bufferDistance(0)
{
	m_buffer = new char[packetSize];
	m_bufferPoint = m_buffer;

	for ( int i = 0; i < packetSize; i++ )
	{
		m_buffer[i] = packet[i];
	}
}

netMetaPacket::~netMetaPacket()
{
	delete [] m_buffer;
}


bool
netMetaPacket::ClientLocation( netClientLocation &packet )
{
	bool success = true;
	
	sint8 type = SPT_ClientLocation;
	Char( type );
	Char( packet.loc.x );
	Char( packet.loc.y );
	Char( packet.loc.z );

	return success;
}

bool
netMetaPacket::MapTile( netMapTile &packet )
{
	bool success = true;
	
	sint8 type = SPT_MapTile;
	Char( type );
	Char( packet.loc.x );
	Char( packet.loc.y );
	Char( packet.loc.z );
	Short( packet.material );
	Short( packet.wall );

	return success;
}

bool
netMetaPacket::MapReset( netMapReset &packet )
{
	bool success = true;
	
	sint8 type = SPT_MapReset;
	Char( type );
	Char( packet.width );
	Char( packet.height );
	
	return success;
}

bool
netMetaPacket::MapUpdateBBox( netMapUpdateBBox &packet )
{
	bool success = true;

	sint8 type = SPT_MapUpdateBBox;
	Char( type );
	Char( packet.loc.x );
	Char( packet.loc.y );
	Char( packet.loc.z );
	Char( packet.width );
	Char( packet.height );
	
	sint32 nTiles = (sint32)packet.width * (sint32)packet.height;
	
	if ( Input() )
	{
		packet.material = new sint16[nTiles];
		packet.wall = new sint16[nTiles];
		packet.entityType = new sint8[nTiles];		
	}
	
	for ( int i = 0; i < nTiles; i++ )
		Short( packet.material[i] );
	for ( int i = 0; i < nTiles; i++ )
		Short( packet.wall[i] );
	for ( int i = 0; i < nTiles; i++ )
		Char( packet.entityType[i] );

	return success;
}

bool
netMetaPacket::MapEntity( netMapEntity &packet )
{
	bool success = true;

	sint8 type = SPT_MapEntity;
	Char(type);
	Char(packet.loc.x);
	Char(packet.loc.y);
	Char(packet.loc.z);
	Char(packet.objectType);

	return success;
}

bool
netMetaPacket::MapObjectList( netMapObjectList &packet )
{
	bool success = true;
	
	sint8 type = SPT_MapObjectList;
	Char( type );
	
	return success;
}

bool
netMetaPacket::Refresh()
{
	bool success = true;
	
	sint8 type = SPT_Refresh;
	Char( type );
	
	return success;
}

bool
netMetaPacket::QuitConfirm()
{
	bool success = true;
	
	sint8 type = SPT_QuitConfirm;
	Char( type );
	
	return success;
}

bool
netMetaPacket::SaveConfirm()
{	
	bool success = true;
	
	sint8 type = SPT_SaveConfirm;
	Char( type );
	
	return success;
}

bool
netMetaPacket::BadPacketNotice()
{
	bool success = true;
	
	sint8 type = SPT_BadPacketNotice;
	Char( type );
	
	return success;
}

//------------------------------------------------------------------------------------

netMetaPacketInput::netMetaPacketInput( char *packet, uint32 packetSize ):
	netMetaPacket( packet, packetSize )
{
}

netMetaPacketInput::~netMetaPacketInput()
{
}

bool
netMetaPacketInput::Char( sint8 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = *m_bufferPoint;
		m_bufferPoint += 1;				// we consider a char to be 1 byte.
		m_bufferDistance += 1;
		success = true;
	}

	return success;
}
	
bool
netMetaPacketInput::Short( sint16 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohs(*((sint16 *)m_bufferPoint));
		m_bufferPoint += sizeof(sint16);
		m_bufferDistance += sizeof(sint16);
		success = true;
	}

	return success;
}

bool
netMetaPacketInput::Long( sint32 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohl(*((sint32 *)m_bufferPoint));
		m_bufferPoint += sizeof(sint32);
		m_bufferDistance += sizeof(sint32);
		success = true;
	}
}

sint8
netMetaPacketInput::PeekChar()
{
	sint8 result = -1;	
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = *(m_bufferPoint);
	}

	return result;
}
	
sint16
netMetaPacketInput::PeekShort()
{
	sint16 result = -1;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohs(*((sint16 *)m_bufferPoint));
	}

	return result;
}

sint32
netMetaPacketInput::PeekLong()
{
	sint32 result = -1;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohl(*((sint32 *)m_bufferPoint));
	}

	return result;
}


//------------------------------------------------------------------------------------

	
netMetaPacketOutput::netMetaPacketOutput( char *packet, uint32 packetSize ):
	netMetaPacket( packet, packetSize )
{
}

netMetaPacketOutput::~netMetaPacketOutput()
{
}

bool
netMetaPacketOutput::Char( sint8 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*(m_bufferPoint++) = result;
		m_bufferDistance += sizeof(sint8);
		success = true;
	}

	return success;
}
	
bool
netMetaPacketOutput::Short( sint16 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*((sint16 *)m_bufferPoint) = htons(result);
		m_bufferPoint += sizeof(sint16);
		m_bufferDistance += sizeof(sint16);
		success = true;
	}

	return success;
}

bool
netMetaPacketOutput::Long( sint32 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*((sint32 *)m_bufferPoint) = htonl(result);
		m_bufferPoint += sizeof(sint32);
		m_bufferDistance += sizeof(sint32);
		success = true;
	}

	return success;
}
