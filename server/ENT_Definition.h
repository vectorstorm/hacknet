#ifndef __ENT_DEFINITION_H__
#define __ENT_DEFINITION_H__

#include "HN_Types.h"
#include "HN_Color.h"

int GetEntityDefinitionCount();

typedef sint8 AlignType;
enum {
	Align_None,
	Align_Chaotic,
	Align_Neutral,
	Align_Lawful
};

typedef uint16 GenocideFlags;
enum {
	Geno_Unique 		= 0x1000,	// generated once only
	Geno_NoHell		= 0x0800,	// not generated in hell
	Geno_Hell 		= 0x0400,	// only generated in hell
	Geno_NoGenerate 	= 0x0200,	// not generated automatically
	Geno_SmallGroup 	= 0x0080,	// appear in small groups
	Geno_LargeGroup 	= 0x0040,	// appear in large groups
	Geno_Genocidable 	= 0x0020,	// can be genocidable
	Geno_NoCorpse 		= 0x0010,	// no corpse left behind
	Geno_Frequency 		= 0x0007,	// creation frequency mask

	Geno_Known 		= 0x0004,	// have been encountered
	Geno_Genocided 		= 0x0002,	// has been genocided
	Geno_Extinct		= 0x0001,	// under population control
	Geno_Gone 		= (Geno_Genocided|Geno_Extinct)
	
};

typedef uint8 MagicResistance;
enum {
	Resist_Fire		= 0x01,
	Resist_Cold		= 0x02,
	Resist_Sleep		= 0x04,
	Resist_Disintegrate	= 0x08,
	Resist_Electricity	= 0x10,
	Resist_Poison		= 0x20,
	Resist_Acid		= 0x40,
	Resist_Stone		= 0x80
};

enum AttackType
{
	Attack_None,
	Attack_Claw,
	Attack_Bite,
	Attack_Kick,
	Attack_Butt,
	Attack_Touch,
	Attack_Sting,
	Attack_Hugs,		// crushing bear hug
	Attack_Spit,
	Attack_Engulf,
	Attack_Breath,
	Attack_Explodes,	// explodes when close
	Attack_Boom,		// explodes when killed
	Attack_Gaze,		// ranged gaze attack
	Attack_Tentacles,
	
	Attack_Weapon,		// uses weapons
	Attack_Magic		// uses magic
};

enum DamageType
{
	Damage_Physical,
	Damage_MagicMissile,
	Damage_Fire,
	Damage_Cold,
	Damage_Sleep,
	Damage_Disintegration,
	Damage_Electric,
	Damage_DrainStrength,
	Damage_Acid,
	Damage_Special1,		// for extension of buzz() (?)
	Damage_Special2,
	Damage_Blind,
	Damage_Stun,
	Damage_Slow,
	Damage_Paralyse,
	Damage_DrainLife,
	Damage_DrainEnergy,
	Damage_Legs,
	Damage_Stone,
	Damage_Stick,
	Damage_StealGold,
	Damage_StealItem,
	Damage_Seduce,
	Damage_Teleport,		// teleports target
	Damage_Rust,
	Damage_Confuse,
	Damage_Digest,
	Damage_Heal,
	Damage_Wrap,			// special 'stick' for eels
	Damage_Lycanthropy,
	Damage_DrainDexterity,
	Damage_DrainConstitution,
	Damage_DrainIntelligence,
	Damage_Disease,
	Damage_Decay,
	Damage_SuccubusSeduction,
	Damage_Hallucination,
	Damage_Death,
	Damage_Pestilence,
	Damage_Famine,
	Damage_Slime,
	Damage_RemoveEnchantment,
	Damage_Corrode,
	Damage_RandomCleric,		// random clerical spell
	Damage_RandomSpell,		// random spell
	Damage_RandomBreath,		// random breath weapon
	Damage_Samurai,			// hits and may steal amulet
	Damage_Curse			// random curse
};



