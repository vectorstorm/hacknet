#ifndef __ENT_BASE_H__
#define __ENT_BASE_H__

#include "HN_Enum.h"
#include "HN_Point.h"
#include "ENT_Types.h"
#include "HN_Status.h"
#include "OBJ_Types.h"

class hnPlayer;
class objBase;

#define INVENTORY_MAX  (52)

class entBase
{
	entType			m_type;
	hnPoint			m_position;

	hnStatus		*m_status;
	objBase			*m_inventory;
	objBase			*m_wieldedObject;

	hnPlayer		*m_player;	// if I'm player controlled, this is my player.
	
protected:	
	int			m_hitPoints;
	int			m_maxHitPoints;
	bool			m_changedLevel;	// needs to refresh all our vision
	
public:
	
	enum{
		AT_Miss,
		AT_Hit,
		AT_Kill
	};	// result of attack
	
				entBase( entType type, const hnPoint &, hnPlayer * player = NULL );
	virtual			~entBase();
	
//	void			SetName( char * name );
	virtual const char *	GetName() = 0;

	void			GetFullName( char * buffer, int bufferSize );
	
	bool			IsAPlayer() { return (m_player != NULL); }
	hnPlayer *		GetPlayer() { return m_player; }
	bool			IsAlive() { return m_status->Alive(); }
	
	const hnPoint &		GetPosition();
	entType			GetType() { return m_type; }
	hnStatus *		GetStatus() { return m_status; }
	objBase *		GetInventory() { return m_inventory; }

	virtual bool		RollToHit( entBase *target );
	virtual sint16		RollDamage( entBase *target );
	
	void			SetPosition( const hnPoint & );
	
	virtual void		Think() {}			// Called to decide what to do each turn.

	virtual bool		IsValidMove( hnDirection direction );	// Called to check to see if a move is legal.
	virtual bool		IsValidMoveDestination( const hnPoint & destination );
	virtual bool		FindMoveDestination( hnPoint & destination, hnDirection direction );  // pass in the direction and a place to store the destination point.  Returns true if it's a legal move.
	virtual void		Move( hnDirection direction );
	virtual void		MoveTo( const hnPoint & position );
	
	virtual bool		IsValidAttack( hnDirection direction );	// Called to check to see if a move is legal.
	virtual entBase *	GetAttackTarget( hnDirection direction );
	virtual int		Attack( hnDirection direction );

	virtual bool		IsValidTake( objBase *object );
	virtual bool		Take( objBase *object, uint8 count );

	virtual bool		IsValidInventoryItem( objBase *object );

	virtual bool		Drop( objBase *object, uint8 count );
	virtual bool		Wield( objBase *object );
	virtual bool		Wear( objBase *object );
	virtual bool		Remove( objBase *object );
	virtual bool		Quaff( objBase *object );
	
	virtual void		PreTurn() {}			// Called before processing a turn
	virtual void		PostTurn() {}			// Called after processing a turn
	
	//virtual void		Move( const hnPoint & offset ) {}
	
protected:
	
	void			Unlink();
};

#endif // __ENT_BASE_H__
