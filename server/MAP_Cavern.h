#ifndef __MAP_CAVERN_H__
#define __MAP_CAVERN_H__

#include "MAP_Base.h"

class mapCavern : public mapBase
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
				mapCavern( uint8 width, uint8 height, uint8 depth );
	virtual			~mapCavern();

	virtual void		Generate();
};

#endif // __HN_CAVERN_MAP_H__
