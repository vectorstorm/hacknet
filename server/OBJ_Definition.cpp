#include "OBJ_Definition.h"
#include "OBJ_Convert.h"

#define OBJECT(obj,bits,prop,type,prob,delay,weight,cost,sdam,ldam,oc1,oc2,nut,color) \
	{obj,bits,prop,type,delay,COLOR_FIELD(color), \
	prob,weight,cost,sdam,0,ldam,0,oc1,oc2,nut}

#define WEAPON(name,appearance,nameknown,merge,big,prob,weight,cost,sdam,ldam,hitbonus,type,subtype,material,color) \
	OBJECT( \
		OBJ(name,appearance), \
		BITS(nameknown,merge,true,false,false,true,false,false,big,false,type,subtype,material), \
		0, Weapon, prob, 0, \
		weight, cost, sdam, ldam, hitbonus, 0, weight, color )

#define PROJECTILE(name,app,kn,prob,wt,cost,sdam,ldam,hitbon,metal,sub,color) \
        OBJECT( \
                OBJ(name,app), \
                BITS(kn,1,1,0,0,1,0,0,0,0,P,sub,metal), 0, \
                Weapon, prob, 0, \
                wt, cost, sdam, ldam, hitbon, 0, wt, color )

#define BOW(name,app,kn,prob,wt,cost,hitbon,metal,sub,color) \
        OBJECT( \
		OBJ(name,app), BITS(kn,0,1,0,0,1,0,0,0,0,0,sub,metal), 0, \
		Weapon, prob, 0, \
		wt, cost, 2, 2, hitbon, 0, wt, color )
							
/* armor ... */
/* IRON denotes ferrous metals, including steel.
 * Only IRON weapons and armor can rust.
 * Only COPPER (including brass) corrodes.
 * Some creatures are vulnerable to SILVER.
 */
#define ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,sub,metal,c) \
        OBJECT( \
                OBJ(name,desc), BITS(kn,0,1,0,mgc,1,0,0,blk,0,0,sub,metal), \
		power, Armour, prob, delay, wt, cost, \
                0, 0, 10 - ac, can, wt, c )
#define HELM(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_Helm,metal,c)
#define CLOAK(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_Cloak,metal,c)
#define SHIELD(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,metal,c) \
        ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,ARM_Shield,metal,c)
#define GLOVES(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_Gloves,metal,c)
#define BOOTS(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
        ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_Boots,metal,c)
#define DRGN_ARMR(name,power,cost,ac,color) \
        ARMOR(name,(char *)0,1,0,1,power,0,5,40,cost,ac,0,ARM_Suit,DRAGON_HIDE,color)

#define OBJ(name,desc) name,desc
#define BITS(nameknown,merge,usesknown,container,magic,charged,unique,nowish,big,tough,dir,subtype,material) \
	nameknown,merge,usesknown,0,magic,charged,unique,nowish,big,tough,dir,material,subtype
#define COLOR_FIELD(X) (X)

