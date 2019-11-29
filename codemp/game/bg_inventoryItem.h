#pragma once

#ifndef _INVENTORY_
#define _INVENTORY_

#include "../game/bg_public.h"
#include "../qcommon/q_shared.h"

#include <string>

extern const char *itemQualityTooltips[];
extern const char *weaponCrystalTooltips[];
extern const char *itemCrystalTooltips[];
extern const char *weaponStat1Tooltips[];
extern const char *weaponStat2Tooltips[];
extern const char *weaponStat3Tooltips[];
extern const char *saberStat1Tooltips[];
extern const char *saberStat2Tooltips[];
extern const char *saberStat3Tooltips[];
extern const char *itemStatTooltips[];

//
// Quality levels of items...
//
typedef enum {
	QUALITY_GREY,		// on weapons/sabers/items - 0 base stats and 0 mod slots.
	QUALITY_WHITE,		// on weapons/sabers/items - 1 base stats and 0 mod slots.
	QUALITY_GREEN,		// on weapons/sabers/items - 2 base stats and 0 mod slots.
	QUALITY_BLUE,		// on weapons/sabers/items - 3 base stats and 1 mod slots.
	QUALITY_PURPLE,		// on weapons/sabers/items - 3 base stats and 2 mod slots.
	QUALITY_ORANGE,		// on weapons/sabers/items - 3 base stats and 3 mod slots.
	QUALITY_GOLD		// on weapons/sabers/items - 3 base stats and 3 mod slots. 1.5x Stats Modifier.
} itemQuality_t;

//
// Stats available to items...
//

// For weapons/sabers/mods/items crystal (power) slot only. Damage types for weapons, and damage resistance types for items.
typedef enum {
	ITEM_CRYSTAL_DEFAULT,			// GREY shots/blade? No special damage/resistance type...
	ITEM_CRYSTAL_RED,				// Bonus Heat Damage/Resistance
	ITEM_CRYSTAL_GREEN,				// Bonus Kinetic (force) Damage/Resistance
	ITEM_CRYSTAL_BLUE,				// Bonus Electric Damage/Resistance
	ITEM_CRYSTAL_WHITE,				// Bonus Cold Damage/Resistance
	// Maybe extras below should be combos of the above? Stois? Thoughts?
	ITEM_CRYSTAL_YELLOW,			// Bonus 1/2 Heat + 1/2 Kinetic Damage/Resistance
	ITEM_CRYSTAL_PURPLE,			// Bonus 1/2 Electric + 1/2 Heat Damage/Resistance
	ITEM_CRYSTAL_ORANGE,			// Bonus 1/2 Cold + 1/2 Kinetic Damage/Resistance
	ITEM_CRYSTAL_PINK,				// Bonus 1/2 Electric + 1/2 Cold Damage/Resistance
	ITEM_CRYSTAL_MAX
} itemPowerCrystal_t;

// For weapons/weapon-mods slot 1 only.
typedef enum {
	WEAPON_STAT1_DEFAULT,						// Pistol
	WEAPON_STAT1_HEAVY_PISTOL,					// Pistol
	WEAPON_STAT1_FIRE_ACCURACY_MODIFIER,		// Sniper Rifle
	WEAPON_STAT1_FIRE_RATE_MODIFIER,			// Blaster Rifle
	WEAPON_STAT1_VELOCITY_MODIFIER,				// Assault Rifle
	WEAPON_STAT1_HEAT_ACCUMULATION_MODIFIER,	// Heavy Blster
	WEAPON_STAT1_MAX
} weaponStat1_t;

// For weapons/weapon-mods slot 2 only.
typedef enum {
	WEAPON_STAT2_DEFAULT,
	WEAPON_STAT2_FIRE_DAMAGE_MODIFIER,
	WEAPON_STAT2_CRITICAL_CHANCE_MODIFIER,
	WEAPON_STAT2_CRITICAL_POWER_MODIFIER,
	WEAPON_STAT2_MAX
} weaponStat2_t;

// For weapons/mods/items slot 3 only.
typedef enum {
	WEAPON_STAT3_SHOT_DEFAULT,
	WEAPON_STAT3_SHOT_BOUNCE,
	WEAPON_STAT3_SHOT_EXPLOSIVE,
	WEAPON_STAT3_SHOT_BEAM,
	WEAPON_STAT3_SHOT_WIDE,
	WEAPON_STAT3_MAX
} weaponStat3_t;

// For sabers/saber-mods slot 1 only.
typedef enum {
	SABER_STAT1_DEFAULT,
	SABER_STAT1_MELEE_BLOCKING,
	SABER_STAT1_RANGED_BLOCKING,
	SABER_STAT1_TIMED_STAGGER_CHANCE,
	SABER_STAT1_MAX
} saberStat1_t;

// For sabers/saber-mods slot 2 only.
typedef enum {
	SABER_STAT2_DEFAULT,
	SABER_STAT2_DAMAGE_MODIFIER,
	SABER_STAT2_CRITICAL_CHANCE_MODIFIER,
	SABER_STAT2_CRITICAL_POWER_MODIFIER,
	SABER_STAT2_SHIELD_PENETRATION_MODIFIER,
	SABER_STAT2_HEALTH_DRAIN,
	SABER_STAT2_FORCE_DRAIN,
	SABER_STAT2_MAX
} saberStat2_t;

// For sabers/saber-mods slot 3 only.
typedef enum {
	SABER_STAT3_DEFAULT,
	SABER_STAT3_LENGTH_MODIFIER,
	SABER_STAT3_SPEED_MODIFIER,
	SABER_STAT3_MAX
} saberStat3_t;