typedef uint8 MonsterSound;
enum
{
	Sound_Silent,
	Sound_Bark,			// may howl on full moon
	Sound_Mew,
	Sound_Roar,
	Sound_Growl,
	Sound_Squeak,
	Sound_Squawk,
	Sound_Hiss,
	Sound_Buzz,
	Sound_Grunt,
	Sound_Neigh,
	Sound_Wail,
	Sound_Gurgle,
	Sound_Burble,
	// up to here are 'animal' noises
	Sound_Animal = Sound_Burble,
	
	Sound_Shriek,
	Sound_Bones,
	Sound_Laugh,
	Sound_Mumble,
	Sound_Imitate,
	Sound_Orc,
	Sound_Humanoid,
	Sound_Arrest,
	Sound_Soldier,
	Sound_Guard,
	Sound_Djinni,
	Sound_Nurse,
	Sound_Seduce,
	Sound_Vampire,
	Sound_Bribe,
	Sound_Cuss,
	Sound_Rider,
	Sound_Leader,
	Sound_Nemesis,
	Sound_Guardian,
	Sound_Sell,
	Sound_Oracle,
	Sound_Priest,
	Sound_Spell,
	Sound_Lycanthrope,
	Sound_Boast,
};

enum{
	Size_Tiny,
	Size_Small,
	Size_Medium,
	Size_Human = Size_Medium,
	Size_Large,
	Size_Huge,
	Size_Gigantic
};

enum{
	M1_Fly 		= 0x00000001,
	M1_Swim 	= 0x00000002,
	M1_Amorphous	= 0x00000004,
	M1_WallWalk	= 0x00000008,
	M1_Cling	= 0x00000010,
	M1_Tunnel	= 0x00000020,
	M1_NeedPick	= 0x00000040,		// needs pick to tunnel
	M1_Conceal	= 0x00000080,		// hides under stuff
	M1_Hide		= 0x00000100,		// mimics or blends with ceiling
	M1_Amphibious	= 0x00000200,		// can survive underwater
	M1_Breathless	= 0x00000400,		// doesn't need to breath
	M1_NoTake	= 0x00000800,		// can't pick stuff up
	M1_NoEyes	= 0x00001000,		// no eyes to gaze into and blind
	M1_NoHands	= 0x00002000,		// no hands to handle things
	M1_NoLimbs	= 0x00004000,		// no arms/legs to kick/wear with.
	M1_NoHead	= 0x00008000,		// no head to behead
	M1_Mindless	= 0x00010000,		// no mind
	M1_Humanoid	= 0x00020000,		// has humanoid head/arms/torso
	M1_Animal	= 0x00040000,		// has animal body
	M1_Slithery	= 0x00080000,		// has serpent body
	M1_Unsolid	= 0x00100000,		// no solid or liquid body
	M1_ThickHide	= 0x00200000,		// thick hide/scales
	M1_Oviparous	= 0x00400000,		// can lay eggs
	M1_Regenerate	= 0x00800000,		// regenerates hit points
	M1_SeeInvisible	= 0x01000000,		// can see invisible creatures
	M1_Teleport	= 0x02000000,		// can teleport
	M1_TeleportControl	= 0x04000000,		// controls where teleport to
	M1_Acid		= 0x08000000,		// acidic to eat
	M1_Poison	= 0x10000000,		// poisonous to eat
	M1_Carnivore	= 0x20000000,		// eats corpses
	M1_Herbivore	= 0x40000000,		// eats fruits
	M1_Omnivore	= 0x60000000,		// eats both
	M1_Metallivore	= 0x80000000,		// eats metal
};


