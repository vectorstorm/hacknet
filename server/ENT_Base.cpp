#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ENT_Base.h"
#include "OBJ_Base.h"
#include "MAP_Base.h"
#include "HN_Dungeon.h"
#include "HN_Player.h"
#include "HN_Group.h"

#include "assert.h"

entBase::entBase( entType type, const hnPoint & pos, hnPlayer *player ):
	m_type(type),
	m_position(pos),
	m_wieldedObject(NULL),
	m_player(player),
	m_hitPoints(1),		// set some minimum number of hitpoints so we don't die before somebody sets
	m_maxHitPoints(1)	// the proper amount on us.
{
	m_status = new hnStatus(1);
	m_inventory = new objBase(OBJ_TYPE_Illegal);
}

entBase::~entBase()
{
	delete m_inventory;
	delete m_status;
}

/*void
entBase::SetName( char * name )
{
	strncpy( m_name, name, MAX_NAME_BYTES );
	// make sure that we've got a null at least at the end of our name, if not earlier
	m_name[MAX_NAME_BYTES-1] = '\0';
}*/

void
entBase::GetFullName( char * buffer, int bufferSize )
{
	if ( m_player )
		snprintf(buffer, bufferSize, "%s the %s", m_player->GetName(), GetName() );
	else
		snprintf(buffer, bufferSize, "the %s", GetName());
}

const hnPoint &
entBase::GetPosition()
{
	return m_position;
}

void
entBase::SetPosition( const hnPoint & pos )
{
	m_position = pos;
}

bool
entBase::IsValidMove( hnDirection dir )
{
	hnPoint point;

	return FindMoveDestination(point,dir);
}

bool
entBase::IsValidTake( objBase *object )
{
	// check to see if the passed object is in the same
	// square with us.
	
	bool result = false;
	mapBase *map = hnDungeon::GetLevel( GetPosition().z );

	if ( map )
	{
		hnPoint pos = GetPosition();
		mapTile &tile = map->MapTile( pos.x, pos.y );
		
		for ( int i = 0; i < tile.object->ObjectCount(); i++ )
		{
			if ( object == tile.object->GetObject(i) )
			{
				result = true;
				break;
			}
		}
	}

	return result;
}

/*objBase *
entBase::GetTakeTarget( const objDescription &desc, uint8 stackID )
{
	objBase *result = NULL;
	
	mapBase *map = hnDungeon::GetLevel( GetPosition().z );
	
	if ( map )
	{
		hnPoint pos = GetPosition();
		objBase *realObject = map->MapTile( pos.x, pos.y ).object->GetObject(stackID);

		if ( realObject && realObject->PartialMatch(desc) )
			result = realObject;
	}
	return result;
}*/

bool
entBase::Take( objBase *object, uint8 count )
{
	bool result = false;
	
	if ( IsValidTake(object) )
	{
		mapBase *map = hnDungeon::GetLevel( GetPosition().z );
	
		if ( map )	// this should never fail.
		{
			hnPoint pos = GetPosition();
			objBase *taken = map->MapTile(pos.x,pos.y).object->RemoveObjectQuantity(object, count);
			m_inventory->AddObject(taken);
			result = true;
		}
		else
		{
			printf("Weird -- GetTakeTarget() worked, but Take() didn't.\n");
		}
	}
	return result;
}

bool
entBase::IsValidInventoryItem( objBase *object )
{
	bool result = false;
	
	for ( int i = 0; i < m_inventory->ObjectCount(); i++ )
	{
		objBase *shuttle = m_inventory->GetObject(i);
		if ( shuttle == object )
		{
			result = true;
			break;
		}
	}

	return result;
}

