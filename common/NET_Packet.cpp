#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "NET_Packet.h"

//--------------------------------------------------------------------------
//
//  METAPACKET
//
//  Metapackets are a method of stuffing more than one 'packet' of data 
//  into a buffer which will eventually be transmitted over a network.
//
//  Points of note:  The packet structure is defined by calls on the
//  base 'netMetaPacket' class.  Calls of Sint8(), Sint32(), String(), etc.
//  are pure virtual functions, implemented by the derived input and
//  output functions to read/write the appropriate types.  This means
//  that we don't have to worry about keeping our packet reading/writing
//  algorithms in synch with each other!  (Though we really should check
//  that our version numbers are identical!  This is a big TODO.)
//
//  We use uint8/sint8 to denote an eight bit unsigned/signed int, and 
//  similar types for 16 and 32 bit numbers.  These are typedefed in 
//  HN_Types.h from the standard network integer types u_int*_t 
//  (uint*_t, under *BSD).
//
//--------------------------------------------------------------------------


netMetaPacket::netMetaPacket( char *packet, uint32 packetSize ):
	m_bufferLength(packetSize),
	m_bufferDistance(0)
{
	m_buffer = new char[packetSize];
	m_bufferPoint = m_buffer;

	for ( unsigned int i = 0; i < packetSize; i++ )
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
	m_error = false;
	
	sint8 type = SPT_ClientLocation;
	Sint8( type );
	Sint8( packet.loc.x );
	Sint8( packet.loc.y );
	Sint8( packet.loc.z );

	return (!m_error);
}

bool
netMetaPacket::ClientStatistics( netClientStatistics &packet )
{
	m_error = false;

	sint8 type = SPT_ClientStatistics;
	Sint8( type );
	Uint8( packet.strength );
	Uint8( packet.dexterity );
	Uint8( packet.constitution );
	Uint8( packet.intelligence );
	Uint8( packet.wisdom );
	Uint8( packet.charisma );

	return (!m_error);
}

bool
netMetaPacket::ClientHitPoints( netClientHitPoints &packet )
{
	m_error = false;

	sint8 type = SPT_ClientHitPoints;
	Sint8( type );
	Sint16( packet.maxHitPoints );
	Sint16( packet.hitPoints );

	return (!m_error);
}

bool
netMetaPacket::ClientSpellPoints( netClientSpellPoints &packet )
{
	m_error = false;

	sint8 type = SPT_ClientSpellPoints;
	Sint8( type );
	Sint16( packet.maxSpellPoints );
	Sint16( packet.spellPoints );

	return (!m_error);
}

bool
netMetaPacket::ClientExperience( netClientExperience &packet )
{
	m_error = false;

	sint8 type = SPT_ClientExperience;
	Sint8( type );
	Uint16( packet.experiencePoints );
	Uint8( packet.level );

	return (!m_error);
}


bool
netMetaPacket::TextMessage( char * buffer, sint16 & bufferlength )
{
	m_error = false;
	
	sint8 type = SPT_Message;
	Sint8(type);
	String( buffer, bufferlength );
	
	return (!m_error);
}

bool
netMetaPacket::MapTile( netMapTile &packet )
{
	m_error = false;
	
	sint8 type = SPT_MapTile;
	Sint8( type );
	Sint8( packet.loc.x );
	Sint8( packet.loc.y );
	Sint8( packet.loc.z );
	Sint16( packet.material );
	Sint16( packet.wall );

	return (!m_error);
}

bool
netMetaPacket::DungeonReset( sint8 & levelCount )
{
	m_error = false;

	sint8 type = SPT_DungeonReset;
	Sint8(type);
	Sint8( levelCount );

	return (!m_error);
}

bool
netMetaPacket::MapReset( netMapReset &packet )
{
	m_error = false;
	
	sint8 type = SPT_MapReset;
	Sint8( type );
	Sint8( packet.width );
	Sint8( packet.height );
	Sint8( packet.depth );
	
	return (!m_error);
}


