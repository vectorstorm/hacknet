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
//  base 'netMetaPacket' class.  Calls of Char(), Long(), String(), etc.
//  are pure virtual functions, implemented by the derived input and
//  output functions to read/write the appropriate types.  This means
//  that we don't have to worry about keeping our packet reading/writing
//  algorithms in synch with each other!  (Though we really should check
//  that our version numbers are identical!  This is a big TODO.)
//
//  Another point to note is that the functions
//  Char(), Short(), and Long() use NETWORK chars, shorts, and longs.
//  That is, 8 bit, 16 bit, and 32 bit, respectively -- not the native
//  machine's chars, shorts, and longs, which could vary in size across
//  implementations.  To enforce this, we use uint8/sint8 to denote an
//  eight bit unsigned/signed int, and similar types for 16 and 32 bit
//  numbers.  These are typedefed in HN_Types.h from the standard
//  network integer types u_int8_t (uint8_t, under *BSD).
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
	Char( type );
	Char( packet.loc.x );
	Char( packet.loc.y );
	Char( packet.loc.z );

	return (!m_error);
}

bool
netMetaPacket::ClientStatistics( netClientStatistics &packet )
{
	m_error = false;

	sint8 type = SPT_ClientStatistics;
	Char( type );
	Char( packet.strength );
	Char( packet.dexterity );
	Char( packet.constitution );
	Char( packet.intelligence );
	Char( packet.wisdom );
	Char( packet.charisma );

	return (!m_error);
}

bool
netMetaPacket::ClientHitPoints( netClientHitPoints &packet )
{
	m_error = false;

	sint8 type = SPT_ClientHitPoints;
	Char( type );
	Short( packet.maxHitPoints );
	Short( packet.hitPoints );

	return (!m_error);
}

bool
netMetaPacket::ClientSpellPoints( netClientSpellPoints &packet )
{
	m_error = false;

	sint8 type = SPT_ClientSpellPoints;
	Char( type );
	Short( packet.maxSpellPoints );
	Short( packet.spellPoints );

	return (!m_error);
}

bool
netMetaPacket::ClientExperience( netClientExperience &packet )
{
	m_error = false;

	sint8 type = SPT_ClientExperience;
	Char( type );
	Short( packet.experiencePoints );
	Char( packet.level );

	return (!m_error);
}


bool
netMetaPacket::TextMessage( char * buffer, sint16 & bufferlength )
{
	m_error = false;
	
	sint8 type = SPT_Message;
	Char(type);
	String( buffer, bufferlength );
	
	return (!m_error);
}

bool
netMetaPacket::MapTile( netMapTile &packet )
{
	m_error = false;
	
	sint8 type = SPT_MapTile;
	Char( type );
	Char( packet.loc.x );
	Char( packet.loc.y );
	Char( packet.loc.z );
	Short( packet.material );
	Short( packet.wall );

	return (!m_error);
}

bool
netMetaPacket::DungeonReset( sint8 & levelCount )
{
	m_error = false;

	sint8 type = SPT_DungeonReset;
	Char(type);
	Char( levelCount );

	return (!m_error);
}

bool
netMetaPacket::MapReset( netMapReset &packet )
{
	m_error = false;
	
	sint8 type = SPT_MapReset;
	Char( type );
	Char( packet.width );
	Char( packet.height );
	Char( packet.depth );
	
	return (!m_error);
}


//------------------------------------------------------
//  TODO:  This is an ugly hack.  This code (if we're
//    reading data out of the packet) news space to
//    store the data we're reading.  The calling code
//    needs to manually delete it.  This is BAD design,
//    and is just begging for trouble!  I really need
//    to find a clean way around this!  (Perhaps make
//    the netMapUpdateBBox a class, instead of a struct,
//    and have it delete the appropriate arrays when
//    it's deleted?)
//------------------------------------------------------
bool
netMetaPacket::MapUpdateBBox( netMapUpdateBBox &packet )
{
	m_error = false;

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

	return (!m_error);
}

bool
netMetaPacket::MapEntity( netMapEntity &packet )
{
	m_error = false;

	sint8 type = SPT_MapEntity;
	Char(type);
	Char(packet.loc.x);
	Char(packet.loc.y);
	Char(packet.loc.z);
	Char(packet.objectType);

	return (!m_error);
}

