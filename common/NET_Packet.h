#ifndef __NET_PACKET_H__
#define __NET_PACKET_H__

#include "HN_Types.h"
#include "HN_Point.h"
#include "HN_Enum.h"


enum{
	SPT_ClientLocation,
	SPT_ClientStatus,	// send client details about himself.  (Stats, etc)
	SPT_MapTile,
	SPT_DungeonReset,
	SPT_MapReset,
	SPT_MapUpdateBBox,
	SPT_MapObjectList,
	SPT_MapEntity,
	SPT_GroupData,		// sending information on the player's group.
	SPT_Message,		// message from the server
	SPT_QuitConfirm,	// yes, you're out of the game
	SPT_SaveConfirm,	// yes, you've been saved and are out of the game
	SPT_BadPacketNotice,	// you just sent me a bad packet, and are being disconnected.  Version mismatch?
	SPT_Refresh,		// we just finished sending a set of events.  Go ahead and refresh the screen now.
	SPT_MAX
};

struct netClientLocation
{
	hnPoint loc;
};

// ---------------------------------------------------
//   Gads.  Look at this struct.  We're never going to
//   actually change this much stuff all at once.
//   I should split this up into bunches of separate
//   packets....
// ---------------------------------------------------
struct netClientStatus
{
	uint8 strength;
	uint8 dexterity;
	uint8 constitution;
	uint8 intelligence;
	uint8 wisdom;
	uint8 charisma;
	
	uint16 maxHitPoints;
	uint16 hitPoints;

	uint16 maxSpellPoints;
	uint16 spellPoints;

	uint8 level;

	uint16 experiencePoints;
};

struct netMapTile
{
	hnPoint loc;
	sint16 material;
	sint16 wall;
};

struct netGroupData
{
	sint8 memberCount;
	sint8 memberTurns;
	sint8 haveTurnFromClient;
};

struct netMapReset		// we've moved to a new map.. init a map with this width/height.
{
	sint8 width;
	sint8 height;
	sint8 depth;
};

struct netMapUpdateBBox
{
	hnPoint loc;
	sint8 width;
	sint8 height;

	sint16 *material;  // width-first material array (top row, then next row, etc)
	sint16 *wall;
	sint8 *entityType;
};

struct netMapObjectList
{
	hnPoint loc;
	sint8 numObjects;
	sint8 *objectType;
};

struct netMapEntity
{
	hnPoint loc;
	sint8 objectType;
};


enum 
{
	CPT_Move,
	CPT_Wait,
	CPT_Attack,	// an attack is actually different from a move, so we don't attack people by accident.
	CPT_Name,
	CPT_Talk,
	CPT_RequestRefresh,	// request refreshed information on this level
	CPT_Save,	// Save and quit
	CPT_Quit,	// Quit without saving.
	CPT_MAX
};

class netMetaPacket
{
protected:
	char			*m_buffer;
	char			*m_bufferPoint;			// we're we're currently looking in the buffer

	unsigned long		m_bufferLength;			// how long the buffer actually is.
	unsigned long   	m_bufferDistance;		// how much we've read/written to the buffer already

public:
				netMetaPacket(char *buffer, uint32 bufferLength);
	virtual			~netMetaPacket();

	char *			GetBuffer() { return m_buffer; }
	virtual long		GetBufferLength() { return m_bufferLength; }

	bool			Done() { return m_bufferDistance >= m_bufferLength; }
	
	/*****************   Server Packets  ******************/
	bool			ClientLocation( netClientLocation &packet );
	bool			TextMessage( char * messagebuffer, sint16 & bufferlength );
	bool			MapTile( netMapTile &packet );
	bool			GroupData( netGroupData &packet );
	bool			DungeonReset( sint8 & levelCount );
	bool			MapReset( netMapReset &packet );
	bool			MapUpdateBBox( netMapUpdateBBox &packet );
	bool			MapEntity( netMapEntity &packet );
	bool			MapObjectList( netMapObjectList &packet );
	bool			QuitConfirm();
	bool			SaveConfirm();
	bool			Refresh();
	bool			BadPacketNotice();
	
	/*****************   Client Packets  ******************/
	bool			ClientMove( sint8 & direction );
	bool			ClientAttack( sint8 & direction );
	bool			ClientWait();
	bool			ClientName( char * namebuffer, sint16 & bufferLength );
	bool			ClientTalk( char * talkbuffer, sint16 & bufferLength );
	bool			ClientRequestRefresh( sint8 & level );
	bool			ClientSave();
	bool			ClientQuit();

	
	virtual bool 		Char( sint8 & ) = 0;
	virtual bool 		Short( sint16 & ) = 0;
	virtual bool 		Long( sint32 & ) = 0;
	virtual bool		String( char *, sint16 & ) = 0;

	virtual	bool		Input() { return false; }
	virtual bool		Output() { return false; }
};

class netMetaPacketInput: public netMetaPacket		// we've received this and are parsing out of it
{
public:
				netMetaPacketInput(char *packet, uint32 bufferLength);
	virtual			~netMetaPacketInput();

	virtual bool 		Char( sint8 & );
	virtual bool 		Short( sint16 & );
	virtual bool 		Long( sint32 & );
	virtual bool		String( char *, sint16 & );
	
	virtual sint8 		PeekChar();
	virtual sint16 		PeekShort();
	virtual sint32		PeekLong();
	
	virtual	bool		Input() { return true; }
};

class netMetaPacketOutput: public netMetaPacket		// we're writing into this
{
public:
				netMetaPacketOutput(char *packet, uint32 bufferLength);
	virtual			~netMetaPacketOutput();
	
	virtual long		GetBufferLength() { return m_bufferDistance; }	// tell how many bytes we actually wrote, not how big our buffer was

	virtual bool 		Char( sint8 & );
	virtual bool 		Short( sint16 & );
	virtual bool 		Long( sint32 & );
	virtual bool		String( char *, sint16 & );
	
	virtual	bool		Output() { return true; }
};





#endif //__NET_PACKET_H__