objPrototype g_objectPrototype[] =
{
PROJECTILE("arrow", (char *)0,
                1, 55, 1, 2, 6, 6, 0, IRON, -P_BOW, HI_METAL),
PROJECTILE("elven arrow", "runed arrow",
                0, 20, 1, 2, 7, 6, 0, WOOD, -P_BOW, HI_WOOD),
PROJECTILE("orcish arrow", "crude arrow",
                0, 20, 1, 2, 5, 6, 0, IRON, -P_BOW, CLR_BLACK),
PROJECTILE("silver arrow", (char *)0,
                1, 12, 1, 5, 6, 6, 0, SILVER, -P_BOW, HI_SILVER),
PROJECTILE("ya", "bamboo arrow",
                0, 15, 1, 4, 7, 7, 1, METAL, -P_BOW, HI_METAL),
PROJECTILE("crossbow bolt", (char *)0,
                1, 55, 1, 2, 4, 6, 0, IRON, -P_CROSSBOW, HI_METAL),

WEAPON("dart", (char *)0,
        1, 1, 0, 60,  1,  2,  3,  2, 0, P,   -P_DART, IRON, HI_METAL),
WEAPON("shuriken", "throwing star",
        0, 1, 0, 35,  1,  5,  8,  6, 2, P,   -P_SHURIKEN, IRON, HI_METAL),

WEAPON("boomerang", (char *)0,
        1, 1, 0, 15,  5, 20,  9,  9, 0, 0,   -P_BOOMERANG, WOOD, HI_WOOD),

/* spears */
WEAPON("spear", (char *)0,
        1, 1, 0, 50, 30,  3,  6,  8, 0, P,   P_SPEAR, IRON, HI_METAL),
WEAPON("elven spear", "runed spear",
        0, 1, 0, 10, 30,  3,  7,  8, 0, P,   P_SPEAR, WOOD, HI_WOOD),
WEAPON("orcish spear", "crude spear",
        0, 1, 0, 13, 30,  3,  5,  8, 0, P,   P_SPEAR, IRON, CLR_BLACK),
WEAPON("dwarvish spear", "stout spear",
        0, 1, 0, 12, 35,  3,  8,  8, 0, P,   P_SPEAR, IRON, HI_METAL),
WEAPON("silver spear", (char *)0,
        1, 1, 0,  2, 36, 40,  6,  8, 0, P,   P_SPEAR, SILVER, HI_SILVER),
WEAPON("javelin", "throwing spear",
        0, 1, 0, 10, 20,  3,  6,  6, 0, P,   P_JAVELIN, IRON, HI_METAL),

WEAPON("trident", (char *)0,
        1, 0, 0,  8, 25,  5,  6,  4, 0, P,   P_TRIDENT, IRON, HI_METAL),
                                                /* +1 small, +2d4 large */


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

/* bows */
BOW("bow", (char *)0,           1, 24, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
BOW("elven bow", "runed bow",   0, 12, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
BOW("orcish bow", "crude bow",  0, 12, 30, 60, 0, WOOD, P_BOW, CLR_BLACK),
BOW("yumi", "long bow",         0,  0, 30, 60, 0, WOOD, P_BOW, HI_WOOD),
BOW("sling", (char *)0,         1, 40,  3, 20, 0, WOOD, P_SLING, HI_WOOD),
BOW("crossbow", (char *)0,      1, 45, 50, 40, 0, WOOD, P_CROSSBOW, HI_WOOD),

/* helmets */
HELM("elven leather helm", "leather hat",
                0, 0,  0,       6, 1,  3,   8, 9, 0, LEATHER, HI_LEATHER),
HELM("orcish helm", "iron skull cap",
                0, 0,  0,       6, 1, 30,  10, 9, 0, IRON, CLR_BLACK),
HELM("dwarvish iron helm", "hard hat",
                0, 0,  0,       6, 1, 40,  20, 8, 0, IRON, HI_METAL),
HELM("fedora", (char *)0,
                1, 0,  0,       0, 0,  3,   1,10, 0, CLOTH, CLR_BROWN),
HELM("cornuthaum", "conical hat",
                0, 1,  CLAIRVOYANT,
                                3, 1,  4,  80,10, 2, CLOTH, CLR_BLUE),
HELM("dunce cap", "conical hat",
                0, 1,  0,       3, 1,  4,   1,10, 0, CLOTH, CLR_BLUE),
HELM("dented pot", (char *)0,
                1, 0,  0,       2, 0, 10,   8, 9, 0, IRON, CLR_BLACK),
/* With shuffled appearances... */
HELM("helmet", "plumed helmet",
                0, 0,  0,      10, 1, 30,  10, 9, 0, IRON, HI_METAL),
HELM("helm of brilliance", "etched helmet",
                0, 1,  0,       6, 1, 50,  50, 9, 0, IRON, CLR_GREEN),
HELM("helm of opposite alignment", "crested helmet",
                0, 1,  0,       6, 1, 50,  50, 9, 0, IRON, HI_METAL),
HELM("helm of telepathy", "visored helmet",
                0, 1,  TELEPAT, 2, 1, 50,  50, 9, 0, IRON, HI_METAL),
/* suits of armor */
/*
 * There is code in polyself.c that assumes (1) and (2).
 * There is code in objnam.c, mon.c, read.c that assumes (2).
 *
 *      (1) The dragon scale mails and the dragon scales are together.
 *      (2) That the order of the dragon scale mail and dragon scales is the
 *          the same defined in monst.c.
 */
DRGN_ARMR("gray dragon scale mail",   ANTIMAGIC,  1200, 1, CLR_GRAY),
DRGN_ARMR("silver dragon scale mail", REFLECTING, 1200, 1, SILVER),
#if 0   /* DEFERRED */
DRGN_ARMR("shimmering dragon scale mail", DISPLACED, 1200, 1, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scale mail",    FIRE_RES,    900, 1, CLR_RED),
DRGN_ARMR("white dragon scale mail",  COLD_RES,    900, 1, CLR_WHITE),
DRGN_ARMR("orange dragon scale mail", SLEEP_RES,   900, 1, CLR_ORANGE),
DRGN_ARMR("black dragon scale mail",  DISINT_RES, 1200, 1, CLR_BLACK),
DRGN_ARMR("blue dragon scale mail",   SHOCK_RES,   900, 1, CLR_BLUE),
DRGN_ARMR("green dragon scale mail",  POISON_RES,  900, 1, CLR_GREEN),
DRGN_ARMR("yellow dragon scale mail", ACID_RES,    900, 1, CLR_YELLOW),

/* For now, only dragons leave these. */
DRGN_ARMR("gray dragon scales",   ANTIMAGIC,  700, 7, CLR_GRAY),
DRGN_ARMR("silver dragon scales", REFLECTING, 700, 7, SILVER),
#if 0   /* DEFERRED */
DRGN_ARMR("shimmering dragon scales", DISPLACED,  700, 7, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scales",    FIRE_RES,   500, 7, CLR_RED),
DRGN_ARMR("white dragon scales",  COLD_RES,   500, 7, CLR_WHITE),
DRGN_ARMR("orange dragon scales", SLEEP_RES,  500, 7, CLR_ORANGE),
DRGN_ARMR("black dragon scales",  DISINT_RES, 700, 7, CLR_BLACK),
DRGN_ARMR("blue dragon scales",   SHOCK_RES,  500, 7, CLR_BLUE),
DRGN_ARMR("green dragon scales",  POISON_RES, 500, 7, CLR_GREEN),
DRGN_ARMR("yellow dragon scales", ACID_RES,   500, 7, CLR_YELLOW),

ARMOR("plate mail", (char *)0,
        1, 0, 1, 0,     44, 5, 450, 600,  3, 2, ARM_SUIT, IRON, HI_METAL),
ARMOR("crystal plate mail", (char *)0,
        1, 0, 1, 0,     10, 5, 450, 820,  3, 2, ARM_SUIT, GLASS, CLR_WHITE),
ARMOR("bronze plate mail", (char *)0,
        1, 0, 1, 0,     25, 5, 450, 400,  4, 0, ARM_SUIT, COPPER, HI_COPPER),
ARMOR("splint mail", (char *)0,
        1, 0, 1, 0,     62, 5, 400,  80,  4, 1, ARM_SUIT, IRON, HI_METAL),
ARMOR("banded mail", (char *)0,
        1, 0, 1, 0,     72, 5, 350,  90,  4, 0, ARM_SUIT, IRON, HI_METAL),
ARMOR("dwarvish mithril-coat", (char *)0,
        1, 0, 0, 0,     10, 1, 150, 240,  4, 3, ARM_SUIT, MITHRIL, HI_METAL),
ARMOR("elven mithril-coat", (char *)0,
        1, 0, 0, 0,     15, 1, 150, 240,  5, 3, ARM_SUIT, MITHRIL, HI_METAL),
ARMOR("chain mail", (char *)0,
        1, 0, 0, 0,     72, 5, 300,  75,  5, 1, ARM_SUIT, IRON, HI_METAL),
ARMOR("orcish chain mail", "crude chain mail",
        0, 0, 0, 0,     20, 5, 300,  75,  6, 1, ARM_SUIT, IRON, CLR_BLACK),
ARMOR("scale mail", (char *)0,
        1, 0, 0, 0,     72, 5, 250,  45,  6, 0, ARM_SUIT, IRON, HI_METAL),
ARMOR("studded leather armor", (char *)0,
        1, 0, 0, 0,     72, 3, 200,  15,  7, 1, ARM_SUIT, LEATHER, HI_LEATHER),
ARMOR("ring mail", (char *)0,
        1, 0, 0, 0,     72, 5, 250, 100,  7, 0, ARM_SUIT, IRON, HI_METAL),
ARMOR("orcish ring mail", "crude ring mail",
        0, 0, 0, 0,     20, 5, 250,  80,  8, 1, ARM_SUIT, IRON, CLR_BLACK),
ARMOR("leather armor", (char *)0,
        1, 0, 0, 0,     82, 3, 150,   5,  8, 0, ARM_SUIT, LEATHER, HI_LEATHER),
ARMOR("leather jacket", (char *)0,
        1, 0, 0, 0,     12, 0,  30,  10,  9, 0, ARM_SUIT, LEATHER, CLR_BLACK),
/* shirts */
ARMOR("Hawaiian shirt", (char *)0,
        1, 0, 0, 0,      8, 0,   5,   3, 10, 0, ARM_SHIRT, CLOTH, CLR_MAGENTA),
ARMOR("T-shirt", (char *)0,
        1, 0, 0, 0,      2, 0,   5,   2, 10, 0, ARM_SHIRT, CLOTH, CLR_WHITE),
/* cloaks */
/*  'cope' is not a spelling mistake... leave it be */
CLOAK("mummy wrapping", (char *)0,
                1, 0,   0,          0, 0,  3,  2, 10, 1, CLOTH, CLR_GRAY),
CLOAK("elven cloak", "faded pall",
                0, 1,   STEALTH,   10, 0, 10, 60,  9, 3, CLOTH, CLR_BLACK),
CLOAK("orcish cloak", "coarse mantelet",
                0, 0,   0,         10, 0, 10, 40, 10, 2, CLOTH, CLR_BLACK),
CLOAK("dwarvish cloak", "hooded cloak",
                0, 0,   0,         10, 0, 10, 50, 10, 2, CLOTH, HI_CLOTH),
CLOAK("oilskin cloak", "slippery cloak",
                0, 0,   0,         10, 0, 10, 50,  9, 3, CLOTH, HI_CLOTH),
CLOAK("robe", (char *)0,
                1, 1,   0,          0, 0, 15, 50,  8, 3, CLOTH, CLR_RED),
CLOAK("alchemy smock", "apron",
                0, 1,   POISON_RES, 9, 0, 10, 50,  9, 1, CLOTH, CLR_WHITE),
/* With shuffled appearances... */
CLOAK("cloak of protection", "tattered cape",
                0, 1,   PROTECTION,10, 0, 10, 50,  7, 3, CLOTH, HI_CLOTH),
CLOAK("cloak of invisibility", "opera cloak",
                0, 1,   INVIS,     11, 0, 10, 60,  9, 2, CLOTH, CLR_BRIGHT_MAGENTA),
CLOAK("cloak of magic resistance", "ornamental cope",
                0, 1,   ANTIMAGIC,  2, 0, 10, 60,  9, 3, CLOTH, CLR_WHITE),
CLOAK("cloak of displacement", "piece of cloth",
                0, 1,   DISPLACED, 11, 0, 10, 50,  9, 2, CLOTH, HI_CLOTH),

/* shields */
SHIELD("small shield", (char *)0,
                1, 0, 0, 0,          6, 0, 30,  3,  9, 0, WOOD, HI_WOOD),
SHIELD("elven shield", "blue and green shield",
                0, 0, 0, 0,          2, 0, 50,  7,  8, 0, IRON, CLR_GREEN),
SHIELD("Uruk-hai shield", "white-handed shield",
                0, 0, 0, 0,          2, 0, 50,  7,  9, 0, IRON, HI_METAL),
SHIELD("orcish shield", "red-eyed shield",
                0, 0, 0, 0,          2, 0, 50,  7,  9, 0, IRON, CLR_RED),
SHIELD("large shield", (char *)0,
                1, 0, 1, 0,          7, 0,100, 10,  8, 0, IRON, HI_METAL),
SHIELD("dwarvish roundshield", "large round shield",
                0, 0, 0, 0,          4, 0,100, 10,  8, 0, IRON, HI_METAL),
SHIELD("shield of reflection", "polished silver shield",
                0, 1, 0, REFLECTING, 3, 0, 50, 50,  8, 0, SILVER, HI_SILVER),

/* gloves */
/* these have their color but not material shuffled, so the IRON must stay
 * CLR_BROWN (== HI_LEATHER)
 */
GLOVES("leather gloves", "old gloves",
                0, 0,  0,         16, 1, 10,  8,  9, 0, LEATHER, HI_LEATHER),
GLOVES("gauntlets of fumbling", "padded gloves",
                0, 1,  FUMBLING,   8, 1, 10, 50,  9, 0, LEATHER, HI_LEATHER),
GLOVES("gauntlets of power", "riding gloves",
                0, 1,  0,          8, 1, 30, 50,  9, 0, IRON, CLR_BROWN),
GLOVES("gauntlets of dexterity", "fencing gloves",
                0, 1,  0,          8, 1, 10, 50,  9, 0, LEATHER, HI_LEATHER),

/* boots */
BOOTS("low boots", "walking shoes",
                0, 0,  0,         25, 2, 10,  8,  9, 0, LEATHER, HI_LEATHER),
BOOTS("iron shoes", "hard shoes",
                0, 0,  0,          7, 2, 50, 16,  8, 0, IRON, HI_METAL),
BOOTS("high boots", "jackboots",
                0, 0,  0,         15, 2, 20, 12,  8, 0, LEATHER, HI_LEATHER),
/* With shuffled appearances... */
BOOTS("speed boots", "combat boots",
                0, 1,  FAST,      12, 2, 20, 50,  9, 0, LEATHER, HI_LEATHER),
BOOTS("water walking boots", "jungle boots",
                0, 1,  WWALKING,  12, 2, 20, 50,  9, 0, LEATHER, HI_LEATHER),
BOOTS("jumping boots", "hiking boots",
                0, 1,  JUMPING,   12, 2, 20, 50,  9, 0, LEATHER, HI_LEATHER),
BOOTS("elven boots", "mud boots",
                0, 1,  STEALTH,   12, 2, 15,  8,  9, 0, LEATHER, HI_LEATHER),
BOOTS("kicking boots", "buckled boots",
                0, 1,  0,         12, 2, 15,  8,  9, 0, IRON, CLR_BROWN),
BOOTS("fumble boots", "riding boots",
                0, 1,  FUMBLING,  12, 2, 20, 30,  9, 0, LEATHER, HI_LEATHER),
BOOTS("levitation boots", "snow boots",
                0, 1,  LEVITATION,12, 2, 15, 30,  9, 0, LEATHER, HI_LEATHER),


};


int GetDefinitionCount()
{
	return ( sizeof(g_objectPrototype) / sizeof(objPrototype) );
}
