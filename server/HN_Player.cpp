#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "HN_Player.h"
#include "NET_Server.h"
#include "HN_Dungeon.h"
#include "HN_Group.h"
#include "MAP_Base.h"
#include "MAP_Client.h"
#include "HN_Game.h"

#include "ENT_Human.h"
#include "ENT_GridBug.h"

#include "assert.h"

hnPlayer::hnPlayer( int playerID, const hnPoint &where ):
	m_playerID(playerID),
	m_lastSentGroupPlayerCount(0),
	m_lastSentGroupPlayerQueuedTurns(0),
	m_movePending(false),
	m_statsChanged(true),
	m_hitPointsChanged(true),
	m_spellPointsChanged(true)
{
	m_mapCount = hnDungeon::GetInstance()->GetLevelCount();
	m_map = new (mapClient *)[m_mapCount];

	m_entity = new entHuman( where, this );
//	Ever wanted to play as a Grid Bug?  Comment out the line above and 
//	uncomment the next line, and you will!  Yes, we already
//	support non-human players!  Hooray!
//	------------------------------------------------------------
//	m_entity = new entGridBug( where, this );	
	
	hnDungeon::GetInstance()->GetLevel(where.z)->PutEntityAt( m_entity, where.x, where.y );
	
	for ( int i = 0; i < m_mapCount; i++ )
		m_map[i] = NULL;
	
	m_queuedTurn.type = queuedTurn::None;

	if ( m_entity == NULL )
		printf("Unable to allocate entity!\n");
}

hnPlayer::~hnPlayer()
{
	//  Since we're being deleted, remove us from the level.
	hnDungeon::GetInstance()->GetLevel(GetPosition().z)->RemoveEntity( m_entity );

	delete m_entity;

	for ( int i = 0; i < m_mapCount; i++ )
		delete m_map[i];
	
	delete [] m_map;
}

void
hnPlayer::SetEntity( entBase * entity )
{
	printf("Setting entity...\n");
	if ( m_entity )
		delete m_entity;
	
	m_entity = entity;
}

bool
hnPlayer::IsAlive()
{
	if ( m_entity )
	{
		return m_entity->IsAlive();
	}

	return false;
}

bool
hnPlayer::IsValidMove( hnDirection dir )
{
	if ( !m_entity )
		printf("ERROR!  No entity!\n");
	return m_entity->IsValidMove( dir );
}

bool
hnPlayer::IsValidAttack( hnDirection dir )
{
	return m_entity->IsValidAttack( dir );
}

void
hnPlayer::Move( hnDirection dir )
{
	if ( IsValidMove( dir ) )
	{
		m_queuedTurn.type = queuedTurn::Move;
		m_queuedTurn.move.direction = dir;
	}
}

void
hnPlayer::Attack( hnDirection dir )
{
	if ( IsValidAttack( dir ) )
	{
		m_queuedTurn.type = queuedTurn::Attack;
		m_queuedTurn.attack.direction = dir;
	}
}

void
hnPlayer::Wait()
{
	m_queuedTurn.type = queuedTurn::Wait;
}

const hnPoint &
hnPlayer::GetPosition()
{
	return m_entity->GetPosition();
}

#define MAX_TALK_DISTANCE	(12)
#define MAX_TALK_DISTANCE_SQ	(MAX_TALK_DISTANCE * MAX_TALK_DISTANCE)

void
hnPlayer::Listen( const hnPoint & position, char * message )
{
	//---------------------------------------------------------
	//  We hear the sound if we're within MAX_TALK_DISTANCE of
	//  it.
	//
	//  TODO:  We need a measure of 'loudness' which defines
	//  how far away we can hear a sound from.  Get rid of
	//  MAX_TALK_DISTANCE, sometime in the near future.
	//---------------------------------------------------------
	hnPoint offset = position - GetPosition();

	if ( offset.z == 0 )
	{
		if ( (offset.x * offset.x) + (offset.y * offset.y) < MAX_TALK_DISTANCE_SQ )
		{
			Listen(message);
		}
	}
}

