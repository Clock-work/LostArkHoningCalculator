#include "LostArkHoningCalculator/HoningCalculation.h"
#include "LostArkHoningCalculator/GearScoreSelection.h"
#include "LostArkHoningCalculator/Config.h"
#include "LostArkHoningCalculator/utils/Http.h"
#include "LostArkHoningCalculator/ChaosExchange.h"
#include <algorithm>

void printMarysShop()
{
	setConsoleColour(13);
	std::cout << std::endl << "Printing how much cheaper the following Materials would be from Marys Shop" << std::endl;
	setConsoleColour(15);
	std::cout << std::endl << "Negative Numbers mean that it's more expensive!" << std::endl;

	std::cout << std::endl << "Honor Leapstone: " << howMuchCheaper(MarysPrices::honorLeapstone, MarketPrices::honorLeapstone) << "% cheaper" << std::endl;
	std::cout << std::endl << "Greater Honor Leapstone: " << howMuchCheaper(MarysPrices::greaterHonorLeapstone, MarketPrices::greaterHonorLeapstone) << "% cheaper" << std::endl;
	std::cout << std::endl << "Normal to Greater Leapstone(5 : 1 trade): " << howMuchCheaper(MarysPrices::honorLeapstone * 5, MarketPrices::greaterHonorLeapstone) << "% cheaper" << std::endl;

	std::cout << std::endl << "Simple Fusion Material: " << howMuchCheaper(MarysPrices::simpleFusion, MarketPrices::simpleFusion) << "% cheaper" << std::endl;
	std::cout << std::endl << "Basic Fusion Material: " << howMuchCheaper(MarysPrices::basicFusion, MarketPrices::basicFusion) << "% cheaper" << std::endl;

	std::cout << std::endl << "Destruction Stone Crystal: " << howMuchCheaper(MarysPrices::destructionStone, MarketPrices::destructionStone) << "% cheaper" << std::endl;
	std::cout << std::endl << "Guardian Stone Crystal: " << howMuchCheaper(MarysPrices::guardianStone, MarketPrices::guardianStone) << "% cheaper" << std::endl;

	std::cout << std::endl << "Solar Grace: " << howMuchCheaper(MarysPrices::solarGrace, MarketPrices::solarGrace) << "% cheaper" << std::endl;
	std::cout << std::endl << "Solar Blessing: " << howMuchCheaper(MarysPrices::solarBlessing, MarketPrices::solarBlessing) << "% cheaper" << std::endl;
	std::cout << std::endl << "Solar Protection: " << howMuchCheaper(MarysPrices::solarProtection, MarketPrices::solarProtection) << "% cheaper" << std::endl;

	std::cout << std::endl << std::endl << std::endl;
	setConsoleColour(15);
}

bool hasNewVersion()
{
	utils::HttpResult result = utils::httpGet("raw.githubusercontent.com", "https://raw.githubusercontent.com/Clock-work/LostArkHoningCalculator/main/LostArkHoningCalculator/include/LostArkHoningCalculator/Config.h");
	std::vector<std::string> lines = splitString(result.resultData, '\n');
	float newVersion = 0.0f;
	for ( std::string& line : lines )
	{
		if ( line.find("static float configVersion =") != std::string::npos )
		{
			std::vector<std::string> parts = splitString(line, '=');
			std::string versionString = line.substr(line.find('=') + 1);
			versionString = versionString.substr(0, versionString.find(';'));
			versionString.erase(std::remove(versionString.begin(), versionString.end(), ' '), versionString.end());
			newVersion = std::stof(versionString);
		}
	}

	if ( isMore(newVersion, configVersion) )
	{
		return true;
	}
	return false;
}

