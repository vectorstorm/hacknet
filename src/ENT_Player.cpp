#include "ENT_Player.h"
#include "NET_Server.h"
#include "HN_Dungeon.h"
#include "MAP_Base.h"

#include "assert.h"

extern hnPoint offsetVector[10];

entPlayer::entPlayer( int playerID, const hnPoint &where ):
	entBase(ENTITY_Player, where),
	m_playerID(playerID)
{
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

	hnPoint pos = offsetVector[DIR_NorthWest] + GetPosition();
	mapBase *map = hnDungeon::GetLevel( GetPosition().z );
	
	assert(map);

	//---------------------------------------------------------------
	//  For now, we're just going to send the 3x3 box around the
	//  player.  Once true visibility is being cast, we'll send
	//  the the full bounding box around the visible area, with
	//  'unknown' data for the unseen squares within the visibile
	//  alrea's bounding box.
	//---------------------------------------------------------------


	netMapUpdateBBox update;
	update.loc = pos;
	update.width = 3;
	update.height = 3;
	update.material = new sint16[update.width * update.height];
	update.wall = new sint16[update.width * update.height];
	update.entityType = new sint8[update.width * update.height];

	for ( int j = 0; j < update.height; j++ )
		for ( int i = 0; i < update.width; i++)
		{
       			update.material[i+(j*update.width)] = map->MaterialAt(pos.x+i,pos.y+j);
   			update.wall[i+(j*update.width)] = map->WallAt(pos.x+i,pos.y+j);
			
			entBase *entity = map->MapTile(pos.x+i,pos.y+j).entity;
			if ( entity != NULL )
				update.entityType[i+(j*update.width)] = entity->GetType();
			else
				update.entityType[i+(j*update.width)] = ENTITY_None;
		}
	
	netServer::GetInstance()->SendMapUpdateBBox( update );

	delete [] update.material;
	delete [] update.wall;
	delete [] update.entityType;

	netServer::GetInstance()->TransmitMetaPacket();	// all done!

}
