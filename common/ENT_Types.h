#ifndef __ENT_TYPES_H__
#define __ENT_TYPES_H__

enum {
        ENTITY_None,                    // a mapobjectupdate with this type means nothing's here any more.
		        
	ENTITY_Ant,				// types of creatures.
	ENTITY_GridBug,
	ENTITY_Human,

        ENTITY_MAX
};
typedef uint8 entType;

#endif //__ENT_TYPES_H__