bool
entBase::Drop( objBase *object, uint8 count )
{
	bool result = false;
	if ( IsValidInventoryItem(object) )
	{
		if ( !object->IsWorn() )
		{	
			mapBase *map = hnDungeon::GetLevel( GetPosition().z );
		
			if ( map )	// this should never fail.
			{
				objBase *dropped = m_inventory->RemoveObjectQuantity(object,count);
				dropped->SetWieldedPrimary(false);
				dropped->SetWieldedSecondary(false);
				hnPoint pos = GetPosition();
				map->MapTile(pos.x,pos.y).object->AddObject(dropped);
				result = true;
			}
		}
	}
	return result;
}

bool
entBase::Wield( objBase *object )
{
	bool result = false;
	if ( IsValidInventoryItem(object) || object == NULL )
	{
		for ( int i = 0; i < m_inventory->ObjectCount(); i++ )
			m_inventory->GetObject(i)->SetWieldedPrimary(false);
			
		if ( object != NULL )
			result = object->SetWieldedPrimary(true);
		m_wieldedObject = object;
	}
	return result;
}


bool
entBase::Wear( objBase *object )
{
	bool result = false;
	if ( IsValidInventoryItem(object) || object == NULL )
	{
		if ( object != NULL )
			result = object->SetWorn(true);
	}
	return result;
}

bool
entBase::Remove( objBase *object )
{
	bool result = false;
	if ( IsValidInventoryItem(object) || object == NULL )
	{
		if ( object != NULL )
			result = object->SetWorn(false);
	}
	return result;
}


bool
entBase::IsValidMoveDestination( const hnPoint &destination )
{
	bool legalMove = false;
	
	if ( destination.z < 0 )
		return true;	// it's legal to leave the dungeon.. but don't go checking those negative levels for walls!
	
	if ( hnDungeon::GetLevel(destination.z)->WallAt(destination.x,destination.y) & WALL_Passable )
		if ( hnDungeon::GetLevel(destination.z)->MapTile(destination.x,destination.y).entity == NULL )
			legalMove = true;

	return legalMove;
}

bool
entBase::FindMoveDestination( hnPoint &destination, hnDirection dir )
{
	bool legalMove = false;
	hnPoint potentialPos = GetPosition();
	
	// validate the direction we've been given, and return true if we're
	// able to go that way; false if not.

	if ( dir >= DIR_North && dir <= DIR_NorthWest )
		potentialPos.Increment(dir);
	else	// dir == DIR_Up || dir == DIR_Down -- we can't get here otherwise.  (hnGame::ClientMove() ignores other 'directions')
	{
		hnPoint currentPos = GetPosition();
		if ( dir == DIR_Up )
		{
			if ( hnDungeon::GetLevel(currentPos.z)->WallAt(currentPos.x,currentPos.y) & WALL_StairsUp )
			{
				if ( currentPos.z-1 >= 0 )	// if we're not leaving the dungeon...
				{
					hnPoint2D stairPos = hnDungeon::GetLevel(currentPos.z-1)->GetDownStairs();
					potentialPos.Set( stairPos.x, stairPos.y, currentPos.z-1 );
				}
				else
				{
					// we're leaving the dungeon, so don't worry about looking for the stairs to
					// be clear on the other side.
					potentialPos.z = currentPos.z-1;
					legalMove = true;
				}
			}
		}
		else if ( dir == DIR_Down )
		{
			if ( hnDungeon::GetLevel(currentPos.z)->WallAt(currentPos.x,currentPos.y) & WALL_StairsDown )
			{
				hnPoint2D stairPos = hnDungeon::GetLevel(currentPos.z+1)->GetUpStairs();
				potentialPos.Set( stairPos.x, stairPos.y, currentPos.z+1 );
			}
		}
	}
	
	legalMove = IsValidMoveDestination( potentialPos );
	
	if ( legalMove )
		destination = potentialPos;
	
	return legalMove;
}

bool
entBase::RollToHit( entBase *target)
{
	int hitChance = 1 + m_status->Level() + 10;
	
	if ( target->GetStatus()->Paralyzed() )
		hitChance += 2;
	
	bool hit = ( hitChance > (rand() % 20) );
	
	return hit;
}

