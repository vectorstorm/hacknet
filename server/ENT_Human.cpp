#include "ENT_Human.h"


entHuman::entHuman(const hnPoint & where, bool hasPlayer):
	entBase( ENTITY_Human, where, hasPlayer )
{
}

entHuman::~entHuman()
{
}

void
entHuman::Think()
{
}