void calculatePreciseDaggervsHitmaster()
{
	float attributeCrit = 19.68f;
	float cardCrit = 0.0f;
	float setCrit = 15.0f;
	
	float baseCrit = attributeCrit + cardCrit + setCrit;
	float baseCritDmg = 200.0f;

	float setBurstCrit = 25.0f;

	float igniteBurstCrit = baseCrit + setBurstCrit + 25.0f;
	float igniteBurstCritDmg = baseCritDmg + 50.0f;


	//not sure if this affects the efficiency, or if it's a more multiplier 
	float otherDmgIncrease = 0.0f;


	float hitDmgMul = 16.0f;
	float daggerCritDmg = -12.0f;
	float daggerCrit =  20.0f;
	float bluntCritDmg = 50.0f;
	float bluntDmgMulti = -2.0f;

	float baseDmgHit = std::min(baseCrit, 100.0f) / 100 * (baseCritDmg + bluntCritDmg) / 100 * (1 + (otherDmgIncrease + hitDmgMul) / 100);
	float baseDmgPrecise = std::min(baseCrit + daggerCrit, 100.0f) / 100 * (baseCritDmg + daggerCritDmg) / 100 * (1 + otherDmgIncrease / 100);
	float baseDmgBlunt = std::min(baseCrit, 100.0f) / 100 * baseCritDmg / 100 * (1 + (otherDmgIncrease + bluntDmgMulti) / 100);

	float igniteDmgHit = std::min(igniteBurstCrit, 100.0f) / 100 * igniteBurstCritDmg / 100 * (1 + (otherDmgIncrease + hitDmgMul) / 100);
	float igniteDmgPrecise = std::min(igniteBurstCrit + daggerCrit, 100.0f) / 100 * (igniteBurstCritDmg + daggerCritDmg) / 100 * (1 + otherDmgIncrease / 100);
	float igniteDmgBlunt = std::min(igniteBurstCrit, 100.0f) / 100 * (igniteBurstCritDmg + bluntCritDmg) / 100 * (1 + (otherDmgIncrease + bluntDmgMulti) / 100);

	float totalHit = baseDmgHit * 1.5 + igniteDmgHit;
	float totalPrecise = baseDmgPrecise * 1.5 + igniteDmgPrecise;
	float totalBlunt = baseDmgBlunt * 1.5 + igniteDmgBlunt;

	//result: until i have the card set, or more base crit, precise dagger is more efficient than hit master (if the set burts crit is not always up)
	//but if i have more specialization and can proc ignite more often, then hit master can also gain efficiency 
	int test;

}

int main()
{
	try
	{
		calculatePreciseDaggervsHitmaster();
		setConsoleColour(13);
		std::cout << "Started T3 Lost Ark Calculator!" << std::endl << std::endl;
		setConsoleColour(15);
		std::cout << "This will show you the most effecient way to hone your gear including artisans energy and how much solar grace, blessing, protection to use." << std::endl;
		std::cout << "The final cost does not include the upgrade cost of the first step with Silver and Honor shards!" << std::endl;
		std::cout << "After +12 The Program can take a few minutes to calculate and needs 2 GB of free RAM!" << std::endl;
		if ( loadConfig() )
		{
			std::cout << "The config file has changed with new options, or was just created." << std::endl;
			std::cout << "Please edit the config \"LostArkCalculatorConfig.txt\" file with your market prices and honing level and restart the program!" << std::endl;
			sleep(1500);
			std::cin.get();
			return 1;
		}

		if ( hasNewVersion() )
		{
			std::cout << "A new Update is available on \"https://github.com/Clock-work/LostArkHoningCalculator\". Download it now!!!" << std::endl << std::endl << std::endl;
			sleep(1500);
			std::cin.get();
			return 1;
		}

		sleep(2500);

		printMarysShop();

		printChaosExchange();

		sleep(2500);

		printBestTargetGearScoreSelection();

		printHoningCalculation();

		setConsoleColour(15);
	}
	catch ( std::exception e )
	{
		std::cout << "Error running Lost Ark Calculator: " << e.what() << std::endl;
	}
	std::cin.get();
	return 0;
}