//------------------------------------------------------
//  TODO:  This is an ugly hack.  This code (if we're
//    reading data out of the packet) news space to
//    store the data we're reading.  It gets deleted
//    automatically when the netMapUpdateBBox gets
//    deleted, but this isn't particularly intuitive.
//    Need to revamp this system, so we don't new data
//    inside the netMapUpdateBBox anywhere outside of
//    that class, so it can be fully responsible for
//    its own memory usage.
//------------------------------------------------------
bool
netMetaPacket::MapUpdateBBox( netMapUpdateBBox &packet )
{
	m_error = false;

	sint8 type = SPT_MapUpdateBBox;
	Sint8( type );
	Sint8( packet.loc.x );
	Sint8( packet.loc.y );
	Sint8( packet.loc.z );
	Sint8( packet.width );
	Sint8( packet.height );
	
	sint32 nTiles = (sint32)packet.width * (sint32)packet.height;
	
	if ( Input() )
	{
		packet.material = new sint16[nTiles];
		packet.wall = new sint16[nTiles];
		packet.entityType = new sint8[nTiles];		
		packet.objectCount = new uint16[nTiles];
		packet.object = new (objDescription *)[nTiles];
	}
	
	for ( int i = 0; i < nTiles; i++ )
		Sint16( packet.material[i] );
	for ( int i = 0; i < nTiles; i++ )
		Sint16( packet.wall[i] );
	for ( int i = 0; i < nTiles; i++ )
		Sint8( packet.entityType[i] );
	
	for ( int i = 0; i < nTiles; i++ )
	{
		Uint16( packet.objectCount[i] );
		
		if ( Input() )
			packet.object[i] = new objDescription[packet.objectCount[i]];
		
		for ( int j = 0; j < packet.objectCount[i]; j++ )
		{
			// grab this object description
			Uint16( packet.object[i][j].type );
			Uint8( packet.object[i][j].blesscurse );
			Uint8( packet.object[i][j].count );
		}
	}

	return (!m_error);
}

bool
netMetaPacket::MapEntity( netMapEntity &packet )
{
	m_error = false;

	sint8 type = SPT_MapEntity;
	Sint8(type);
	Sint8(packet.loc.x);
	Sint8(packet.loc.y);
	Sint8(packet.loc.z);
	Sint8(packet.objectType);

	return (!m_error);
}

bool
netMetaPacket::MapObjectList( netMapObjectList &packet )
{
	m_error = false;
	
	sint8 type = SPT_MapObjectList;
	Sint8( type );
	
	return (!m_error);
}

bool
netMetaPacket::GroupData( netGroupData &packet )
{
	m_error = false;

	sint8 type = SPT_GroupData;
	Sint8( type );
	Sint8( packet.memberCount );
	Sint8( packet.memberTurns );
	Sint8( packet.haveTurnFromClient );

	return (!m_error);
}

bool
netMetaPacket::Refresh()
{
	m_error = false;
	
	sint8 type = SPT_Refresh;
	Sint8( type );
	
	return (!m_error);
}

bool
netMetaPacket::JoinOK()
{
	m_error = false;

	sint8 type = SPT_JoinOk;
	Sint8( type );

	return (!m_error);
}

bool
netMetaPacket::QuitConfirm()
{
	m_error = false;
	
	sint8 type = SPT_QuitConfirm;
	Sint8( type );
	
	return (!m_error);
}

bool
netMetaPacket::SaveConfirm()
{	
	m_error = false;
	
	sint8 type = SPT_SaveConfirm;
	Sint8( type );
	
	return (!m_error);
}

bool
netMetaPacket::BadPacketNotice()
{
	m_error = false;
	
	sint8 type = SPT_BadPacketNotice;
	Sint8( type );
	
	return (!m_error);
}


bool
netMetaPacket::ClientMove( sint8 & direction )
{
	m_error = false;
	
	sint8 type = CPT_Move;
	Sint8( type );
	Sint8( direction );
	
	return (!m_error);
}

bool
netMetaPacket::ClientAttack( sint8 & direction )
{
	m_error = false;
	
	sint8 type = CPT_Attack;
	Sint8( type );
	Sint8( direction );
	
	return (!m_error);
}

