#include "OBJ_Definition.h"
#include "OBJ_Convert.h"

#define OBJECT(obj,bits,prop,type,prob,delay,weight,cost,sdam,ldam,oc1,oc2,nut,color) \
	{obj,bits,prop,type,delay,COLOR_FIELD(color), \
	prob,weight,cost,sdam,0,ldam,0,oc1,oc2,nut}

#define WEAPON(name,appearance,nameknown,merge,big,prob,weight,cost,sdam,ldam,hitbonus,type,subtype,material,color) \
	OBJECT( \
		OBJ(name,appearance), BITS(nameknown,merge,true,false,false,true,false,false,big,false,type,subtype,material), \
		0, Weapon, prob, 0, \
		weight, cost, sdam, ldam, hitbonus, 0, weight, color )

#define OBJ(name,desc) name,desc
#define BITS(nameknown,merge,usesknown,container,magic,charged,unique,nowish,big,tough,dir,subtype,material) \
	nameknown,merge,usesknown,0,magic,charged,unique,nowish,big,tough,dir,material,subtype
#define COLOR_FIELD(X) (X)

objPrototype g_objectPrototype[] =
{
WEAPON("dagger", (char *)0,
        1, 1, 0, 30, 10,  4,  4,  3, 2, P,   P_DAGGER, IRON, HI_METAL),
WEAPON("elven dagger", "runed dagger",
        0, 1, 0, 10, 10,  4,  5,  3, 2, P,   P_DAGGER, WOOD, HI_WOOD),
WEAPON("orcish dagger", "crude dagger",
        0, 1, 0, 12, 10,  4,  3,  3, 2, P,   P_DAGGER, IRON, CLR_BLACK),
WEAPON("silver dagger", (char *)0,
        1, 1, 0,  3, 12, 40,  4,  3, 2, P,   P_DAGGER, SILVER, HI_SILVER),
WEAPON("athame", (char *)0,
        1, 1, 0,  0, 10,  4,  4,  3, 2, S,   P_DAGGER, IRON, HI_METAL),
WEAPON("scalpel", (char *)0,
        1, 1, 0,  0,  5,  4,  3,  3, 2, S,   P_KNIFE, IRON, HI_METAL),
WEAPON("knife", (char *)0,
        1, 1, 0, 20,  5,  4,  3,  2, 0, P|S, P_KNIFE, IRON, HI_METAL),
WEAPON("stiletto", (char *)0,
        1, 1, 0,  5,  5,  4,  3,  2, 0, P|S, P_KNIFE, IRON, HI_METAL),
WEAPON("worm tooth", (char *)0,
        1, 0, 0,  0, 20,  2,  2,  2, 0, 0,   P_KNIFE, 0, CLR_WHITE),
WEAPON("crysknife", (char *)0,
        1, 0, 0,  0, 20,100, 10, 10, 3, P,   P_KNIFE, MINERAL, CLR_WHITE),

WEAPON("axe", (char *)0,
        1, 0, 0, 40, 60,  8,  6,  4, 0, S,   P_AXE, IRON, HI_METAL),
WEAPON("battle-axe", "double-headed axe",
        0, 0, 1, 10,120, 40,  8,  6, 0, S,   P_AXE, IRON, HI_METAL),
                                                /* "double-bitted" ? */

/* swords */
WEAPON("short sword", (char *)0,
        1, 0, 0,  8, 30, 10,  6,  8, 0, P,   P_SHORT_SWORD, IRON, HI_METAL),
WEAPON("elven short sword", "runed short sword",
        0, 0, 0,  2, 30, 10,  8,  8, 0, P,   P_SHORT_SWORD, WOOD, HI_WOOD),
WEAPON("orcish short sword", "crude short sword",
        0, 0, 0,  3, 30, 10,  5,  8, 0, P,   P_SHORT_SWORD, IRON, CLR_BLACK),
WEAPON("dwarvish short sword", "broad short sword",
        0, 0, 0,  2, 30, 10,  7,  8, 0, P,   P_SHORT_SWORD, IRON, HI_METAL),
WEAPON("scimitar", "curved sword",
        0, 0, 0, 15, 40, 15,  8,  8, 0, S,   P_SCIMITAR, IRON, HI_METAL),
WEAPON("silver saber", (char *)0,
        1, 0, 0,  6, 40, 75,  8,  8, 0, S,   P_SABER, SILVER, HI_SILVER),
WEAPON("broadsword", (char *)0,
        1, 0, 0,  8, 70, 10,  4,  6, 0, S,   P_BROAD_SWORD, IRON, HI_METAL),
                                                /* +d4 small, +1 large */
WEAPON("elven broadsword", "runed broadsword",
        0, 0, 0,  4, 70, 10,  6,  6, 0, S,   P_BROAD_SWORD, WOOD, HI_WOOD),
                                                /* +d4 small, +1 large */
WEAPON("long sword", (char *)0,
        1, 0, 0, 50, 40, 15,  8, 12, 0, S,   P_LONG_SWORD, IRON, HI_METAL),
WEAPON("two-handed sword", (char *)0,
        1, 0, 1, 22,150, 50, 12,  6, 0, S,   P_TWO_HANDED_SWORD, IRON, HI_METAL),
                                                /* +2d6 large */
WEAPON("katana", "samurai sword",
        0, 0, 0,  4, 40, 80, 10, 12, 1, S,   P_LONG_SWORD, IRON, HI_METAL),
/* special swords set up for artifacts */
WEAPON("tsurugi", "long samurai sword",
        0, 0, 1,  0, 60,500, 16,  8, 2, S,   P_TWO_HANDED_SWORD, METAL, HI_METAL),
                                                /* +2d6 large */
WEAPON("runesword", "runed broadsword",
        0, 0, 0,  0, 40,300,  4,  6, 0, S,   P_BROAD_SWORD, IRON, CLR_BLACK),
                                                /* +d4 small, +1 large */
                                                /* +5d2 +d8 from level drain */

/* polearms */
/* spear-type */
WEAPON("partisan", "vulgar polearm",
        0, 0, 1,  5, 80, 10,  6,  6, 0, P,   P_POLEARMS, IRON, HI_METAL),
                                                /* +1 large */
WEAPON("ranseur", "hilted polearm",
        0, 0, 1,  5, 50,  6,  4,  4, 0, P,   P_POLEARMS, IRON, HI_METAL),
                                                /* +d4 both */
WEAPON("spetum", "forked polearm",
        0, 0, 1,  5, 50,  5,  6,  6, 0, P,   P_POLEARMS, IRON, HI_METAL),
                                                /* +1 small, +d6 large */
WEAPON("glaive", "single-edged polearm",
        0, 0, 1,  8, 75,  6,  6, 10, 0, S,   P_POLEARMS, IRON, HI_METAL),
WEAPON("lance", (char *)0,
        1, 0, 0,  4,180, 10,  6,  8, 0, P,   P_LANCE, IRON, HI_METAL),
/* axe-type */
WEAPON("halberd", "angled poleaxe",
        0, 0, 1,  8,150, 10, 10,  6, 0, P|S, P_POLEARMS, IRON, HI_METAL),
                                                /* +1d6 large */
WEAPON("bardiche", "long poleaxe",
        0, 0, 1,  4,120,  7,  4,  4, 0, S,   P_POLEARMS, IRON, HI_METAL),
                                                /* +1d4 small, +2d4 large */
WEAPON("voulge", "pole cleaver",
        0, 0, 1,  4,125,  5,  4,  4, 0, S,   P_POLEARMS, IRON, HI_METAL),
                                                /* +d4 both */
WEAPON("dwarvish mattock", "broad pick",
        0, 0, 1, 13,120, 50, 12,  8,-1, B,   P_PICK_AXE, IRON, HI_METAL),

/* curved/hooked */
WEAPON("fauchard", "pole sickle",
        0, 0, 1,  6, 60,  5,  6,  8, 0, P|S, P_POLEARMS, IRON, HI_METAL),
WEAPON("guisarme", "pruning hook",
        0, 0, 1,  6, 80,  5,  4,  8, 0, S,   P_POLEARMS, IRON, HI_METAL),
                                                /* +1d4 small */
WEAPON("bill-guisarme", "hooked polearm",
        0, 0, 1,  4,120,  7,  4, 10, 0, P|S, P_POLEARMS, IRON, HI_METAL),
                                                /* +1d4 small */
/* other */
WEAPON("lucern hammer", "pronged polearm",
        0, 0, 1,  5,150,  7,  4,  6, 0, B|P, P_POLEARMS, IRON, HI_METAL),
                                                /* +1d4 small */
WEAPON("bec de corbin", "beaked polearm",
        0, 0, 1,  4,100,  8,  8,  6, 0, B|P, P_POLEARMS, IRON, HI_METAL),

/* bludgeons */
WEAPON("mace", (char *)0,
        1, 0, 0, 40, 30,  5,  6,  6, 0, B,   P_MACE, IRON, HI_METAL),
                                                /* +1 small */
WEAPON("morning star", (char *)0,
        1, 0, 0, 12,120, 10,  4,  6, 0, B,   P_MORNING_STAR, IRON, HI_METAL),
                                                /* +d4 small, +1 large */
WEAPON("war hammer", (char *)0,
        1, 0, 0, 15, 50,  5,  4,  4, 0, B,   P_HAMMER, IRON, HI_METAL),
                                                /* +1 small */
WEAPON("club", (char *)0,
        1, 0, 0, 12, 30,  3,  6,  3, 0, B,   P_CLUB, WOOD, HI_WOOD),
#ifdef KOPS
WEAPON("rubber hose", (char *)0,
        1, 0, 0,  0, 20,  3,  4,  3, 0, B,   P_WHIP, PLASTIC, CLR_BROWN),
#endif
WEAPON("quarterstaff", "staff",
        0, 0, 1, 11, 40,  5,  6,  6, 0, B,   P_QUARTERSTAFF, WOOD, HI_WOOD),
/* two-piece */
WEAPON("aklys", "thonged club",
        0, 0, 0,  8, 15,  4,  6,  3, 0, B,   P_CLUB, IRON, HI_METAL),
WEAPON("flail", (char *)0,
        1, 0, 0, 40, 15,  4,  6,  4, 0, B,   P_FLAIL, IRON, HI_METAL),
                                                /* +1 small, +1d4 large */
/* misc */
WEAPON("bullwhip", (char *)0,
        1, 0, 0,  2, 20,  4,  2,  1, 0, 0,   P_WHIP, LEATHER, CLR_BROWN),


};


int GetDefinitionCount()
{
	return ( sizeof(g_objectPrototype) / sizeof(objPrototype) );
}