// For any items/item-mods slots only.
typedef enum {
	ITEM_STAT1_DEFAULT,
	ITEM_STAT1_HEALTH_MAX_MODIFIER,
	ITEM_STAT1_HEALTH_REGEN_MODIFIER,
	ITEM_STAT1_SHIELD_MAX_MODIFIER,
	ITEM_STAT1_SHIELD_REGEN_MODIFIER,
	ITEM_STAT1_FORCEPOWER_MAX_MODIFIER,
	ITEM_STAT1_FORCEPOWER_REGEN_MODIFIER,
	ITEM_STAT1_STRENGTH_MODIFIER,
	ITEM_STAT1_EVASION_MODIFIER,
	ITEM_STAT1_SPEED_MODIFIER,
	ITEM_STAT1_AGILITY_MODIFIER, // 1/2 speed + 1/2 evasion
	ITEM_STAT1_BLOCKING_MODIFIER,
	ITEM_STAT1_DAMAGE_REDUCTION_MODIFIER,
	ITEM_STAT1_PENETRATION_REDUCTION_MODIFIER,
	ITEM_STAT1_MAX
} itemStat_t;

//
// A quality based price scale modifier... Used internally... Matches levels of itemQuality_t.
//
extern const float qualityPriceModifier[];

class inventoryItem
{
private:
	// Values...
	uint16_t			m_itemID;				// Tracks unique ID for the item. Generated by the server inventory system. Needs to be < 65536 for transmission...
	uint16_t			m_bgItemID;				// Link to the original base item in bg_itemlist.
#if defined(_CGAME) || defined(rd_warzone_x86_EXPORTS)
	char				m_model[MAX_QPATH];		// Only used on client. Stores and registers the model for this item.
	char				m_icon[MAX_QPATH];		// Only used on client. Stores and registers the icon for this item.
#endif
	itemQuality_t		m_quality;				// Quality level of this item, as defined by itemQuality_t
	uint16_t			m_quantity;				// Quantity of this stack of items.

	// Base item modifier stats... The item itself has 1 crystal (color modifier), and 3 stat modifiers.
	uint16_t			m_crystal;				// itemPowerCrystal_t - Damage types for weapons, and damage resistance types for items. This crystal is replaceable.
	uint16_t			m_basicStat1;			// weapons: weaponStat1_t. sabers: saberStat1_t. items: itemStat_t
	uint16_t			m_basicStat2;			// weapons: weaponStat2_t. sabers: saberStat2_t. items: itemStat_t
	uint16_t			m_basicStat3;			// weapons: weaponStat3_t. sabers: saberStat3_t. items: itemStat_t
	float				m_basicStat1value;		// m_basicStat1 strength multiplier
	float				m_basicStat2value;		// m_basicStat2 strength multiplier
	float				m_basicStat3value;		// m_basicStat3 strength multiplier

	//
	// Private Internal Functions...
	//

public:
	//
	// Construction/Destruction...
	//
	inventoryItem(uint16_t itemID);
	inventoryItem(uint16_t, uint16_t, itemQuality_t, uint16_t); // paramterized constructor
	~inventoryItem(); // destructor

	qboolean			m_transmitted;

	//
	// Item Setup Functions...
	//
	void setItemID(uint16_t itemID);
	void setBaseItem(uint16_t);
	void setQuality(itemQuality_t);
	void setQuantity(uint16_t);

	// Base item modifier stats...
	void setCrystal(uint16_t);
	void setStat1(uint16_t, float);
	void setStat2(uint16_t, float);
	void setStat3(uint16_t, float);

	//
	// Item Accessing Functions...
	//
	uint16_t getItemID();
	gitem_t	*getBaseItem();
	uint16_t getBaseItemID();
	itemQuality_t getQuality();
	const char *getName();
	char *getDescription();
	uint16_t getQuantity();
	float getCost(uint16_t modItemID1, uint16_t modItemID2, uint16_t modItemID3);
	float getStackCost(uint16_t modItemID1, uint16_t modItemID2, uint16_t modItemID3);
	char *getModel();
	char *getIcon();
	float getCrystalPower(void);

	qboolean isModification();
	qboolean isCrystal();

	// Base item modifier stats...
	uint16_t getCrystal();
	uint16_t getBasicStat1();
	uint16_t getBasicStat2();
	uint16_t getBasicStat3();
	float getBasicStat1Value();
	float getBasicStat2Value();
	float getBasicStat3Value();

	// Installed module modifier stats... modItemID is needed from ps->inventoryMod#[#]
	uint16_t getMod1Stat(uint16_t modItemID);
	uint16_t getMod2Stat(uint16_t modItemID);
	uint16_t getMod3Stat(uint16_t modItemID);
	float getMod1Value(uint16_t modItemID);
	float getMod2Value(uint16_t modItemID);
	float getMod3Value(uint16_t modItemID);

	// Visual looks for each stat (which model to use). Modules used first, Stats used as fallback...
	uint16_t getVisualType1(uint16_t modItemID);
	uint16_t getVisualType2(uint16_t modItemID);
	uint16_t getVisualType3(uint16_t modItemID);

	bool getIsTwoHanded(uint16_t modItemID1 = 0);

	const char *getColorStringForQuality();
	const char *getTooltip(uint16_t modItemID1, uint16_t modItemID2, uint16_t modItemID3); // Can parse modItemID# = 0 for base tooltip without any mods...
};

//
// Global Stuff...
//

extern inventoryItem *BG_GetInventoryItemByID(uint16_t id);
extern void GenerateAllInventoryItems(void);
#endif
