#pragma once 
#include "Utils.h"
#include "ConfigStructs.h"

extern const std::string configPath;

constexpr float configVersion = 7.0f;

namespace HoningConfig {
	//default maximum amount that can be used per upgrade
	constexpr int DEFAULT_MAX_SOLAR_GRACE_AMOUNT = 12;
	//default maximum amount that can be used per upgrade
	constexpr int DEFAULT_MAX_SOLAR_BLESSING_AMOUNT = 6;
	//default maximum amount that can be used per upgrade
	constexpr int DEFAULT_MAX_SOLAR_PROTECTION_AMOUNT = 2;
	//always only 1 can be used for honing 
	constexpr int MAX_HONING_BOOK_AMOUNT = 1;
	//the books always give +10% chance
	constexpr float HONING_BOOK_CHANCE = 10.0f;

	//effectiveness multiplier for the chance
	constexpr float GRACE_TO_BLESSING_MULTIPLIER = 2.0;
	//effectiveness multiplier for the chance
	constexpr float BLESSING_TO_PROTECTION_MULTIPLIER = 3.0;
	//the limit of the added honing chance from solar material combined
	//todo: new: seems like this is no longer valid
	constexpr float MAX_ADDITIONAL_SOLAR_CHANCE = 340.01f;
	//the multiplier to the base chance that gets added as artisans energy on a failed honing attempt
	constexpr float ARTISANS_ENERGY_MULTIPLIER = 0.465168;
	constexpr float ARTISANS_ENERGY_ADDED_FLAT = -0.0000772596;

	//todo: test if the research also affects the solar material chance 
	constexpr float T3_STRONGHOLD_HONING_RESEARCH_ADD = 10.0f;

	//at +3 base!!!!!!!
	constexpr float lowSetBaseGearScore = 1310.0f;
	constexpr int lowSetBaseHoningLevel = 3;
	constexpr int lowSetMaxHoningLevel = 15;
	//at +0 base 
	constexpr float mediumSetBaseGearScore = 1340.0f;
	constexpr int mediumSetBaseHoningLevel = 0;
	constexpr int mediumSetMaxHoningLevel = 20;

	constexpr float baseGearScorePerHone = 5.0f / 6.0f;
	constexpr float baseGearScorePerHoneAt16HonesAndAbove = baseGearScorePerHone * 3;

	//how much silver 1 gold would be worth
	extern int silverAmountPerGold;
	extern float baseHoningSuccessRate;
	extern bool isIlvl1340Set;
	extern bool useMarysShopPrices;
	extern float targetGearScore;
	extern bool useT3StrongholdHoningResearch;
	extern bool useBooksForFullMaterialCalc;
	extern int itemHoningLevel;
}

namespace MarketPrices {
	extern float honorLeapstone;
	extern float greaterHonorLeapstone;
	extern float simpleFusion;
	extern float basicFusion;
	//price for a single one, but sold in bundles of 10
	extern float destructionStone;
	//price for a single one, but sold in bundles of 10
	extern float guardianStone;
	extern float solarGrace;
	extern float solarBlessing;
	extern float solarProtection;
	//used for armour honing +10%
	extern float tailoringMendingBook;
	//used for weapon honing +10% chance
	extern float metallurgyWeldingBook;
	extern float smallHonorShardPouch;
	extern float largeHonorShardPouch;
	extern float powderOfSage;
}

//the prices are already converted to gold and are not in crystals!
namespace MarysPrices {
	//gold cost of one crystal
	extern float crystalPrice;

	extern float honorLeapstone;
	extern float greaterHonorLeapstone;
	extern float simpleFusion;
	extern float basicFusion;
	extern float destructionStone;
	extern float guardianStone;
	extern float solarGrace;
	extern float solarBlessing;
	extern float solarProtection;

	//convert a marys shop offer from crystal to gold cost 
	// first parameter should be amountGained and second parameter should be crystalCost
	static float convertCrystalToGoldPrice(const std::tuple<float, float>& arguments);

}

bool loadConfig();

float getHonorLeapstoneCost();

float getGreaterHonorLeapstoneCost();

float getSimpleFusionCost();

float getBasicFusionCost();

float getSolarGraceCost();

float getSolarBlessingCost();

float getSolarProtectionCost();

float getHoningBookCost(bool isWeapon, bool isIlvl1340Set);

float getDestructionStoneCost();

float getGuardianStoneCost();