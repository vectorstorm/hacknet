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
#include "OBJ_Base.h"
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
	
	for ( int i = 0; i < INVENTORY_MAX; i++ )
	{
		m_clientInventory[i].type = OBJ_TYPE_Illegal;
		m_clientInventory[i].count = 0;
		m_clientInventoryMapping[i] = NULL;
	}
	
	m_queuedTurn.type = queuedTurn::None;
	m_completedTurn.type = queuedTurn::None;

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

bool
hnPlayer::IsValidInventoryItem( const objDescription &desc, uint8 inventorySlot )
{
	bool result = false;
	objBase *object = m_clientInventoryMapping[inventorySlot];
	
	if (  m_entity->IsValidInventoryItem( object ) )
	{
		result = object->PartialMatch(desc);
	}
	
	return result;
}

bool
hnPlayer::IsValidTake( const objDescription &desc, uint8 stackID )
{
	return (GetTakeTarget(desc,stackID) != NULL);
}

objBase *
hnPlayer::GetTakeTarget( const objDescription &desc, uint8 stackID )
{
	objBase *result = NULL;
	mapBase *map = hnDungeon::GetLevel( GetPosition().z );
	
	if ( map )
	{
		hnPoint pos = GetPosition();
		objBase *realObject = map->MapTile(pos.x,pos.y).object->GetObject(stackID);

		if ( realObject && realObject->PartialMatch(desc) )
			result = realObject;
	}
	return result;
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

void
hnPlayer::Take( const objDescription &desc, uint8 stackID )
{
	if ( IsValidTake( desc, stackID ) )
	{
		m_queuedTurn.type = queuedTurn::Take;
		m_queuedTurn.take.desc = desc;
		m_queuedTurn.take.object = GetTakeTarget(desc,stackID);
		m_queuedTurn.take.takeCount = desc.count;
	}
}

void
hnPlayer::Drop( const objDescription &object, uint8 inventorySlot )
{
	if ( IsValidInventoryItem( object, inventorySlot ) )
	{
		m_queuedTurn.type = queuedTurn::Drop;
		m_queuedTurn.drop.desc = object;
		m_queuedTurn.drop.object = m_clientInventoryMapping[inventorySlot];
		m_queuedTurn.drop.dropCount = object.count;
	}
	else
	{
		printf("Player requested illegal drop.\n");
	}
}

void
hnPlayer::Wield( const objDescription &object, uint8 inventorySlot )
{
	if ( IsValidInventoryItem( object, inventorySlot ) )
	{
		m_queuedTurn.type = queuedTurn::Wield;
		m_queuedTurn.wield.object = m_clientInventoryMapping[inventorySlot];
		m_queuedTurn.wield.desc = object;
		m_queuedTurn.wield.inventorySlot = inventorySlot;
	}
	else
	{
		printf("Received unwield request...\n");
		// unwield instead.
		m_queuedTurn.type = queuedTurn::Wield;
		m_queuedTurn.wield.object = NULL;
		m_queuedTurn.wield.desc.type = OBJ_TYPE_Illegal;
		m_queuedTurn.wield.inventorySlot = inventorySlot;
	}
}

void
hnPlayer::Wear( const objDescription &object, uint8 inventorySlot )
{
	if ( IsValidInventoryItem( object, inventorySlot ) )
	{
		m_queuedTurn.type = queuedTurn::Wear;
		m_queuedTurn.wear.object = m_clientInventoryMapping[inventorySlot];
		m_queuedTurn.wear.desc = object;
		m_queuedTurn.wear.inventorySlot = inventorySlot;
	}
	else
	{
		printf("Received illegal wear request...\n");
	}
}

void
hnPlayer::Remove( const objDescription &object, uint8 inventorySlot )
{
	if ( IsValidInventoryItem( object, inventorySlot ) )
	{
		m_queuedTurn.type = queuedTurn::Remove;
		m_queuedTurn.remove.object = m_clientInventoryMapping[inventorySlot];
		m_queuedTurn.remove.desc = object;
		m_queuedTurn.remove.inventorySlot = inventorySlot;
	}
	else
	{
		printf("Received illegal remove request...\n");
	}
}



void
hnPlayer::UnWield()
{
	printf("AIEEE!\n");
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
	bool success = false;

	switch ( m_queuedTurn.type )
	{
		case queuedTurn::Move:
			m_movePending = m_entity->FindMoveDestination( m_moveDestination, m_queuedTurn.move.direction );
			success = true;
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
			
			success = true;
			break;
		case queuedTurn::Take:
			success = m_entity->Take( m_queuedTurn.take.object, m_queuedTurn.take.takeCount );
			break;
		case queuedTurn::Drop:
			success = m_entity->Drop( m_queuedTurn.drop.object, m_queuedTurn.drop.dropCount );
			break;
		case queuedTurn::Wield:
			success = m_entity->Wield( m_queuedTurn.wield.object );
			if ( m_queuedTurn.wield.object )
				m_queuedTurn.wield.object->FillDescription(m_queuedTurn.wield.desc);
			break;
		case queuedTurn::Wear:
			success = m_entity->Wear( m_queuedTurn.wear.object );
			if ( m_queuedTurn.wear.object )
				m_queuedTurn.wear.object->FillDescription(m_queuedTurn.wear.desc);
			break;
		case queuedTurn::Remove:
			printf("Performing remove...\n");
			success = m_entity->Remove( m_queuedTurn.wear.object );
			if ( m_queuedTurn.remove.object )
				m_queuedTurn.remove.object->FillDescription(m_queuedTurn.remove.desc);
			break;
		case queuedTurn::Wait:
			// do nothing.
			break;
	}
	
	if ( success )
		m_completedTurn = m_queuedTurn;
	else
	{
		char *buffer="Request failed.";
		netServer::GetInstance()->StartMetaPacket(m_playerID);
		netServer::GetInstance()->SendMessage(buffer);
		netServer::GetInstance()->TransmitMetaPacket();
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
	//  Sanity checks, and be sure that our vision map has been
	//  allocated.
	//---------------------------------------------------------------
	if ( GetPosition().z < 0 )
	{
		printf("ERROR!  I'm at an illegal level depth!\n");
		return;
	}
	
	//---------------------------------------------------------------
      	// in case we need to send a packet, tell the server to
      	// start building one.  We'll tell it to transmit it at the
      	// end of this function.  If no data is actually put into the
      	// packet, then the server won't actually send anything.
	//---------------------------------------------------------------
	netServer::GetInstance()->StartMetaPacket(m_playerID);


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
	SendMapData( tlpos, brpos, GetPosition().z );
	
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
		//   the status system itself can tell when it needs to
		//   send updates, so we don't need to worry about putting
		//   that logic in here.
		//----------------------------------------------------------

		netServer::GetInstance()->SendStatus( m_entity->GetStatus() );
	}

	objBase *inventory = m_entity->GetInventory();
	uint8 updatedID = 0;

	if ( inventory )
	{
		//----------------------------------------------------------
		//  'inventory' is our real inventory.  m_clientInventory
		//  is the most recent inventory we've sent to our client.
		//  'm_clientInventoryMapping' is a conversion from our
		//  inventory slots to the objBase pointers they're
		//  mirroring.  Update our inventory slots based upon the
		//  current objBase pointers, and set the flag if anything
		//  has changed.
		//----------------------------------------------------------
		
		bool inventoryUpdated = false;
		bool slotUsed[INVENTORY_MAX];

		for ( int i = 0; i < INVENTORY_MAX; i++ )
			slotUsed[i] = false;
		
		for ( int i = 0; i < inventory->ObjectCount(); i++ )
		{
			objBase *object = inventory->GetObject(i);
			if ( object )
			{
				bool foundMatch = false;
				
				for ( int j = 0; j < INVENTORY_MAX; j++ )
				{
					if ( m_clientInventoryMapping[j] == object )
					{
						foundMatch = true;
						slotUsed[j] = true;
						if ( !object->ExactMatch(m_clientInventory[j]) )
						{
							updatedID = j;
							object->FillDescription( m_clientInventory[j] );
							netServer::GetInstance()->SendInventoryItem( m_clientInventory[j], j );
							inventoryUpdated = true;
							printf("Sending updated inventory slot %d\n",j);
						}
						break;
					}
				}

				if ( !foundMatch )
				{
					bool finished = false;
					objDescription result;
					object->FillDescription(result);
					// first look for a slot that was previously
					// used for this type of object...

					for ( int j = 0; j < INVENTORY_MAX; j++ )
					{
						if ( m_clientInventoryMapping[j] == NULL && m_clientInventory[j].type == result.type )
						{
							updatedID = j;
							finished = true;
							inventoryUpdated = true;
							m_clientInventory[j] = result;
							m_clientInventoryMapping[j] = object;
							slotUsed[j] = true;
							netServer::GetInstance()->SendInventoryItem( m_clientInventory[j], j );
							printf("Sending updated inventory slot %d\n",j);
							break;
						}
					}

					if ( !finished )
					{
						for ( int j = 0; j < INVENTORY_MAX; j++ )
						{
							if ( m_clientInventoryMapping[j] == NULL )
							{
								updatedID = j;
								m_clientInventoryMapping[j] = object;
								m_clientInventory[j] = result;
								finished = true;
								inventoryUpdated = true;
								slotUsed[j] = true;
								netServer::GetInstance()->SendInventoryItem( m_clientInventory[j], j );
								printf("Sending updated inventory slot %d\n",j);
								break;
							}
						}
					}
				}
			}
		}

		// now go through our inventory and turn off all slots
		// which are marked as unused.

		for ( int i = 0; i < INVENTORY_MAX; i++ )
		{
			if ( slotUsed[i] == false && m_clientInventory[i].count > 0 )
			{
				m_clientInventory[i].count = 0;
				inventoryUpdated = true;
				netServer::GetInstance()->SendInventoryItem( m_clientInventory[i], i );
							printf("Sending updated inventory slot %d\n",i);
			}
		}
		
		/*if ( inventoryUpdated )
		{
			printf("Updating inventory.\n");
			netInventory inven(INVENTORY_MAX);

			for ( int i = 0; i < INVENTORY_MAX; i++ )
				inven.SetObject(i, m_clientInventory[i]);

			netServer::GetInstance()->SendInventory(inven);
			
		}*/
	}

	//---------------------------------------------------------------
	// Check to see if we need to send the result of an action..
	//---------------------------------------------------------------

	switch( m_completedTurn.type )
	{
		case queuedTurn::Take:
			netServer::GetInstance()->SendTakenItem( m_completedTurn.take.desc, updatedID );
			break;
		case queuedTurn::Drop:
			netServer::GetInstance()->SendDroppedItem( m_completedTurn.drop.desc );
			break;
		case queuedTurn::Wield:
			//netServer::GetInstance()->SendWieldedItem( m_completedTurn.wield.desc, m_completedTurn.wield.inventorySlot );
			break;
	}
	m_completedTurn.type = queuedTurn::None;

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
	netServer::GetInstance()->StartMetaPacket(m_playerID);
	// Send our visibility data on this level to our player.
	mapBase *realMap = hnDungeon::GetLevel( level );
	
	assert(realMap);


	//  Allocate map storage if we haven't been here before.
	if ( m_map[ level ] == NULL )
		m_map[ level ] = new mapClient( realMap->GetWidth(), realMap->GetHeight() );

	mapClient *map = m_map[ level ];
	assert( map );

	
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
	
	SendMapData( tlpos, brpos, level );
	netServer::GetInstance()->TransmitMetaPacket();	// all done!
}

void
hnPlayer::SendMapData( const hnPoint2D &tlpos, const hnPoint2D &brpos, int level )
{
	// we must start and transmit the metapacket OUTSIDE this call!

	if ( tlpos.x <= brpos.x && tlpos.y <= brpos.y ) 
	{
		mapClient *map = m_map[ level ];
		assert( map );
		map->ResetChanged();
	
		netMapUpdateBBox update;

		update.loc.Set( tlpos.x, tlpos.y, level );
		update.width = ( brpos.x - tlpos.x ) + 1;
		update.height = ( brpos.y - tlpos.y ) + 1;

		update.material = new sint16[update.width * update.height];
		update.wall = new sint16[update.width * update.height];
		update.entityType = new sint8[update.width * update.height];
		update.objectCount = new uint16[update.width * update.height];
		update.object = new objDescription *[update.width * update.height];

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
	
		netServer::GetInstance()->SendClientLocation( GetPosition() );
		netServer::GetInstance()->SendMapUpdateBBox( update );
	}
	else
	{
		//printf( "No visual change.\n" );
	}
}
