#ifndef __HN_OBJECT_H__
#define __HN_OBJECT_H__

#include "HN_Entity.h"
#include "HN_Types.h"

class hnObject: public hnEntity
{
	enum {
		BC_Uncursed,
		BC_Blessed,
		BC_Cursed
	};
	uint8		m_blesscurse;
	
public:
			hnObject( hnEntityType type, const hnPoint & where );
	virtual 	~hnObject();

	bool		Blessed() { return (m_blesscurse == BC_Blessed); }
	bool		Cursed() { return (m_blesscurse == BC_Cursed); }
};



#endif //__HN_OBJECT_H__
