#include "ENT_Player.h"
#include "NET_Server.h"
#include "HN_Dungeon.h"
#include "MAP_Base.h"
#include "MAP_Client.h"

#include "assert.h"

extern hnPoint offsetVector[10];

entPlayer::entPlayer( int playerID, const hnPoint &where ):
	entBase(ENTITY_Player, where),
	m_playerID(playerID)
{
	m_mapCount = hnDungeon::GetInstance()->GetLevelCount();
	m_map = new (mapClient *)[m_mapCount];

	for ( int i = 0; i < m_mapCount; i++ )
		m_map[i] = NULL;
	
}

entPlayer::~entPlayer()
{
}

void
entPlayer::Move( hnDirection dir )
{
	entBase::Move( dir );
	// is there anything else we need to do here?  Probably..
}

#define MAX_TALK_DISTANCE	(5)
#define MAX_TALK_DISTANCE_SQ	(MAX_TALK_DISTANCE * MAX_TALK_DISTANCE)

void
entPlayer::Listen( const hnPoint & position, char * message )
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
entPlayer::Listen( char * message )
{
	netServer::GetInstance()->StartMetaPacket(m_playerID);
	netServer::GetInstance()->SendMessage( message );
	netServer::GetInstance()->TransmitMetaPacket();	// all done!
}

void
entPlayer::PostTurn()
{
	// Send an update packet to our player.

	netServer::GetInstance()->StartMetaPacket(m_playerID);
	netServer::GetInstance()->SendClientLocation( GetPosition() );

	mapBase *realMap = hnDungeon::GetLevel( GetPosition().z );
	
	assert(realMap);

	//---------------------------------------------------------------
	//  Update visibility here.  For now, just update the 3x3 box
	//  around the player.
	//---------------------------------------------------------------

	if ( m_map[ GetPosition().z ] == NULL )
	{
		// we're new on this level.

		m_map[ GetPosition().z ] = new mapClient( realMap->GetWidth(), realMap->GetHeight() );
	}

	mapClient *map = m_map[ GetPosition().z ];

	if ( map )
	{
		hnPoint pos = GetPosition() + offsetVector[DIR_NorthWest];

		for ( int j = 0; j < 3; j++ )
			for ( int i = 0; i < 3; i++ )
			{
				map->MaterialAt( pos.x+i, pos.y+j ) = realMap->MaterialAt( pos.x+i, pos.y+j );
				map->WallAt( pos.x+i, pos.y+j ) = realMap->WallAt( pos.x+i, pos.y+j );
				if ( realMap->MapTile( pos.x+i, pos.y+j ).entity == NULL )
					map->MapTile( pos.x+i, pos.y+j ).entity = ENTITY_None;
				else
					map->MapTile( pos.x+i, pos.y+j ).entity = realMap->MapTile( pos.x+i, pos.y+j ).entity->GetType();
			}
	}

	//---------------------------------------------------------------
	//  For now, we're just going to send the 3x3 box around the
	//  player.  Once true visibility is being cast, we'll send
	//  the the full bounding box around the visible area, with
	//  'unknown' data for the unseen squares within the visibile
	//  alrea's bounding box.
	//---------------------------------------------------------------


	netMapUpdateBBox update;
	
	if ( m_changedLevel )
	{
		update.loc.Set(0,0,0);
		update.width = map->GetWidth();
		update.height = map->GetHeight();
		m_changedLevel = false;
	}
	else
	{
		update.loc = GetPosition() + offsetVector[DIR_NorthWest];
		update.width = 3;
		update.height = 3;
	}
	update.material = new sint16[update.width * update.height];
	update.wall = new sint16[update.width * update.height];
	update.entityType = new sint8[update.width * update.height];

	for ( int j = 0; j < update.height; j++ )
		for ( int i = 0; i < update.width; i++)
		{
       			update.material[i+(j*update.width)] = map->MaterialAt(update.loc.x+i,update.loc.y+j);
   			update.wall[i+(j*update.width)] = map->WallAt(update.loc.x+i,update.loc.y+j);
			update.entityType[i+(j*update.width)] = map->MapTile(update.loc.x+i,update.loc.y+j).entity;
		}
	
	netServer::GetInstance()->SendMapUpdateBBox( update );

	delete [] update.material;
	delete [] update.wall;
	delete [] update.entityType;

	netServer::GetInstance()->TransmitMetaPacket();	// all done!

}