bool
netMetaPacket::ClientWait( )
{
	m_error = false;
	
	sint8 type = CPT_Wait;
	Sint8( type );
	
	return (!m_error);
}


bool
netMetaPacket::ClientName( char * namebuffer, sint16 & bufferlength )
{
	m_error = false;
	
	sint8 type = CPT_Name;
	Sint8(type);
	String( namebuffer, bufferlength );
	
	return (!m_error);
}

bool
netMetaPacket::ClientTalk( char * talkbuffer, sint16 & bufferlength )
{
	m_error = false;
	
	sint8 type = CPT_Talk;
	Sint8(type);
	String( talkbuffer, bufferlength );
	
	return (!m_error);
}

bool
netMetaPacket::ClientRequestRefresh( sint8 & level )
{
	m_error = false;

	sint8 type = CPT_RequestRefresh;
	Sint8(type);
	Sint8(level);

	return (!m_error);
}

bool
netMetaPacket::ClientQuit()
{
	m_error = false;
	
	sint8 type = CPT_Quit;
	Sint8( type );
	
	return (!m_error);
}

bool
netMetaPacket::ClientSave()
{
	m_error = false;
	
	sint8 type = CPT_Save;
	Sint8( type );
	
	return (!m_error);
}

//------------------------------------------------------------------------------------
//  Input packets are packets which we have just received, so we copy data
//  FROM the buffer TO the types passed in.
//------------------------------------------------------------------------------------

netMetaPacketInput::netMetaPacketInput( char *packet, uint32 packetSize ):
	netMetaPacket( packet, packetSize )
{
}

netMetaPacketInput::~netMetaPacketInput()
{
}

bool
netMetaPacketInput::Sint8( sint8 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = *m_bufferPoint;
		m_bufferPoint += 1;				// we consider a char to be 1 byte.
		m_bufferDistance += 1;
		success = true;
	}
	
	if ( !success )
		m_error = true;
	
	return (!m_error);
}

bool
netMetaPacketInput::Uint8( uint8 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = *((uint8 *)m_bufferPoint);
		m_bufferPoint += 1;				// we consider a char to be 1 byte.
		m_bufferDistance += 1;
		success = true;
	}
	
	if ( !success )
		m_error = true;
	
	return (!m_error);
}

bool
netMetaPacketInput::Sint16( sint16 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohs(*((sint16 *)m_bufferPoint));
		m_bufferPoint += sizeof(sint16);
		m_bufferDistance += sizeof(sint16);
		success = true;
	}
	
	if ( !success )
		m_error = true;

	return (!m_error);
}

bool
netMetaPacketInput::Uint16( uint16 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohs(*((uint16 *)m_bufferPoint));
		m_bufferPoint += sizeof(uint16);
		m_bufferDistance += sizeof(uint16);
		success = true;
	}
	
	if ( !success )
		m_error = true;

	return (!m_error);
}

bool
netMetaPacketInput::Sint32( sint32 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohl(*((sint32 *)m_bufferPoint));
		m_bufferPoint += sizeof(sint32);
		m_bufferDistance += sizeof(sint32);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}

bool
netMetaPacketInput::Uint32( uint32 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohl(*((uint32 *)m_bufferPoint));
		m_bufferPoint += sizeof(uint32);
		m_bufferDistance += sizeof(uint32);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}


bool
netMetaPacketInput::String( char * string, sint16 & stringLength )
{	
	bool success = true;		// we always succeed, because this
					// code was written to be fault-tolerant.
					// if there's an error, one of the
					// functions we call will flag it (but
					// this should never happen, because
					// we're supposed to be fault-tolerant!)
	
	sint16 length;			// how much we're going to read
	sint16 packetStringLength;	// how much is actually stored in the packet
	
	// reading packet..
	Sint16(packetStringLength);
	length = packetStringLength;

	if ( length > stringLength-1 )
		length = stringLength-1;	// don't read more from the packet than we can actually read!
	
	
	for ( int i = 0; i < length; i++ )
	{
		sint8 thebyte;
		Sint8(thebyte);
		string[i] = thebyte;
	}
	string[length] = '\0';
	
	// now, in case there's more data in the packet than we have space for,
	// keep reading chars out of the packet until we reach the end of this
	// string (otherwise parsing any other packets in this metapacket would
	// fail!)
	sint8 temp;
	for ( int i = length; i < packetStringLength; i++ )
		Sint8(temp);
		
	// if we're reading a packet, be sure we stick a null on the end, for safety.
	string[stringLength-1] = '\0';

	if ( !success )
		m_error = true;

	return (!m_error);
}

