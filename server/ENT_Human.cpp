#include "ENT_Human.h"


entHuman::entHuman(const hnPoint & where, bool hasPlayer):
	entBase( ENTITY_Human, where, hasPlayer )
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
