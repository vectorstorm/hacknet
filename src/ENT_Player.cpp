#include <stdio.h>
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
	m_mapCount = hnDungeon::GetInstance()->GetLevelCount();
	m_map = new (mapBase *)[m_mapCount];

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

bool
entPlayer::CanSee( const hnPoint & where )
{
	mapBase *map = m_map[ GetPosition().z ];

	return map->MapTile(where.x, where.y).visible;
}

void
entPlayer::PostTurn()
{
	// Send an update packet to our player.

	netServer::GetInstance()->StartMetaPacket(m_playerID);
	netServer::GetInstance()->SendClientLocation( GetPosition() );

	mapBase *realMap = hnDungeon::GetLevel( GetPosition().z );
	
	assert(realMap);


	//  Allocate map storage if we haven't been here before.
	if ( m_map[ GetPosition().z ] == NULL )
		m_map[ GetPosition().z ] = new mapBase( realMap->GetWidth(), realMap->GetHeight() );

	mapBase *map = m_map[ GetPosition().z ];
	assert( map );

	//---------------------------------------------------------------
	//  Calculate visibility
	//---------------------------------------------------------------

	map->UpdateVisibility( GetPosition(), realMap );
	
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
		printf("Just changed levels.\n");
		update.loc.Set(0,0,0);
		update.width = map->GetWidth();
		update.height = map->GetHeight();
		m_changedLevel = false;
	}
	else
	{
		hnPoint2D 	tlpos = map->GetTopLeftVisibility();
		hnPoint2D	brpos = map->GetBottomRightVisibility();
		update.loc.Set( tlpos.x, tlpos.y, 0 );
		update.width = (brpos.x - tlpos.x) + 1;
		update.height = (brpos.y - tlpos.y) + 1;
	}
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