sint16
entBase::RollDamage( entBase *foe )
{
	int damage = 1;
	
	if ( m_wieldedObject )
		damage = m_wieldedObject->RollDamage(foe);

	// do other stuff in here too, like checking the 
	// object's damage type and comparing that to
	// strengths and weaknesses of the target.
	
	return damage;
}

void
entBase::Move( hnDirection dir )
{
	// final check for legality...
	if ( !IsValidMove(dir) )
		return;
	
	hnPoint pos = m_position;
	pos.Increment(dir);
	
	mapBase *origMap = hnDungeon::GetLevel( m_position.z );
	mapBase *map = hnDungeon::GetLevel( pos.z );
	
	if ( dir == DIR_Up )
	{
		if ( pos.z >= 0 )
		{
			hnPoint2D stairsPos = map->GetDownStairs();
			pos.x = stairsPos.x;
			pos.y = stairsPos.y;
		}
	}
	else if ( dir == DIR_Down )
	{
		hnPoint2D stairsPos = map->GetUpStairs();
		pos.x = stairsPos.x;
		pos.y = stairsPos.y;
	}
	
	origMap->RemoveEntity(this);
	if ( map )
		map->PutEntityAt(this, pos.x, pos.y);
	
	m_position = pos;
}

void
entBase::MoveTo( const hnPoint & pos )
{
	//------------------------------------------------
	// TODO: check to be sure that this is a legal position!
	//------------------------------------------------
	if ( !IsValidMoveDestination( pos ) )
		return;
		
	mapBase *origMap = hnDungeon::GetLevel( m_position.z );
	mapBase *map = hnDungeon::GetLevel( pos.z );
	
	origMap->RemoveEntity(this);
	if ( map )
		map->PutEntityAt(this, pos.x, pos.y);
	
	m_position = pos;
}

bool
entBase::IsValidAttack( hnDirection dir )
{
	bool valid = false;
	
	if ( dir >= 0 && dir < DIR_Up )
	{
		hnPoint target = GetPosition();
		target.Increment(dir);

		mapBase *map = hnDungeon::GetLevel( target.z );

		if ( map && map->MapTile(target.x, target.y).entity != NULL )
			valid = true;
	}
	
	return valid;
}

entBase *
entBase::GetAttackTarget( hnDirection dir )
{
	if ( !IsValidAttack(dir) )	// perhaps our target has already died?
		return NULL;
	
	hnPoint target = GetPosition();
	target.Increment(dir);

	mapBase *map = hnDungeon::GetLevel( target.z );

	entBase *foe = map->MapTile(target.x, target.y).entity;
	
	return foe;
}

int
entBase::Attack( hnDirection dir )
{
	if ( !IsValidAttack(dir) )	// perhaps our target has already died?
		return false;
	
	hnPoint target = GetPosition();
	target.Increment(dir);

	if ( m_wieldedObject )
	{
		// if the object is no longer in our inventory, we
		// aren't attacking with it any more.
		if ( !IsValidInventoryItem(m_wieldedObject) )
			m_wieldedObject = NULL;
	}

	mapBase *map = hnDungeon::GetLevel( target.z );

	entBase *foe = map->MapTile(target.x, target.y).entity;

	if ( RollToHit(foe) )
	{
		// we hit!
		foe->GetStatus()->TakeDamage( RollDamage(foe) );	// just do one hit point of damage for now.

		if ( foe->GetStatus()->Destroyed() )
		{
			// we killed it!
			map->RemoveEntity( foe );

			if ( foe->IsAPlayer() )
				foe->GetPlayer()->SetEntity(NULL);
			else
			{
				hnGroupManager::GetInstance()->RemoveEntity(foe);
				delete foe;
			}
			
			return AT_Kill;
		}
		
		return AT_Hit;
	}
	else
	{
		// we missed!
	}

	return AT_Miss;
}
