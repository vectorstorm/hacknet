/*  OBJ_Convert.h
     Contains necessary #defines to convert from NetHack-style
     object property enums to HackNet-style object property enums  */

#ifndef OBJ_CONVERT_H
#define OBJ_CONVERT_H

#include "OBJ_Types.h"
#include "OBJ_Definition.h"
#include "HN_Properties.h"

/* Skills */

#define P               DIR_Pierce
#define S               DIR_Slash
#define B               DIR_Whack
#define PIERCE		DIR_Pierce
#define SLASH		DIR_Slash
#define WHACK		DIR_Whack

#define RAY		DIR_Ray
#define IMMEDIATE	DIR_Immediate
#define NODIR		DIR_None

#define P_NONE			SKILL_None
#define P_DAGGER                SKILL_Dagger
#define P_KNIFE                 SKILL_Knife
#define P_AXE                   SKILL_Axe
#define P_SHORT_SWORD           SKILL_ShortSword
#define P_SCIMITAR              SKILL_Scimitar
#define P_SABER                 SKILL_Saber
#define P_BROAD_SWORD           SKILL_BroadSword
#define P_LONG_SWORD            SKILL_LongSword
#define P_TWO_HANDED_SWORD      SKILL_TwoHandedSword
#define P_POLEARMS              SKILL_Polearms
#define P_LANCE                 SKILL_Lance
#define P_PICK_AXE              SKILL_PickAxe
#define P_MACE                  SKILL_Mace
#define P_MORNING_STAR          SKILL_MorningStar
#define P_HAMMER                SKILL_Hammer
#define P_CLUB                  SKILL_Club
#define P_WHIP                  SKILL_Whip
#define P_QUARTERSTAFF          SKILL_Quarterstaff
#define P_FLAIL                 SKILL_Flail
#define P_UNICORN_HORN		SKILL_UnicornHorn

#define P_DART			SKILL_Dart
#define P_SHURIKEN		SKILL_Shuriken
#define P_BOOMERANG		SKILL_Boomerang
#define P_SPEAR			SKILL_Spear
#define P_JAVELIN		SKILL_Javelin
#define P_TRIDENT		SKILL_Trident

#define P_BOW			SKILL_Bow
#define P_SLING			SKILL_Sling
#define P_CROSSBOW		SKILL_Crossbow

#define P_ATTACK_SPELL		SKILL_AttackSpell
#define P_ENCHANTMENT_SPELL	SKILL_EnchantmentSpell
#define P_MATTER_SPELL		SKILL_MatterSpell
#define P_CLERIC_SPELL		SKILL_ClericSpell
#define P_ESCAPE_SPELL		SKILL_EscapeSpell
#define P_HEALING_SPELL		SKILL_HealingSpell
#define P_DIVINATION_SPELL	SKILL_DivinationSpell


/*  Armour Types  */

#define ARM_SUIT		ARM_Suit
#define ARM_SHIRT		ARM_Shirt

/* Materials */

#define IRON            MAT_Iron
#define METAL           MAT_Metal
#define WOOD            MAT_Wood
#define MINERAL         MAT_Mineral
#define SILVER          MAT_Silver
#define LEATHER         MAT_Leather
#define CLOTH		MAT_Cloth
#define DRAGON_HIDE	MAT_Dragon_Hide
#define MITHRIL		MAT_Mithril
#define GLASS		MAT_Glass
#define COPPER		MAT_Copper
#define FLESH		MAT_Flesh
#define GOLD		MAT_Gold
#define BONE		MAT_Bone
#define PLASTIC		MAT_Plastic
#define GEMSTONE	MAT_Gemstone
#define WAX		MAT_Wax
#define PLATINUM	MAT_Platinum
#define LIQUID		MAT_Liquid
#define PAPER		MAT_Paper
#define VEGGY		MAT_Vegetable

#define HI_METAL        MAT_Metal
#define HI_MINERAL	MAT_Mineral
#define HI_WOOD         MAT_Wood
#define HI_SILVER       MAT_Silver
#define HI_COPPER	MAT_Copper
#define HI_CLOTH	MAT_Cloth
#define HI_LEATHER	MAT_Leather
#define HI_GOLD		MAT_Gold
#define HI_GLASS	MAT_Glass
#define HI_ORGANIC	MAT_Organic
#define HI_PAPER	MAT_Paper

/*  Colors */

#define CLR_BLACK       	COLOR_Black
#define CLR_WHITE       	COLOR_White
#define CLR_BROWN       	COLOR_Brown
#define CLR_GREEN		COLOR_Green
#define CLR_RED			COLOR_Red
#define CLR_BLUE		COLOR_Blue
#define CLR_MAGENTA		COLOR_Magenta
#define CLR_BRIGHT_MAGENTA	COLOR_Magenta
#define CLR_GRAY		COLOR_Gray
#define CLR_YELLOW		COLOR_Yellow
#define CLR_ORANGE		COLOR_Orange
#define CLR_CYAN		COLOR_Cyan
#define CLR_BRIGHT_GREEN	COLOR_BrightGreen
#define CLR_BRIGHT_CYAN		COLOR_BrightCyan
#define CLR_BRIGHT_BLUE		COLOR_BrightBlue

