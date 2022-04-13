#include "LostArkHoningCalculator/Config.h"
#include <algorithm>

const std::string configPath = "LostArkCalculatorConfig.txt";

namespace HoningConfig {
	int silverAmountPerGold;
	float baseHoningSuccessRate;
	bool isIlvl1340Set;
	bool useMarysShopPrices;
	float targetGearScore;
	bool useT3StrongholdHoningResearch;
	bool useBooksForFullMaterialCalc;
	int itemHoningLevel;
}

namespace MarketPrices {
	float honorLeapstone;
	float greaterHonorLeapstone;
	float simpleFusion;
	float basicFusion;
	float destructionStone;
	float guardianStone;
	float solarGrace;
	float solarBlessing;
	float solarProtection;
	float tailoringMendingBook;
	float metallurgyWeldingBook;
	float smallHonorShardPouch;
	float largeHonorShardPouch;
	float powderOfSage;
}

namespace MarysPrices {
	float crystalPrice;
	float honorLeapstone;
	float greaterHonorLeapstone;
	float simpleFusion;
	float basicFusion;
	float destructionStone;
	float guardianStone;
	float solarGrace;
	float solarBlessing;
	float solarProtection;
	static float convertCrystalToGoldPrice(const std::tuple<float, float>& arguments)
	{
		float amountGained = std::get<0>(arguments);
		float crystalCost = std::get<1>(arguments);
		return  (crystalCost * crystalPrice) / static_cast<float>(amountGained);
	}
}

bool loadConfig()
{
	try
	{
		ConfigFile configFile {configPath, configVersion};

		float crystalCost = configFile.getNextValueFloat(0.1f, 450.0f, "Edit all config values here according to your market prices and your Gear Honing Level \n\n\n\nCurrency Exchange Buy Crystals\nGold Cost");
		float crystalAmount = configFile.getNextValueFloat(0.1f, 95.0f, "Crystals gained");
		MarysPrices::crystalPrice = crystalCost / crystalAmount;

		HoningConfig::silverAmountPerGold = configFile.getNextValueFloat(0.1f, 500.0f, "\nHow much Silver would 1 Gold be worth for you?(the gold exchange vendor would give 100 Silver for 1 Gold)");

		MarketPrices::honorLeapstone = configFile.getNextValueFloat(0.1f, 18.0f, "\n\n\nAuction House Market Prices\nHonor Leapstone");
		MarketPrices::greaterHonorLeapstone = configFile.getNextValueFloat(0.1f, 64.0f, "Great Honor Leapstone");

		MarketPrices::simpleFusion = configFile.getNextValueFloat(0.1f, 8.0f, "Simple Oreha Fusion Material");
		MarketPrices::basicFusion = configFile.getNextValueFloat(0.1f, 9.0f, "Basic Oreha Fusion Material");

		MarketPrices::destructionStone = configFile.getNextValueFloat(0.1f, 8.0f, "Destruction Stone Crystal(for 10)") / 10.0f;//price per 10 stones
		MarketPrices::guardianStone = configFile.getNextValueFloat(0.1f, 6.0f, "Guardian Stone Crystal(for 10)") / 10.0f;

		MarketPrices::solarGrace = configFile.getNextValueFloat(0.1f, 24.0f, "Solar Grace");
		MarketPrices::solarBlessing = configFile.getNextValueFloat(0.1f, 88.0f, "Solar Blessing");
		MarketPrices::solarProtection = configFile.getNextValueFloat(0.1f, 200.0f, "Solar Protection");
		MarketPrices::metallurgyWeldingBook = configFile.getNextValueFloat(3.0f, 635.0f, "Metallurgy: Basic Welding");
		MarketPrices::tailoringMendingBook = configFile.getNextValueFloat(3.0f, 399.0f, "Tailoring: Basic Mending");

		MarketPrices::smallHonorShardPouch = configFile.getNextValueFloat(2.0f, 23.0f, "Small Honor Shard Pouch");
		MarketPrices::largeHonorShardPouch = configFile.getNextValueFloat(4.0f, 77.0f, "Large Honor Shard Pouch");
		MarketPrices::powderOfSage = configFile.getNextValueFloat(4.0f, 100.0f, "Powder of Sage");

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
	catch (const std::exception& e)
	{
		throw std::runtime_error("Config loading error! Please check, or delete your config file!");
	}
}

float getHonorLeapstoneCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::honorLeapstone, MarysPrices::honorLeapstone);
	else
		return MarketPrices::honorLeapstone;
}

float getGreaterHonorLeapstoneCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::greaterHonorLeapstone, MarysPrices::greaterHonorLeapstone);
	else
		return MarketPrices::greaterHonorLeapstone;
}

float getSimpleFusionCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::simpleFusion, MarysPrices::simpleFusion);
	else
		return MarketPrices::simpleFusion;
}

float getBasicFusionCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::basicFusion, MarysPrices::basicFusion);
	else
		return MarketPrices::basicFusion;
}

float getSolarGraceCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::solarGrace, MarysPrices::solarGrace);
	else
		return MarketPrices::solarGrace;
}

float getSolarBlessingCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::solarBlessing, MarysPrices::solarBlessing);
	else
		return MarketPrices::solarBlessing;
}

float getSolarProtectionCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::solarProtection, MarysPrices::solarProtection);
	else
		return MarketPrices::solarProtection;
}

float getHoningBookCost(bool isWeapon, bool isIlvl1340Set)
{
	if (isIlvl1340Set)
	{
		//todo: not implemented yet 
		return 99999999.9f;
	}
	if (isWeapon)
		return MarketPrices::metallurgyWeldingBook;
	else
		return MarketPrices::tailoringMendingBook;
}

float getDestructionStoneCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::destructionStone, MarysPrices::destructionStone);
	else
		return MarketPrices::destructionStone;
}

float getGuardianStoneCost()
{
	if (HoningConfig::useMarysShopPrices)
		return std::min<float>(MarketPrices::guardianStone, MarysPrices::guardianStone);
	else
		return MarketPrices::guardianStone;
}