enum{
	M2_NoPolymorph	= 0x00000001,
	M2_Undead	= 0x00000002,
	M2_Lycanthrope	= 0x00000004,
	M2_Human	= 0x00000008,
	M2_Elf		= 0x00000010,
	M2_Dwarf	= 0x00000020,
	M2_Gnome	= 0x00000040,		
	M2_Orc		= 0x00000080,		
	M2_Demon	= 0x00000100,	
	M2_Mercenary	= 0x00000200,		// soldier or guard
	M2_Lord		= 0x00000400,		// lord of its kind
	M2_Prince	= 0x00000800,		// overlord of its kind
	M2_Minion	= 0x00001000,		// minion of a deity
	M2_Giant	= 0x00002000,		
	M2_Male		= 0x00010000,		// always male
	M2_Female	= 0x00020000,		// always female
	M2_Neuter	= 0x00040000,		// neither male nor female
	M2_ProperName	= 0x00080000,		// name is a proper name
	M2_Hostile	= 0x00100000,		// always starts hostile
	M2_Peaceful	= 0x00200000,		// always starts peaceful
	M2_Domestic	= 0x00400000,		// can be tamed by feeding
	M2_Wander	= 0x00800000,		// wanders randomly
	M2_Stalk	= 0x01000000,		// follows you to other levels
	M2_Nasty	= 0x02000000,		// extra-nasty monster
	M2_Strong	= 0x04000000,		// strong (or big) monster
	M2_RockThrow	= 0x08000000,		// throws boulders
	M2_Greedy	= 0x10000000,		// likes gold
	M2_Jewels	= 0x20000000,		// likes jewels
	M2_Collect	= 0x40000000,		// picks up weapons and food
	M2_Magic	= 0x80000000,		// picks up magic items
};

enum{
	M3_WantsAmulet  = 0x00000001,		// would like to steal the amulet
	M3_WantsBell	= 0x00000002,		// wants the bell
	M3_WantsBook	= 0x00000004,		// wants the book
	M3_WantsCandelabrum = 0x00000008,	// wants the candelabrum
	M3_WantsArtifact =0x00000010,		// wants quest artifacts
	M3_WantsAll	= 0x0000001f,		// wants any major artifact
	M3_WaitForYou	= 0x00000040,		// waits to see you or get attacked
	M3_Close	= 0x00000080,		// lets you close unless attacked
	M3_Covetous	= 0x0000001f,		// wants something
	M3_WaitMask	= 0x000000c0,		// waiting...
	M3_Infravision	= 0x00000100,		// has infravision
	M3_Infravisable = 0x00000200		// visible by infravision
};

enum{
	Color_Black,
	Color_Red,
	Color_Green,
	Color_Brown,
	Color_Blue,
	Color_Magenta,
	Color_Cyan,
	Color_Gray,
	Color_None,
	Color_Orange,
	Color_BrightGreen,
	Color_Yellow,
	Color_BrightBlue,
	Color_BrightMagenta,
	Color_BrightCyan,
	Color_White
};

#define MAX_ATTACKS (6)

struct attack {
	uint8		attackType;	// gross attack type (claw, bite, etc.)
	uint8		damageType;	// damage type (physical, fire, cold, etc.)
	uint8		damageN;	// how many dice of damage
	uint8		damageD;	// how many sides on those dice
};

struct entPrototype
{
	const char *	name;

	uint8		type;			// type, as determined by ENT_Types.h
	sint8		level;			// default monster level
	sint8		speed;			// movement speed
	sint8		ac;			// (base) armor class
	sint8		mr;			// (base) magic resistance
	AlignType	alignment;		// basic alignment
	GenocideFlags	genocide;		// creation/genocide value
	struct attack	attack[MAX_ATTACKS];	// array of our attacks
	uint16		corpseWeight;		// how heavy is my corpse?
	uint16		corpseNutrition;	// how nutritious is our corpse?
	uint16		length;			// how long are we?  (only used for long worms, I think?)
	uint8		sound;			// what noise we make
	uint8		size;			// physical size (small or large)
	uint8		resists;		// resistance flags
	uint8		corpseIntrinsics;	// intrinsics gained from corpse
	uint32		flags1;			// boolean flags
	uint32		flags2;			// more boolean flags
	uint16		flags3;			// yet more boolean flags
	uint8		color;			// suggested color
};

#endif
