#include <stdio.h>
#include "HN_Player.h"
#include "NET_Server.h"
#include "HN_Dungeon.h"
#include "MAP_Base.h"

#include "ENT_Human.h"
#include "ENT_GridBug.h"

#include "assert.h"

extern hnPoint offsetVector[10];

hnPlayer::hnPlayer( int playerID, const hnPoint &where ):
	m_playerID(playerID)
{
	m_mapCount = hnDungeon::GetInstance()->GetLevelCount();
	m_map = new (mapBase *)[m_mapCount];

	m_entity = new entHuman( where );
//	Ever want to play as a Grid Bug?  Comment the line above and 
//	uncomment the next line, and you will!  Yes, we already
//	support non-human players!  Hooray!
//	------------------------------------------------------------
//	m_entity = new entGridBug( where );	
	
	hnDungeon::GetInstance()->GetLevel(where.z)->PutEntityAt( m_entity, where.x, where.y );
	
	for ( int i = 0; i < m_mapCount; i++ )
		m_map[i] = NULL;
	
}

hnPlayer::~hnPlayer()
{
	hnDungeon::GetInstance()->GetLevel(GetPosition().z)->RemoveEntity( m_entity );

	delete m_entity;

	for ( int i = 0; i < m_mapCount; i++ )
		delete m_map[i];
	
	delete [] m_map;
}

bool
hnPlayer::IsValidMove( hnDirection dir )
{
	return m_entity->IsValidMove( dir );
}

void
hnPlayer::Move( hnDirection dir )
{
	m_entity->Move( dir );
	// is there anything else we need to do here?  Probably..
}

const hnPoint &
hnPlayer::GetPosition()
{
	return m_entity->GetPosition();
}

#define MAX_TALK_DISTANCE	(5)
#define MAX_TALK_DISTANCE_SQ	(MAX_TALK_DISTANCE * MAX_TALK_DISTANCE)

void
hnPlayer::Listen( const hnPoint & position, char * message )
{
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
	netServer::GetInstance()->StartMetaPacket(m_playerID);
	netServer::GetInstance()->SendMessage( message );
	netServer::GetInstance()->TransmitMetaPacket();	// all done!
}

bool
hnPlayer::CanSee( const hnPoint & where )
{
	mapBase *map = m_map[ GetPosition().z ];

	return map->MapTile(where.x, where.y).visible;
}

void
hnPlayer::SetName( char * name )
{
	if ( m_entity )
	{
		m_entity->SetName(name);
	}
}

char *
hnPlayer::GetName()
{
	char * result = NULL;
	
	if ( m_entity )
	{
		result = m_entity->GetName();
	}

	return result;
}



void
hnPlayer::PostTurn()
{
	// Send an update packet to our player.

	netServer::GetInstance()->StartMetaPacket(m_playerID);
	netServer::GetInstance()->SendClientLocation( GetPosition() );

	mapBase *realMap = hnDungeon::GetLevel( GetPosition().z );
	
	assert(realMap);


	//  Allocate map storage if we haven't been here before.
	if ( m_map[ m_entity->GetPosition().z ] == NULL )
		m_map[ m_entity->GetPosition().z ] = new mapBase( realMap->GetWidth(), realMap->GetHeight(), GetPosition().z );

	mapBase *map = m_map[ m_entity->GetPosition().z ];
	assert( map );

	//---------------------------------------------------------------
	//  Calculate visibility
	//---------------------------------------------------------------
	
	map->UpdateVisibility( m_entity->GetPosition(), realMap );
	
	netMapUpdateBBox update;
	
	hnPoint2D 	tlpos = map->GetTopLeftVisibility();
	hnPoint2D	brpos = map->GetBottomRightVisibility();
		
	update.loc.Set( tlpos.x, tlpos.y, m_entity->GetPosition().z );
	update.width = (brpos.x - tlpos.x) + 1;
	update.height = (brpos.y - tlpos.y) + 1;
	
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
			update.entityType[i+(j*update.width)] = map->MapTile(x,y).entityType;
		}
	
	netServer::GetInstance()->SendMapUpdateBBox( update );

	delete [] update.material;
	delete [] update.wall;
	delete [] update.entityType;
	
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
		m_map[ level ] = new mapBase( realMap->GetWidth(), realMap->GetHeight(), level );

	mapBase *map = m_map[ level ];
	assert( map );

	netMapUpdateBBox update;
	
	//--------------------------------------------------------
	//  Instead of sending the whole level, send only the
	//  bounding box around what the player has seen on the
	//  level.  (And someday soon, we'll even only send THAT
	//  if the client hasn't visited the level before during
	//  this connect!)
	//--------------------------------------------------------
	
	hnPoint2D 	tlpos = map->GetTopLeftMaxVisibility();
	hnPoint2D	brpos = map->GetBottomRightMaxVisibility();
	
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
			update.entityType[i+(j*update.width)] = map->MapTile(x,y).entityType;
		}
	
	netServer::GetInstance()->SendMapUpdateBBox( update );

	delete [] update.material;
	delete [] update.wall;
	delete [] update.entityType;

	netServer::GetInstance()->TransmitMetaPacket();	// all done!
}
