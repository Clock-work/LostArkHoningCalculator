#pragma once 
#include "Utils.h"
#include <tuple>

static const std::string configPath = "LostArkCalculatorConfig.txt";

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

	//the multiplier to the base chance that gets added as artisans energy on a failed honing attempt
	static const float ARTISANS_ENERGY_MULTIPLIER = 0.465;
	//recursion stop to avoid the program taking too much ram
	static const int MAX_HONING_TRIES = 6;

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

inline void replaceConfig()
{
	std::vector<std::string> lines;

	lines.push_back("Edit all config values here according to your market prices and your Gear Honing Level \n\n\n");
	lines.push_back("Currency Exchange Buy Crystals");
	lines.push_back("Gold Cost:880");
	lines.push_back("Crystals gained:95");

	lines.push_back("\nHow much Silver would 1 Gold be worth for you?(the gold exchange vendor would give 100 Silver for 1 Gold):500\n");

	lines.push_back("\n\nAuction House Market Prices");
	lines.push_back("Honor Leapstone:100");
	lines.push_back("Great Honor Leapstone:332");
	lines.push_back("Simple Oreha Fusion Material:11");
	lines.push_back("Basic Oreha Fusion Material:14");
	lines.push_back("Destruction Stone Crystal(for 10):80");
	lines.push_back("Guardian Stone Crystal(for 10):52");
	lines.push_back("Solar Grace:45");
	lines.push_back("Solar Blessing:186");
	lines.push_back("Solar Protection:405");

	lines.push_back("\n\nHoning Config");
	lines.push_back("Are you using the Item Level 1340 Set with Great Honor Leapstones?(true/false):false");
	lines.push_back("Your current Base Honing Success Rate without Solar stuff after failing an upgrade (otherwise leave at 0):0");
	lines.push_back("The current Honing Item Level of your Gear (the number on your Gear from 0 to 19):0");

	lines.push_back("\n\nMarys Shop");
	lines.push_back("Honor Leapstone Amount:10");
	lines.push_back("Honor Leapstone Crystal Cost:20");
	lines.push_back("Greater Honor Leapstone Amount:5");
	lines.push_back("Greater Honor Leapstone Crystal Cost:50");
	lines.push_back("Simple Oreha Fusion Material Amount(in chest):10");
	lines.push_back("Simple Oreha Fusion Material Crystal Cost:30");
	lines.push_back("Basic Oreha Fusion Material Amount(in chest):10");
	lines.push_back("Basic Oreha Fusion Material Crystal Cost:40");

	lines.push_back("Destruction Stone Crystal Amount:300");
	lines.push_back("Destruction Stone Crystal Crystal Cost:240");
	lines.push_back("Guardian Stone Crystal Amount:200");
	lines.push_back("Guardian Stone Crystal Crystal Cost:50");

	lines.push_back("Solar Grace Amount:20");
	lines.push_back("Solar Grace Crystal Cost:80");
	lines.push_back("Solar Blessing Amount:15");
	lines.push_back("Solar Blessing Crystal Cost:150");
	lines.push_back("Solar Protection Amount:3");
	lines.push_back("Solar Protection Crystal Cost:150");

	writeUtf8FileFromLines(configPath, lines);
}

inline bool parseConfig()
{
	try
	{
		std::vector<std::vector<std::string>> lines = splitVector(readUtf8FileToLines(configPath), ':', 2);
		for ( int i = 0; i < lines.size(); ++i )
		{
			if ( lines.at(i).size() < 2 )
			{
				lines.erase(lines.begin() + i--);
			}
			else if ( lines.at(i).at(1).find(',') != std::string::npos )
			{
				std::vector<std::string> parts = splitString(lines.at(i).at(1), ',');
				std::string mostRecentPart = parts.at(parts.size() - 1);
				lines.at(i).at(1) = mostRecentPart;
			}
		}

		if ( lines.empty() )
		{
			std::cout << "Please edit the config \"LostArkCalculatorConfig.txt\" file with your market prices and honing level and restart the program" << std::endl;
			replaceConfig();
			return false;
		}
		int currentLineCounter = 0;

		auto getNextValue = [&] ()
		{
			float value = (float) std::stod(lines.at(currentLineCounter)[1]);
			currentLineCounter += 1;
			return value;
		};

		auto getNextTwoValues = [&] ()
		{
			float value1 = (float) std::stod(lines.at(currentLineCounter)[1]);
			currentLineCounter += 1;
			float value2 = (float) std::stod(lines.at(currentLineCounter)[1]);
			currentLineCounter += 1;
			return std::make_tuple(value1, value2);
		};

		MarysPrices::crystalPrice = getNextValue() / getNextValue();

		HoningConfig::silverAmountPerGold = getNextValue();

		MarketPrices::honorLeapstone = getNextValue();
		MarketPrices::greaterHonorLeapstone = getNextValue();

		MarketPrices::simpleFusion = getNextValue();
		MarketPrices::basicFusion = getNextValue();

		MarketPrices::destructionStone = getNextValue() / 10.0;//price per 10 stones
		MarketPrices::guardianStone = getNextValue() / 10.0;

		MarketPrices::solarGrace = getNextValue();
		MarketPrices::solarBlessing = getNextValue();
		MarketPrices::solarProtection = getNextValue();

		auto& line = lines.at(currentLineCounter++)[1];
		HoningConfig::isIlvl1340Set = line == "True" || line == "true" ? true : false;
		HoningConfig::baseHoningSuccessRate = getNextValue();
		HoningConfig::itemHoningLevel = getNextValue();

		MarysPrices::honorLeapstone = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::greaterHonorLeapstone = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::simpleFusion = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::basicFusion = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::destructionStone = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::guardianStone = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::solarGrace = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::solarBlessing = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());
		MarysPrices::solarProtection = MarysPrices::convertCrystalToGoldPrice(getNextTwoValues());

		return true;
	}
	catch ( const std::exception& e )
	{
		std::cout << std::string(std::string("config loading error(please check, or delete your config file): ") + e.what(), 1);
		return false;
	}
}