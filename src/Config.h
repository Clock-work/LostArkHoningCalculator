#pragma once 
#include "Utils.h"
#include "ConfigStructs.h"

static const std::string configPath = "LostArkCalculatorConfig.txt";

static float configVersion = 1.0f;

namespace HoningConfig {
	//default maximum amount that can be used per upgrade
	static const int DEFAULT_MAX_SOLAR_GRACE_AMOUNT = 12;
	//default maximum amount that can be used per upgrade
	static const int DEFAULT_MAX_SOLAR_BLESSING_AMOUNT = 6;
	//default maximum amount that can be used per upgrade
	static const int DEFAULT_MAX_SOLAR_PROTECTION_AMOUNT = 2;

	//effectiveness multiplier for the chance
	static const float GRACE_TO_BLESSING_MULTIPLIER = 2.0;
	//effectiveness multiplier for the chance
	static const float BLESSING_TO_PROTECTION_MULTIPLIER = 3.0;
	//the limit of the added honing chance from solar material combined
	static const float MAX_ADDITIONAL_SOLAR_CHANCE = 34.01f;

	//the multiplier to the base chance that gets added as artisans energy on a failed honing attempt
	static const float ARTISANS_ENERGY_MULTIPLIER = 0.465;

	//how much silver 1 gold would be worth
	static int silverAmountPerGold;
	static int itemHoningLevel;
	static float baseHoningSuccessRate;
	static bool isIlvl1340Set;
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

		float crystalCost = configFile.getNextValueFloat(0.1f, 765.0f, "Edit all config values here according to your market prices and your Gear Honing Level \n\n\n\nCurrency Exchange Buy Crystals\nGold Cost");
		float crystalAmount = configFile.getNextValueFloat(0.1f, 95.0f, "Crystals gained");
		MarysPrices::crystalPrice = crystalCost / crystalAmount;

		HoningConfig::silverAmountPerGold = configFile.getNextValueFloat(0.1f, 500.0f, "\nHow much Silver would 1 Gold be worth for you?(the gold exchange vendor would give 100 Silver for 1 Gold)");

		MarketPrices::honorLeapstone = configFile.getNextValueFloat(0.1f, 93.0f, "\n\n\nAuction House Market Prices\nHonor Leapstone");
		MarketPrices::greaterHonorLeapstone = configFile.getNextValueFloat(0.1f, 400.0f, "Great Honor Leapstone");

		MarketPrices::simpleFusion = configFile.getNextValueFloat(0.1f, 11.0f, "Simple Oreha Fusion Material");
		MarketPrices::basicFusion = configFile.getNextValueFloat(0.1f, 12.0f, "Basic Oreha Fusion Material");

		MarketPrices::destructionStone = configFile.getNextValueFloat(0.1f, 37.0f, "Destruction Stone Crystal(for 10)") / 10.0f;//price per 10 stones
		MarketPrices::guardianStone = configFile.getNextValueFloat(0.1f, 24.0f, "Guardian Stone Crystal(for 10)") / 10.0f;

		MarketPrices::solarGrace = configFile.getNextValueFloat(0.1f, 51.0f, "Solar Grace");
		MarketPrices::solarBlessing = configFile.getNextValueFloat(0.1f, 171.0f, "Solar Blessing");
		MarketPrices::solarProtection = configFile.getNextValueFloat(0.1f, 385.0f, "Solar Protection");

		HoningConfig::isIlvl1340Set = configFile.getNextValueBool(0.1f, false, "\n\nHoning Config\nAre you using the Item Level 1340 Set with Great Honor Leapstones?(true/false)");
		HoningConfig::baseHoningSuccessRate = configFile.getNextValueFloat(0.1f, 0.0f, "Your current Base Honing Success Rate without Solar stuff after failing previous upgrades (otherwise leave at 0)");
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
		std::cout << std::string(std::string("Config loading error! Please check, or delete your config file!"), 1);
		return false;
	}
}