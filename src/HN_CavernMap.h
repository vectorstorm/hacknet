#ifndef __HN_CAVERN_MAP_H__
#define __HN_CAVERN_MAP_H__

#include "HN_Map.h"

class hnCavernMap : public hnMap
{
	//-----------------------------------------------------
	//  Cavernous level generation
	//-----------------------------------------------------
	void			Randomise();
	void			HighPassOne();
	void			HighPassTwo();
	void			HighPassThree();
	void			Wallify();
	
public:
				hnCavernMap( unsigned int width, unsigned int height );
	virtual			~hnCavernMap();

	virtual void		Generate();
};

#endif // __HN_CAVERN_MAP_H__
