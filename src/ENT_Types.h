#ifndef __ENT_TYPES_H__
#define __ENT_TYPES_H__

enum {
        ENTITY_None,                    // a mapobjectupdate with this type means nothing's here any more.
		        
        ENTITY_Monster,      	        // there should be one of these for every type of monster
        ENTITY_Player,

        ENTITY_MAX
};
typedef uint8 entType;

#endif //__ENT_TYPES_H__
