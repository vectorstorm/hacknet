#include "ENT_Human.h"


entHuman::entHuman(const hnPoint & where, hnPlayer *player):
	entBase( ENTITY_Human, where, player )
{
	SetName("human");
}

entHuman::~entHuman()
{
}

void
entHuman::Think()
{
}