void
hnPlayer::Listen( char * message )
{
	//---------------------------------------------------------
	//  If there's no position for the sound, then we hear it
	//---------------------------------------------------------
	netServer::GetInstance()->StartMetaPacket(m_playerID);
	netServer::GetInstance()->SendMessage( message );
	netServer::GetInstance()->TransmitMetaPacket();	// all done!
}

void
hnPlayer::See( const hnPoint & where, hnPlayer * who, char * message )
{
	if ( CanSee( where ) )
	{
		// make ourselves a proper message by taking the
		// name of 'who', and concatonating the message
		// onto the end.
		
		char buffer[256];
		char nameBuffer[128];
		
		if ( who == this )
			sprintf(nameBuffer,"You");
		else
			who->GetFullName(nameBuffer,128);
		
		snprintf(buffer,256,"%s %s", nameBuffer, message);
		buffer[255] = '\0';
		
		netServer::GetInstance()->StartMetaPacket(m_playerID);
		netServer::GetInstance()->SendMessage(buffer);
		netServer::GetInstance()->TransmitMetaPacket();
	}
}

void
hnPlayer::See( const hnPoint & where, char * message )
{
	if ( CanSee( where ) )
	{
		// if we can see the point, send the text message
		//  "'who' 'message'" to our client.
		netServer::GetInstance()->StartMetaPacket(m_playerID);
		netServer::GetInstance()->SendMessage(message);
		netServer::GetInstance()->TransmitMetaPacket();
	}
}

bool
hnPlayer::CanSee( const hnPoint & where )
{
	mapClient *map = m_map[ GetPosition().z ];

	return map->MapTile(where.x, where.y).visible;
}

bool
hnPlayer::HasQueuedTurn()
{
	return ( m_queuedTurn.type != queuedTurn::None );
}

void
hnPlayer::SetName( char * name )
{
	snprintf( m_name, MAX_PLAYER_NAME_BYTES, "%s", name );
}

char *
hnPlayer::GetName()
{
	return m_name;
}

void
hnPlayer::GetFullName( char * buffer, int bufferSize )
{
	if ( m_entity )
		m_entity->GetFullName(buffer, bufferSize);
	else
		strncpy(buffer,m_name,bufferSize);
}


void
hnPlayer::DoAction()
{
	entBase *entity;
	int result;
	char name[128];
	char buffer[128];
	
	switch ( m_queuedTurn.type )
	{
		case queuedTurn::Move:
			m_movePending = m_entity->FindMoveDestination( m_moveDestination, m_queuedTurn.move.direction );
			break;
		case queuedTurn::Attack:
			
			entity = m_entity->GetAttackTarget( m_queuedTurn.attack.direction );
			
			if (entity)
			{
				entity->GetFullName(name,128);
			}
			result = m_entity->Attack( m_queuedTurn.attack.direction );
			
			if ( result == entBase::AT_Hit )
				sprintf(buffer,"hit %s!", name);
			else if ( result == entBase::AT_Kill )
				sprintf(buffer,"destroyed %s!", name);
			else
				sprintf(buffer,"missed %s!", name);
			
			hnGame::GetInstance()->SeenEvent(this,buffer);
			
			//netServer::GetInstance()->StartMetaPacket(m_playerID);
			//netServer::GetInstance()->SendMessage(buffer);
			//netServer::GetInstance()->TransmitMetaPacket();
			break;
		case queuedTurn::Wait:
			// do nothing.
			break;
	}

	m_queuedTurn.type = queuedTurn::None;
}

void
hnPlayer::DoMove()
{
	// if we're moving in response to the turn we just took, then
	// move us!

	if ( m_movePending )
	{
		if ( m_moveDestination.z < 0 )
		{
			printf("%s left the dungeon!\n", GetName() );
			netServer::GetInstance()->SendQuitConfirm(m_playerID);
		}
		else
		{
			m_entity->MoveTo( m_moveDestination );
			RecalculateVision();
			m_movePending = false;
		}
	}

}

