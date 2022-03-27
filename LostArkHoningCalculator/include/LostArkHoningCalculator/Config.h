#pragma once 
#include "Utils.h"
#include "ConfigStructs.h"

static const std::string configPath = "LostArkCalculatorConfig.txt";

static float configVersion = 5.0f;

namespace HoningConfig {
	//default maximum amount that can be used per upgrade
	static const int DEFAULT_MAX_SOLAR_GRACE_AMOUNT = 12;
	//default maximum amount that can be used per upgrade
	static const int DEFAULT_MAX_SOLAR_BLESSING_AMOUNT = 6;
	//default maximum amount that can be used per upgrade
	static const int DEFAULT_MAX_SOLAR_PROTECTION_AMOUNT = 2;
	//always only 1 can be used for honing 
	static const int MAX_HONING_BOOK_AMOUNT = 1;
	//the books always give +10% chance
	static const float HONING_BOOK_CHANCE = 10.0f;

	//effectiveness multiplier for the chance
	static const float GRACE_TO_BLESSING_MULTIPLIER = 2.0;
	//effectiveness multiplier for the chance
	static const float BLESSING_TO_PROTECTION_MULTIPLIER = 3.0;
	//the limit of the added honing chance from solar material combined
	//todo: new: seems like this is no longer valid
	static const float MAX_ADDITIONAL_SOLAR_CHANCE = 340.01f;
	//the multiplier to the base chance that gets added as artisans energy on a failed honing attempt
	static const float ARTISANS_ENERGY_MULTIPLIER = 0.465168;
	static const float ARTISANS_ENERGY_ADDED_FLAT = -0.0000772596;

	//todo: test if the research also affects the solar material chance 
	static const float T3_STRONGHOLD_HONING_RESEARCH_ADD = 10.0f;

	//at +3 base!!!!!!!
	static const float lowSetBaseGearScore = 1310.0f;
	static const int lowSetBaseHoningLevel = 3;
	static const int lowSetMaxHoningLevel = 15;
	//at +0 base 
	static const float mediumSetBaseGearScore = 1340.0f;
	static const int mediumSetBaseHoningLevel = 0;
	static const int mediumSetMaxHoningLevel = 20;

	static const float baseGearScorePerHone = 5.0f / 6.0f;
	static const float baseGearScorePerHoneAt16HonesAndAbove = baseGearScorePerHone * 3;

	//how much silver 1 gold would be worth
	static int silverAmountPerGold;
	static float baseHoningSuccessRate;
	static bool isIlvl1340Set;
	static bool useMarysShopPrices;
	static float targetGearScore;
	static bool useT3StrongholdHoningResearch;
	static bool useBooksForFullMaterialCalc;
	static int itemHoningLevel;
}

namespace MarketPrices {
	static float honorLeapstone;
	static float greaterHonorLeapstone;
	static float simpleFusion;
	static float basicFusion;
	//price for a single one, but sold in bundles of 10
	static float destructionStone;
	//price for a single one, but sold in bundles of 10
	static float guardianStone;
	static float solarGrace;
	static float solarBlessing;
	static float solarProtection;
	//used for armour honing +10%
	static float tailoringMendingBook;
	//used for weapon honing +10% chance
	static float metallurgyWeldingBook;
	static float smallHonorShardPouch;
	static float largeHonorShardPouch;
	static float powderOfSage;
}

//the prices are already converted to gold and are not in crystals!
namespace MarysPrices {
	//gold cost of one crystal
	static float crystalPrice;

	static float honorLeapstone;
	static float greaterHonorLeapstone;
	static float simpleFusion;
	static float basicFusion;
	static float destructionStone;
	static float guardianStone;
	static float solarGrace;
	static float solarBlessing;
	static float solarProtection;

	//convert a marys shop offer from crystal to gold cost 
	// first parameter should be amountGained and second parameter should be crystalCost
	static float convertCrystalToGoldPrice(const std::tuple<float, float>& arguments)
	{
		float amountGained = std::get<0>(arguments);
		float crystalCost = std::get<1>(arguments);
		return  ( crystalCost * crystalPrice ) / static_cast<float>( amountGained );
	}

}