/* Properties */

#define SHOCK_RES		PROP_Shock_Resistance
#define SLEEP_RES		PROP_Sleep_Resistance
#define DISINT_RES		PROP_Disintegration_Resistance
#define POISON_RES		PROP_Poison_Resistance
#define ACID_RES		PROP_Acid_Resistance
#define FIRE_RES		PROP_Fire_Resistance
#define COLD_RES		PROP_Cold_Resistance
#define STEALTH			PROP_Stealth
#define PROTECTION		PROP_Protection
#define INVIS			PROP_Invisible
#define ANTIMAGIC		PROP_Antimagic
#define DISPLACED		PROP_Displaced
#define REFLECTING		PROP_Reflecting
#define FUMBLING		PROP_Fumbling
#define FAST			PROP_Fast
#define WWALKING		PROP_Water_Walking
#define JUMPING			PROP_Jumping
#define LEVITATION		PROP_Levitation
#define CLAIRVOYANT		PROP_Clairvoyant
#define TELEPAT			PROP_Telepathy
#define UNCHANGING		PROP_Unchanging
#define MAGICAL_BREATHING	PROP_Magical_Breathing
#define ADORNED			PROP_Adorned
#define REGENERATION		PROP_Regeneration
#define SEARCHING		PROP_Searching
#define FIXED_ABIL		PROP_Fixed_Abilities
#define HUNGER			PROP_Hunger
#define CONFLICT		PROP_Conflict
#define WARNING			PROP_Warning
#define FREE_ACTION		PROP_Free_Action
#define SLOW_DIGESTION		PROP_Slow_Digestion
#define TELEPORT		PROP_Teleport
#define TELEPORT_CONTROL	PROP_Teleport_Control
#define POLYMORPH		PROP_Polymorph
#define POLYMORPH_CONTROL	PROP_Polymorph_Control
#define AGGRAVATE_MONSTER	PROP_Aggravate_Monster
#define PROT_FROM_SHAPE_CHANGERS	PROP_Protection_From_Shape_Changers
#define LIFESAVED		PROP_Life_Saved
#define STRANGLED		PROP_Strangled
#define SLEEPING		PROP_Sleeping
#define SEE_INVIS		PROP_See_Invisible
#define CONFUSION		PROP_Confusion
#define BLINDED			PROP_Blinded
#define HALLUC			PROP_Hallucination
#define GAIN_ABIL		PROP_Gain_Ability
#define REST_ABIL		PROP_Restore_Ability
#define PARALYSE		PROP_Paralyse
#define HEAL			PROP_Heal
#define EXTRA_HEAL		PROP_Extra_Heal
#define FULL_HEAL		PROP_Full_Heal
#define GAIN_LEVEL		PROP_Gain_Level
#define ENLIGHTEN		PROP_Enlightenment
#define MONST_DET		PROP_Detect_Monsters
#define OBJ_DET			PROP_Detect_Objects
#define GAIN_ENERGY		PROP_Gain_Energy
#define SLEEP			PROP_Sleep
#define BOOZE			PROP_Booze
#define SICKNESS		PROP_Sickness
#define JUICE			PROP_Juice
#define ACID			PROP_Acid
#define OIL			PROP_Oil
#define WATER			PROP_Water

#define RANDOM_CLASS		OBJ_TYPE_Random
#define ILLOBJ_CLASS		OBJ_TYPE_Illegal
#define WEAPON_CLASS		OBJ_TYPE_Weapon
#define RING_CLASS		OBJ_TYPE_Ring
#define AMULET_CLASS		OBJ_TYPE_Amulet
#define TOOL_CLASS		OBJ_TYPE_Tool
#define FOOD_CLASS		OBJ_TYPE_Food
#define POTION_CLASS		OBJ_TYPE_Potion
#define SCROLL_CLASS		OBJ_TYPE_Scroll
#define SPBOOK_CLASS		OBJ_TYPE_Spellbook
#define WAND_CLASS		OBJ_TYPE_Wand
#define GOLD_CLASS		OBJ_TYPE_Gold
#define GEM_CLASS		OBJ_TYPE_Gem
#define ROCK_CLASS		OBJ_TYPE_Rock
#define BALL_CLASS		OBJ_TYPE_Ball
#define CHAIN_CLASS		OBJ_TYPE_Chain
#define VENOM_CLASS		OBJ_TYPE_Venom


#endif  // OBJ_CONVERT_H