void
hnPlayer::RecalculateVision()
{
	//---------------------------------------------------------------
	//  Sanity checks, and be sure that our vision map has been
	//  allocated.
	//---------------------------------------------------------------
	mapBase *realMap = hnDungeon::GetLevel( GetPosition().z );
	assert(realMap);

	if ( m_map[ m_entity->GetPosition().z ] == NULL )
		m_map[ m_entity->GetPosition().z ] = new mapClient( realMap->GetWidth(), realMap->GetHeight() );

	mapClient *map = m_map[ m_entity->GetPosition().z ];
	assert( map );

	//---------------------------------------------------------------
	//  Calculate visibility
	//---------------------------------------------------------------
	
	//map->UpdateVisibility( m_entity->GetPosition(), realMap );
	realMap->CalculateVisibility( m_entity->GetPosition(), map );
}

void
hnPlayer::UpdateVision()
{
	//---------------------------------------------------------------
	//  Sanity checks, and be sure that our vision map has been
	//  allocated.
	//---------------------------------------------------------------
	if ( GetPosition().z < 0 )
		return;		// we've left the dungeon.  No need to do vision checks.
	mapBase *realMap = hnDungeon::GetLevel( GetPosition().z );
	assert(realMap);

	if ( m_map[ m_entity->GetPosition().z ] == NULL )
		m_map[ m_entity->GetPosition().z ] = new mapClient( realMap->GetWidth(), realMap->GetHeight() );

	mapClient *map = m_map[ m_entity->GetPosition().z ];
	assert( map );

	//---------------------------------------------------------------
	//  Copy visible squares of real map into our vision map
	//---------------------------------------------------------------
	
	realMap->UpdateMap( map );
}

void
hnPlayer::SendUpdate()
{
	//---------------------------------------------------------------
	// in case we need to send a packet, tell the server to
	// start building one.  We'll tell it to transmit it at the
	// end of this function.  If no data is actually put into the
	// packet, then the server won't actually send anything.
	//---------------------------------------------------------------
	netServer::GetInstance()->StartMetaPacket(m_playerID);
	
	//---------------------------------------------------------------
	//  Sanity checks, and be sure that our vision map has been
	//  allocated.
	//---------------------------------------------------------------
	if ( GetPosition().z < 0 )
		return;

	mapBase *realMap = hnDungeon::GetLevel( GetPosition().z );
	assert(realMap);

	//  Allocate map storage if we haven't been here before.
	if ( m_map[ m_entity->GetPosition().z ] == NULL )
		m_map[ m_entity->GetPosition().z ] = new mapClient( realMap->GetWidth(), realMap->GetHeight() );

	mapClient *map = m_map[ m_entity->GetPosition().z ];
	assert( map );

	// Send an update packet to our player.
	netMapUpdateBBox update;
	
	hnPoint2D 	tlpos = map->GetTopLeftChanged();
	hnPoint2D	brpos = map->GetBottomRightChanged();
	
	//------------------------------------------------------------------------
	// if something has changed in our vision since we sent our last
	// update packet, tlpos and brpos will define the topleft and bottomright
	// points of a rectangle.  If not, tlpos will be the bottom right corner
	// of the map, and brpos will be the top left corner of the map.
	//------------------------------------------------------------------------
	if ( tlpos.x <= brpos.x && tlpos.y <= brpos.y )	
	{
		map->ResetChanged();		// reset our map's memory of what has changed, since we're updating the client.

		netServer::GetInstance()->SendClientLocation( GetPosition() );
	
		update.loc.Set( tlpos.x, tlpos.y, m_entity->GetPosition().z );
		update.width = (brpos.x - tlpos.x) + 1;
		update.height = (brpos.y - tlpos.y) + 1;
	
		update.material = new sint16[update.width * update.height];
		update.wall = new sint16[update.width * update.height];
		update.entityType = new sint8[update.width * update.height];

		update.object = new (objDescription *)[update.width * update.height];
		update.objectCount = new uint16[update.width * update.height];
		
		for ( int j = 0; j < update.height; j++ )
			for ( int i = 0; i < update.width; i++)
			{
				int x = update.loc.x+i;
				int y = update.loc.y+j;
			
       				update.material[i+(j*update.width)] = map->MaterialAt(x,y);
   				update.wall[i+(j*update.width)] = map->WallAt(x,y);
				update.entityType[i+(j*update.width)] = map->MapTile(x,y).entity;
				
				int objectCount = map->MapTile(x,y).objectCount;

				objDescription * list = NULL;
				if ( objectCount > 0 )
					list = new objDescription[objectCount];
					
				update.objectCount[i+(j*update.width)] = objectCount;
				update.object[i+(j*update.width)] = list;

				for ( int k = 0; k < objectCount; k++ )
					update.object[i+(j*update.width)][k] = map->MapTile(x,y).object[k];
			}
	
		netServer::GetInstance()->SendMapUpdateBBox( update );

		//delete [] update.material;
		//delete [] update.wall;
		//delete [] update.entityType;
	
	}
	
	if ( m_group )
	{
		//----------------------------------------------------------------------------------
		//  Put some group update data into the same metapacket, just for convenience.
		//  No point sending those nasty big TCP headers more often then necessary!
		//  
		//  Remember that we're always in a 'group', even if we're the only player in
		//  it!  This tells the client how many members in the group, and how many of
		//  those players have submitted turn data so far!
		//----------------------------------------------------------------------------------
		int groupMembers = m_group->GetPlayerCount();
		int groupMembersWithTurns = m_group->QueuedTurnCount();
		
		if ( groupMembers != m_lastSentGroupPlayerCount ||
			groupMembersWithTurns != m_lastSentGroupPlayerQueuedTurns )
		{
			netServer::GetInstance()->SendGroupData( groupMembers, groupMembersWithTurns, HasQueuedTurn() );

			m_lastSentGroupPlayerCount = groupMembers;
			m_lastSentGroupPlayerQueuedTurns = groupMembersWithTurns;
		}
	}

	if ( m_entity->GetStatus() )
	{
		//----------------------------------------------------------
		//  Send data on any updates to our health, stats, etc.
		//----------------------------------------------------------

		netServer::GetInstance()->SendStatus( m_entity->GetStatus() );
	}
	
	//----------------------------------------------------------------
	// Send the metapacket now.  The server will recognize if there's
	// no data in the packet, and will abort the send (returning
	// false, although we don't really care about that).
	//----------------------------------------------------------------
	netServer::GetInstance()->TransmitMetaPacket();	// all done!
}

