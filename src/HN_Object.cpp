#include "HN_Object.h"

hnObject::hnObject( hnEntityType type, const hnPoint &where ):
	hnEntity(type,where),
	m_blesscurse(BC_Uncursed)
{
}

hnObject::~hnObject()
{
}
