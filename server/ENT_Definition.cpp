#include "ENT_Definition.h"
#include "ENT_Types.h"

#define MONSTER(nam,sym,lvl,gen,freq,atk,siz,mr1,mr2,flg1,flg2,flg3,col) \
	{nam,sym,lvl,gen,freq,atk,siz,mr1,mr2,flg1,flg2,flg3,col}

#define LEVEL(lvl,mov,ac,mr,aln) lvl,mov,ac,mr,aln
#define SIZE(wt,nut,pxl,snd,siz) wt,nut,pxl,snd,siz
#define ATTACK(at,ad,n,d) {at,ad,n,d}
#define ARRAY(a1,a2,a3,a4,a5,a6) {a1,a2,a3,a4,a5,a6}

#define NO_ATTACK {0,0,0,0}

entPrototype g_entityPrototype[] =
{
	MONSTER("giant ant", ENTITY_Ant,
		LEVEL(2,18,3,0,Align_None), (Geno_Genocidable|Geno_SmallGroup),3,
		ARRAY(	ATTACK(Attack_Bite, Damage_Physical, 1, 4),
			NO_ATTACK, NO_ATTACK, NO_ATTACK, NO_ATTACK, NO_ATTACK ),
		SIZE(10, 10, 0, Sound_Silent, Size_Tiny), 0, 0,
		M1_Animal | M1_NoHands | M1_Oviparous | M1_Carnivore,
		M2_Hostile, 0, Color_Brown)
};


int GetEntityDefinitionCount()
{
	return ( sizeof(g_entityPrototype) / sizeof(entPrototype) );
}
