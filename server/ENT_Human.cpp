#include "ENT_Human.h"

char * entHuman::s_name = "human";

entHuman::entHuman(const hnPoint & where, hnPlayer *player):
	entBase( ENTITY_Human, where, player )
{
}

entHuman::~entHuman()
{
}

void
entHuman::Think()
{
}