bool
netMetaPacket::MapObjectList( netMapObjectList &packet )
{
	m_error = false;
	
	sint8 type = SPT_MapObjectList;
	Char( type );
	
	return (!m_error);
}

bool
netMetaPacket::GroupData( netGroupData &packet )
{
	m_error = false;

	sint8 type = SPT_GroupData;
	Char( type );
	Char( packet.memberCount );
	Char( packet.memberTurns );
	Char( packet.haveTurnFromClient );

	return (!m_error);
}

bool
netMetaPacket::Refresh()
{
	m_error = false;
	
	sint8 type = SPT_Refresh;
	Char( type );
	
	return (!m_error);
}

bool
netMetaPacket::JoinOK()
{
	m_error = false;

	sint8 type = SPT_JoinOk;
	Char( type );

	return (!m_error);
}

bool
netMetaPacket::QuitConfirm()
{
	m_error = false;
	
	sint8 type = SPT_QuitConfirm;
	Char( type );
	
	return (!m_error);
}

bool
netMetaPacket::SaveConfirm()
{	
	m_error = false;
	
	sint8 type = SPT_SaveConfirm;
	Char( type );
	
	return (!m_error);
}

bool
netMetaPacket::BadPacketNotice()
{
	m_error = false;
	
	sint8 type = SPT_BadPacketNotice;
	Char( type );
	
	return (!m_error);
}


bool
netMetaPacket::ClientMove( sint8 & direction )
{
	m_error = false;
	
	sint8 type = CPT_Move;
	Char( type );
	Char( direction );
	
	return (!m_error);
}

bool
netMetaPacket::ClientAttack( sint8 & direction )
{
	m_error = false;
	
	sint8 type = CPT_Attack;
	Char( type );
	Char( direction );
	
	return (!m_error);
}

bool
netMetaPacket::ClientWait( )
{
	m_error = false;
	
	sint8 type = CPT_Wait;
	Char( type );
	
	return (!m_error);
}


bool
netMetaPacket::ClientName( char * namebuffer, sint16 & bufferlength )
{
	m_error = false;
	
	sint8 type = CPT_Name;
	Char(type);
	String( namebuffer, bufferlength );
	
	return (!m_error);
}

bool
netMetaPacket::ClientTalk( char * talkbuffer, sint16 & bufferlength )
{
	m_error = false;
	
	sint8 type = CPT_Talk;
	Char(type);
	String( talkbuffer, bufferlength );
	
	return (!m_error);
}

bool
netMetaPacket::ClientRequestRefresh( sint8 & level )
{
	m_error = false;

	sint8 type = CPT_RequestRefresh;
	Char(type);
	Char(level);

	return (!m_error);
}

bool
netMetaPacket::ClientQuit()
{
	m_error = false;
	
	sint8 type = CPT_Quit;
	Char( type );
	
	return (!m_error);
}

bool
netMetaPacket::ClientSave()
{
	m_error = false;
	
	sint8 type = CPT_Save;
	Char( type );
	
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
	
	if ( !success )
		m_error = true;
	
	return (!m_error);
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
	
	if ( !success )
		m_error = true;

	return (!m_error);
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
	Short(packetStringLength);
	length = packetStringLength;

	if ( length > stringLength-1 )
		length = stringLength-1;	// don't read more from the packet than we can actually read!
	
	
	for ( int i = 0; i < length; i++ )
	{
		sint8 thebyte;
		Char(thebyte);
		string[i] = thebyte;
	}
	string[length] = '\0';
	
	// now, in case there's more data in the packet than we have space for,
	// keep reading chars out of the packet until we reach the end of this
	// string (otherwise parsing any other packets in this metapacket would
	// fail!)
	sint8 temp;
	for ( int i = length; i < packetStringLength; i++ )
		Char(temp);
		
	// if we're reading a packet, be sure we stick a null on the end, for safety.
	string[stringLength-1] = '\0';

	if ( !success )
		m_error = true;

	return (!m_error);
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
netMetaPacketOutput::Char( sint8 & result )
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

	if ( !success )
		m_error = true;

	return (!m_error);
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
	
	Short(length);
	
	for ( int i = 0; i < length; i++ )
	{
		sint8 thebyte = string[i];
		Char(thebyte);
	}
	
	if ( !success )
		m_error = true;
	
	return (!m_error);
}