sint8
netMetaPacketInput::PeekSint8()
{
	sint8 result = -1;	
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = *(m_bufferPoint);
	}

	return result;
}
	
sint16
netMetaPacketInput::PeekSint16()
{
	sint16 result = -1;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohs(*((sint16 *)m_bufferPoint));
	}

	return result;
}

sint32
netMetaPacketInput::PeekSint32()
{
	sint32 result = -1;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		result = ntohl(*((sint32 *)m_bufferPoint));
	}

	return result;
}


//------------------------------------------------------------------------------------
//  Output packets are packets which we have just received, so we copy data
//  FROM the types passed in TO the buffer.
//------------------------------------------------------------------------------------

	
netMetaPacketOutput::netMetaPacketOutput( char *packet, uint32 packetSize ):
	netMetaPacket( packet, packetSize )
{
}

netMetaPacketOutput::~netMetaPacketOutput()
{
}

bool
netMetaPacketOutput::Sint8( sint8 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*(m_bufferPoint++) = result;
		m_bufferDistance += sizeof(sint8);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}
	
bool
netMetaPacketOutput::Uint8( uint8 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*((uint8 *)m_bufferPoint++) = result;
		m_bufferDistance += sizeof(uint8);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}
	

bool
netMetaPacketOutput::Sint16( sint16 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*((sint16 *)m_bufferPoint) = htons(result);
		m_bufferPoint += sizeof(sint16);
		m_bufferDistance += sizeof(sint16);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}

bool
netMetaPacketOutput::Uint16( uint16 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*((uint16 *)m_bufferPoint) = htons(result);
		m_bufferPoint += sizeof(uint16);
		m_bufferDistance += sizeof(uint16);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}


bool
netMetaPacketOutput::Sint32( sint32 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*((sint32 *)m_bufferPoint) = htonl(result);
		m_bufferPoint += sizeof(sint32);
		m_bufferDistance += sizeof(sint32);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}

bool
netMetaPacketOutput::Uint32( uint32 & result )
{
	bool success = false;
	
	if ( m_bufferDistance < m_bufferLength )
	{
		*((uint32 *)m_bufferPoint) = htonl(result);
		m_bufferPoint += sizeof(uint32);
		m_bufferDistance += sizeof(uint32);
		success = true;
	}

	if ( !success )
		m_error = true;

	return (!m_error);
}


bool
netMetaPacketOutput::String( char * string, sint16 & stringLength )
{
	bool success = true;		// we always succeed!
	sint16 length = strlen(string);
	
	if ( length > stringLength )	// this can never happen, right?  Unless the string isn't null terminated.
	{
		assert(0);
		length = stringLength;
	}
	
	Sint16(length);
	
	for ( int i = 0; i < length; i++ )
	{
		sint8 thebyte = string[i];
		Sint8(thebyte);
	}
	
	if ( !success )
		m_error = true;
	
	return (!m_error);
}


//---------------------------------------------------------------------
//  Individual packet classes, where required.
//---------------------------------------------------------------------

netMapUpdateBBox::netMapUpdateBBox():
	material(NULL), 
	wall(NULL), 
	entityType(NULL),
	object(NULL),
	objectCount(NULL)
{
}

netMapUpdateBBox::~netMapUpdateBBox()
{
	delete [] material;
	delete [] wall;
	delete [] entityType;

	if ( object )
	{
		for ( int i = 0; i < width * height; i++ )
			delete [] object[i];
	}
	delete [] object;
	delete [] objectCount;
}
	