inline bool loadConfig()
{
	try
	{
		ConfigFile configFile { configPath, configVersion};

		float crystalCost = configFile.getNextValueFloat(0.1f, 493.0f, "Edit all config values here according to your market prices and your Gear Honing Level \n\n\n\nCurrency Exchange Buy Crystals\nGold Cost");
		float crystalAmount = configFile.getNextValueFloat(0.1f, 95.0f, "Crystals gained");
		MarysPrices::crystalPrice = crystalCost / crystalAmount;

		HoningConfig::silverAmountPerGold = configFile.getNextValueFloat(0.1f, 500.0f, "\nHow much Silver would 1 Gold be worth for you?(the gold exchange vendor would give 100 Silver for 1 Gold)");

		MarketPrices::honorLeapstone = configFile.getNextValueFloat(0.1f, 23.0f, "\n\n\nAuction House Market Prices\nHonor Leapstone");
		MarketPrices::greaterHonorLeapstone = configFile.getNextValueFloat(0.1f, 114.0f, "Great Honor Leapstone");

		MarketPrices::simpleFusion = configFile.getNextValueFloat(0.1f, 9.0f, "Simple Oreha Fusion Material");
		MarketPrices::basicFusion = configFile.getNextValueFloat(0.1f, 10.0f, "Basic Oreha Fusion Material");

		MarketPrices::destructionStone = configFile.getNextValueFloat(0.1f, 14.0f, "Destruction Stone Crystal(for 10)") / 10.0f;//price per 10 stones
		MarketPrices::guardianStone = configFile.getNextValueFloat(0.1f, 16.0f, "Guardian Stone Crystal(for 10)") / 10.0f;

		MarketPrices::solarGrace = configFile.getNextValueFloat(0.1f, 27.0f, "Solar Grace");
		MarketPrices::solarBlessing = configFile.getNextValueFloat(0.1f, 80.0f, "Solar Blessing");
		MarketPrices::solarProtection = configFile.getNextValueFloat(0.1f, 178.0f, "Solar Protection");
		MarketPrices::metallurgyWeldingBook = configFile.getNextValueFloat(3.0f, 2000.0f, "Metallurgy: Basic Welding");
		MarketPrices::tailoringMendingBook = configFile.getNextValueFloat(3.0f, 1174.0f, "Tailoring: Basic Mending");

		MarketPrices::smallHonorShardPouch = configFile.getNextValueFloat(2.0f, 24.0f, "Small Honor Shard Pouch");
		MarketPrices::largeHonorShardPouch = configFile.getNextValueFloat(4.0f, 53.0f, "Large Honor Shard Pouch");
		MarketPrices::powderOfSage = configFile.getNextValueFloat(4.0f, 95.0f, "Powder of Sage");

		HoningConfig::isIlvl1340Set = configFile.getNextValueBool(0.1f, false, "\n\nHoning Config\nAre you using the Item Level 1340 Set with Great Honor Leapstones?(true/false)");
		HoningConfig::baseHoningSuccessRate = configFile.getNextValueFloat(0.1f, 0.0f, "Your current Base Honing Success Rate without Solar materials after already failing some previous upgrades (otherwise leave at 0)");
		HoningConfig::useMarysShopPrices = configFile.getNextValueBool(2.0f, true, "Also include Marys Shop prices for honing materials and use the cheapest for calculation(true/false)");
		HoningConfig::targetGearScore = configFile.getNextValueFloat(5.0f, 0.0f, "Only Set Your Target Gear ItemLevel here 1340-1500 if you want to compute the best individual gear upgrade count(leave at 0 to disable)");
		HoningConfig::useT3StrongholdHoningResearch = configFile.getNextValueBool(4.0f, false, "Are you using the T3 Stronghold Honing Research for alts with your main being over ilvl 1385?(true/false)");
		HoningConfig::useBooksForFullMaterialCalc = configFile.getNextValueBool(5.0f, false, "Should Honing Books be displayed in the alternative full material use Calculation for comparison(does not affect main Calculation)?(true/false)");
		HoningConfig::itemHoningLevel = configFile.getNextValueFloat(0.1f, 0.0f, "The current Honing Item Level of your Gear (the number on your Gear from 0 to 19)");

		MarysPrices::honorLeapstone = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 10.0f, "\n\nMarys Shop\nHonor Leapstone Amount", 20.0f, "Honor Leapstone Crystal Cost"));
		MarysPrices::greaterHonorLeapstone = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 5.0f, "Greater Honor Leapstone Amount", 50.0f, "Greater Honor Leapstone Crystal Cost"));

		MarysPrices::simpleFusion = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 10.0f, "Simple Oreha Fusion Material Amount(in chest)", 30.0f, "Simple Oreha Fusion Material Crystal Cost"));
		MarysPrices::basicFusion = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 10.0f, "Basic Oreha Fusion Material Amount(in chest)", 40.0f, "Basic Oreha Fusion Material Crystal Cost"));

		MarysPrices::destructionStone = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 300.0f, "Destruction Stone Crystal Amount", 240.0f, "Destruction Stone Crystal Crystal Cost"));
		MarysPrices::guardianStone = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 200.0f, "Guardian Stone Crystal Amount", 50.0f, "Guardian Stone Crystal Crystal Cost"));
		
		MarysPrices::solarGrace = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 20.0f, "Solar Grace Amount", 80.0f, "Solar Grace Crystal Cost"));
		MarysPrices::solarBlessing = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 15.0f, "Solar Blessing Amount", 150.0f, "Solar Blessing Crystal Cost"));
		MarysPrices::solarProtection = MarysPrices::convertCrystalToGoldPrice(configFile.getNextTwoValuesFloat(0.1f, 3.0f, "Solar Protection Amount", 150.0f, "Solar Protection Crystal Cost"));

		return configFile.writeChanges();
	}
	catch ( const std::exception& e )
	{
		throw std::runtime_error("Config loading error! Please check, or delete your config file!");
	}
}

