#include "LostArkHoningCalculator/HoningCalculation.h"
#include "LostArkHoningCalculator/GearScoreSelection.h"
#include "LostArkHoningCalculator/Config.h"
#include "LostArkHoningCalculator/utils/Http.h"
#include "LostArkHoningCalculator/ChaosExchange.h"

void printMarysShop()
{
	std::cout << std::endl << "Printing how much cheaper the following Materials would be from Marys Shop" << std::endl;
	std::cout << std::endl << "Negative Numbers mean that it's more expensive!" << std::endl;

	std::cout << std::endl << "Honor Leapstone: " << howMuchCheaper(MarysPrices::honorLeapstone, MarketPrices::honorLeapstone) << "% cheaper" << std::endl;
	std::cout << std::endl << "Greater Honor Leapstone: " << howMuchCheaper(MarysPrices::greaterHonorLeapstone, MarketPrices::greaterHonorLeapstone) << "% cheaper" << std::endl;

	std::cout << std::endl << "Simple Fusion Material: " << howMuchCheaper(MarysPrices::simpleFusion, MarketPrices::simpleFusion) << "% cheaper" << std::endl;
	std::cout << std::endl << "Basic Fusion Material: " << howMuchCheaper(MarysPrices::basicFusion, MarketPrices::basicFusion) << "% cheaper" << std::endl;

	std::cout << std::endl << "Destruction Stone Crystal: " << howMuchCheaper(MarysPrices::destructionStone, MarketPrices::destructionStone) << "% cheaper" << std::endl;
	std::cout << std::endl << "Guardian Stone Crystal: " << howMuchCheaper(MarysPrices::guardianStone, MarketPrices::guardianStone) << "% cheaper" << std::endl;

	std::cout << std::endl << "Solar Grace: " << howMuchCheaper(MarysPrices::solarGrace, MarketPrices::solarGrace) << "% cheaper" << std::endl;
	std::cout << std::endl << "Solar Blessing: " << howMuchCheaper(MarysPrices::solarBlessing, MarketPrices::solarBlessing) << "% cheaper" << std::endl;
	std::cout << std::endl << "Solar Protection: " << howMuchCheaper(MarysPrices::solarProtection, MarketPrices::solarProtection) << "% cheaper" << std::endl;

	std::cout << std::endl << std::endl;
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

int main()
{
	try
	{
		std::cout << std::endl << "Started T3 Lost Ark Calculator!" << std::endl << std::endl;
		std::cout << std::endl << "This will show you the most effecient way to hone your gear including artisans energy and how much solar grace, blessing, protection to use." << std::endl << std::endl;
		std::cout << std::endl << "The final cost does not include the upgrade cost of the first step with Silver and Honor shards!" << std::endl << std::endl;
		std::cout << std::endl << "After +12 The Program can take a few minutes to calculate and needs 2 GB of free RAM!" << std::endl << std::endl;
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

		sleep(1500);

		printMarysShop();

		printChaosExchange();

		sleep(1500);

		printBestTargetGearScoreSelection();

		printHoningCalculation();

	}
	catch ( std::exception e )
	{
		std::cout << "Error running Lost Ark Calculator: " << e.what() << std::endl;
	}
	std::cin.get();
	return 0;
}