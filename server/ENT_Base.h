#ifndef __ENT_BASE_H__
#define __ENT_BASE_H__

#include "HN_Enum.h"
#include "HN_Point.h"
#include "ENT_Types.h"
#include "HN_Status.h"

class hnPlayer;

#define MAX_NAME_BYTES (128)

class entBase
{
	entType			m_type;
	hnPoint			m_position;

	hnStatus		*m_status;
	hnPlayer		*m_player;	// if I'm player controlled, this is my player.
	
protected:	
	char 			m_name[MAX_NAME_BYTES];
	int			m_hitPoints;
	int			m_maxHitPoints;
	bool			m_changedLevel;	// needs to refresh all our vision
	
public:
				entBase( entType type, const hnPoint &, hnPlayer * player = NULL );
	virtual			~entBase();
	
	void			SetName( char * name );
	char *			GetName();

	bool			IsAPlayer() { return (m_player != NULL); }
	bool			IsAlive() { return m_status->Alive(); }
	
	const hnPoint &		GetPosition();
	entType			GetType() { return m_type; }
	hnStatus *		GetStatus() { return m_status; }

	virtual bool		RollToHit( entBase *target );
	
	void			SetPosition( const hnPoint & );
	
	virtual void		Think() {}			// Called to decide what to do each turn.

	virtual bool		IsValidMove( hnDirection direction );	// Called to check to see if a move is legal.
	virtual bool		IsValidMoveDestination( const hnPoint & destination );
	virtual bool		FindMoveDestination( hnPoint & destination, hnDirection direction );  // pass in the direction and a place to store the destination point.  Returns true if it's a legal move.
	virtual void		Move( hnDirection direction );
	virtual void		MoveTo( const hnPoint & position );
	
	virtual bool		IsValidAttack( hnDirection direction );	// Called to check to see if a move is legal.
	virtual bool		Attack( hnDirection direction );
	
	virtual void		PreTurn() {}			// Called before processing a turn
	virtual void		PostTurn() {}			// Called after processing a turn
	
	//virtual void		Move( const hnPoint & offset ) {}
	
protected:
	
	void			Unlink();
};

#endif // __ENT_BASE_H__