void
hnPlayer::RefreshMap( int level )
{
	// Send our visibility data on this level to our player.

	netServer::GetInstance()->StartMetaPacket(m_playerID);

	mapBase *realMap = hnDungeon::GetLevel( level );
	
	assert(realMap);


	//  Allocate map storage if we haven't been here before.
	if ( m_map[ level ] == NULL )
		m_map[ level ] = new mapClient( realMap->GetWidth(), realMap->GetHeight() );

	mapClient *map = m_map[ level ];
	assert( map );

	netMapUpdateBBox update;
	
	//--------------------------------------------------------
	//  Instead of sending the whole level, send only the
	//  bounding box around what the player has seen on the
	//  level.   This function should be VERY rarely called!
	//  (usually only immediately after resuming a saved
	//  game.  Since we don't currently save games, this
	//  function should be EXTREMELY rarely called!
	//
	//  ie:  NEVER!  :)
	//
	//--------------------------------------------------------
	
	hnPoint2D 	tlpos = map->GetTopLeftMaxChanged();
	hnPoint2D	brpos = map->GetBottomRightMaxChanged();
	
	update.loc.Set( tlpos.x, tlpos.y, level );
	update.width = ( brpos.x - tlpos.x ) + 1;
	update.height = ( brpos.y - tlpos.y ) + 1;

	update.material = new sint16[update.width * update.height];
	update.wall = new sint16[update.width * update.height];
	update.entityType = new sint8[update.width * update.height];

	for ( int j = 0; j < update.height; j++ )
		for ( int i = 0; i < update.width; i++)
		{
			int x = update.loc.x+i;
			int y = update.loc.y+j;
			
       			update.material[i+(j*update.width)] = map->MaterialAt(x,y);
   			update.wall[i+(j*update.width)] = map->WallAt(x,y);
			update.entityType[i+(j*update.width)] = map->MapTile(x,y).entity;
		}
	
	netServer::GetInstance()->SendMapUpdateBBox( update );

	delete [] update.material;
	delete [] update.wall;
	delete [] update.entityType;

	netServer::GetInstance()->TransmitMetaPacket();	// all done!
}
