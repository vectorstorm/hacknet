#ifndef __HN_PLAYER_H__
#define __HN_PLAYER_H__

#include "HN_Types.h"
#include "HN_Enum.h"
#include "HN_Point.h"
#include "OBJ_Types.h"
#include "ENT_Base.h"

//class mapBase;
class mapClient;
class entBase;
class hnGroup;

struct queuedTurn
{
	enum turnType{
		None,
		Move,
		Wait,
		Attack,
		Take,
		Drop,
		Wield,
		Wear,
		Remove,
		Quaff,
		UnWield
	};
	int type;

	union{
		struct{
			hnDirection direction;
		} move;
		struct{
			hnDirection direction;
		} attack;
		struct{
			objBase * object;
			objDescription desc;
			uint8 takeCount;
		} take;
		struct{
			objBase * object;
			objDescription desc;
			uint8 dropCount;
		} drop;
		struct{
			objBase * object;
			objDescription desc;
			uint8	inventorySlot;
		} wield;
		struct{
			objBase * object;
			objDescription desc;
			uint8	inventorySlot;
		} wear;
		struct{
			objBase * object;
			objDescription desc;
			uint8	inventorySlot;
		} remove;
		struct{
			objBase * object;
			objDescription desc;
			uint8	inventorySlot;
		} quaff;
	};
};

#define MAX_PLAYER_NAME_BYTES	(80)

class hnPlayer
{
	char		m_name[MAX_PLAYER_NAME_BYTES];
	
	int		m_playerID;				// the ID the server assigns to us.  NOT TO BE SAVED.
	entBase *	m_entity;
	
	mapClient **	m_map;
	int		m_mapCount;
	
	objDescription 	m_clientInventory[INVENTORY_MAX];
	objBase *	m_clientInventoryMapping[INVENTORY_MAX];
	
	hnGroup *	m_group;				// pointer to the group we're a part of.
	
	queuedTurn	m_queuedTurn;
	queuedTurn	m_completedTurn;
	
	int		m_lastSentGroupPlayerCount;
	int		m_lastSentGroupPlayerQueuedTurns;

	hnPoint		m_moveDestination;			// where will we be going?
	bool		m_movePending;				// are we moving this turn?
	
	bool		m_statsChanged;
	bool		m_hitPointsChanged;
	bool		m_spellPointsChanged;
	
public:
			hnPlayer( int playerID, const hnPoint & where );
	virtual		~hnPlayer();
	
	void		Die();
	
	void		SetName( char * name );
	void		SetEntity( entBase *entity );
	void		SetGroup( hnGroup * group ) { m_group = group; }
	
	const hnPoint &	GetPosition();
	int		GetID() { return m_playerID; }
	char *		GetName();

	void		GetFullName( char * buffer, int bufferSize );

	bool		IsAlive();

	
	bool		HasQueuedTurn();	// are we ready to process a turn?
	virtual bool	IsValidMove( hnDirection dir );
	virtual bool	IsValidAttack( hnDirection dir );
	virtual bool	IsValidInventoryItem( const objDescription &object, uint8 inventoryDrop );

	virtual bool	IsValidTake( const objDescription &object, uint8 stackID );
	objBase *	GetTakeTarget( const objDescription &object, uint8 stackID );

	virtual void	Listen( const hnPoint & position, char * message );
	virtual void	Listen( char * message );
	
	virtual void	See( const hnPoint & position, char * message );
	virtual void	See( const hnPoint & position, entBase * who, char * message );
	
	virtual bool	CanSee( const hnPoint & position );
	
	virtual void	DoAction();		// process our turn.
	virtual void	DoMove();		// do any movement associated with the turn we just processed.
	virtual void	RecalculateVision();	// calculate what we can see
	virtual void	UpdateVision();		// check again what's in the squares we previously calculated we could see.
	virtual void	SendUpdate();		// send all updates to client.

	virtual void	RefreshMap( int level );
	void		SendMapData( const hnPoint2D &topLeft, const hnPoint2D &bottomRight, int level );
	
	
	
	//  Queued Actions Beneath This Point ------------------------------------------
	virtual void	Move( hnDirection dir );
	virtual void	Attack( hnDirection dir );
	virtual void	Take( const objDescription &desc, uint8 stackID );
	virtual void	Drop( const objDescription &desc, uint8 inventorySlot );
	virtual void	Wield( const objDescription &desc, uint8 inventorySlot );
	virtual void	Wear( const objDescription &desc, uint8 inventorySlot );
	virtual void	Remove( const objDescription &desc, uint8 inventorySlot );
	virtual void	Quaff( const objDescription &desc, uint8 inventorySlot );
	virtual void	UnWield( );
	virtual void	Wait( );
	
};

#endif
