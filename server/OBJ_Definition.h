#ifndef __OBJ_DEFINITION_H__
#define __OBJ_DEFINITION_H__

#include "HN_Types.h"
#include "OBJ_Types.h"

int GetDefinitionCount();

typedef uint8 directType;
enum {
                //  For wands & spells
        DIR_None = 1,           // non-directional spell
        DIR_Immediate = 2,       // directional spell
        DIR_Ray = 3,             // zap beams

                // for anthing else used as a weapon
        DIR_Pierce = 1,
        DIR_Slash = 2,
        DIR_Whack = 0
};

typedef uint8 materialType;
enum {
        MAT_Liquid,
        MAT_Wax,
        MAT_Vegetable,          /* food */
        MAT_Flesh,              /* food */
        MAT_Paper,
        MAT_Cloth,
        MAT_Leather,
        MAT_Wood,
        MAT_Bone,
        MAT_Dragon_Hide,        /* not leather! */
        MAT_Iron,               /* includes steel */
        MAT_Metal,
        MAT_Copper,             /* includes brass */
        MAT_Silver,
        MAT_Gold,
        MAT_Platinum,
        MAT_Mithril,
        MAT_Plastic,
        MAT_Glass,
        MAT_Gemstone,
        MAT_Mineral
};

typedef uint8 armCategory;
enum
{
	ARM_Shield,
	ARM_Helm,
	ARM_Gloves,
	ARM_Boots,
	ARM_Cloak,
	ARM_Shirt,
	ARM_Suit
};

struct objPrototype
{
	char *		name;
	char *		desc;
	
        unsigned int 	stackable:1;    	// true if this object will stack with similar objects
        unsigned int 	prediscovered:1;	// if true, everyone starts out knowing this

        unsigned int 	magic:1;		// inherantly magical
        unsigned int 	charged:1;		// may have +n or (n) charges
        unsigned int 	unique:1;		// one-of-a-kind
        unsigned int 	nowish:1;		// can't be wished for
        unsigned int 	big:1;			// if it's physically large

        unsigned int 	direct:2;

	uint8		skill;			// what skill do we exercise/use?
	uint8		armcat;			// what sort of armor are we?
	
	uint8		material;		// lots of bits.

	uint16		type;			// object type (armour, weapon, etc.)
	uint8		delay;			// time taken using object

	uint16		probability;		// likelihood of generating.. higher numbers make it more likely to be selected.
	uint16		weight;			// encumbrance (1 cn = 0.1 lb)
	uint16		cost;			// base cost in shops

	sint8		smallDamage;		// maximum vs-small-monster damage
	sint8		largeDamage;		// maximum vs-large-monster damage

	
};

enum skillType
{
	SKILL_Dagger,
	SKILL_Knife,
	SKILL_Axe,
	SKILL_PickAxe,
	SKILL_ShortSword,
	SKILL_BroadSword,
	SKILL_LongSword,
	SKILL_TwoHandedSword,
	SKILL_Scimitar,
	SKILL_Saber,
	SKILL_Club,
	SKILL_Mace,
	SKILL_MorningStar,
	SKILL_Flail,
	SKILL_Hammer,
	SKILL_Quarterstaff,
	SKILL_Polearms,
	SKILL_Spear,
	SKILL_Javelin,
	SKILL_Trident,
	SKILL_Lance,
	SKILL_Bow,
	SKILL_Sling,
	SKILL_Crossbow,
	SKILL_Dart,
	SKILL_Shuriken,
	SKILL_Boomerang,
	SKILL_Whip,
	SKILL_UnicornHorn,

	SKILL_FirstWeapon = SKILL_Dagger,
	SKILL_LastWeapon = SKILL_UnicornHorn,

	SKILL_AttackSpell,
	SKILL_HealingSpell,
	SKILL_DivinationSpell,
	SKILL_EnchantmentSpell,
	SKILL_ClericSpell,
	SKILL_EscapeSpell,
	SKILL_MatterSpell,
	
	SKILL_FirstSpell = SKILL_AttackSpell,
	SKILL_LastSpell = SKILL_MatterSpell,

	SKILL_BareHandedCombat,
	SKILL_MartialArts,
	SKILL_TwoWeaponCombat,
	SKILL_Riding,
	
	SKILL_FirstHandToHand = SKILL_BareHandedCombat,
	SKILL_LastHandToHand = SKILL_Riding,
};

#endif