inline float getHonorLeapstoneCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::honorLeapstone, MarysPrices::honorLeapstone);
	else
		return MarketPrices::honorLeapstone;
}

inline float getGreaterHonorLeapstoneCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::greaterHonorLeapstone, MarysPrices::greaterHonorLeapstone);
	else
		return MarketPrices::greaterHonorLeapstone;
}

inline float getSimpleFusionCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::simpleFusion, MarysPrices::simpleFusion);
	else
		return MarketPrices::simpleFusion;
}

inline float getBasicFusionCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::basicFusion, MarysPrices::basicFusion);
	else
		return MarketPrices::basicFusion;
}

inline float getSolarGraceCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::solarGrace, MarysPrices::solarGrace);
	else
		return MarketPrices::solarGrace;
}

inline float getSolarBlessingCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::solarBlessing, MarysPrices::solarBlessing);
	else
		return MarketPrices::solarBlessing;
}

inline float getSolarProtectionCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::solarProtection, MarysPrices::solarProtection);
	else
		return MarketPrices::solarProtection;
}

inline float getHoningBookCost(bool isWeapon, bool isIlvl1340Set)
{
	if (isIlvl1340Set)
	{
		//todo: not implemented yet 
		return 99999999.9f;
	}
	if ( isWeapon )
		return MarketPrices::metallurgyWeldingBook;
	else
		return MarketPrices::tailoringMendingBook;
}

inline float getDestructionStoneCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::destructionStone, MarysPrices::destructionStone);
	else
		return MarketPrices::destructionStone;
}

inline float getGuardianStoneCost()
{
	if ( HoningConfig::useMarysShopPrices )
		return std::min(MarketPrices::guardianStone, MarysPrices::guardianStone);
	else
		return MarketPrices::guardianStone;
}