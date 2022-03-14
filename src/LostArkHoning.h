#pragma once
#include "Utils.h"
#include <functional>

//preise für samstag mittag sind nach dem zweiten ausklammernauskommentiert. die aktuellen preise sind für sonntag mittag 16 uhr. nach dem ersten ausklammern sind die preise für sonntag morgen 6 uhr
//morgends früh für 88 verkauft. am abend dann für 79 kaufen = big profit??? am nächsten morgen war dann wieder für 90 verkauft 
// destruction stone für 85 verkauft und guardian stone für 51 

// great honor leapstone für 347 verkauft 

// solar protection für 420 verkauft, solar blessing für 187 und solar grace für 48

static std::string configPath = "LostArkCalculatorConfig.txt";

static float crystalPrice = 1;

static int silverAmountPerGold = 100;

static float leapstoneP = 75;// 79;//85;//94.0; 
static float greaterLeapstoneP = 303;// 320;//360;//306.0;

static float simpleFusionP = 11;//12.0;
static float basicFusionP = 13.9;//12.0;

static float destructionStoneP = 69 / 10.0;//56;//69;//63;
static float guardianStoneP = 44 / 10.0;//37;//44;//42;

static float solarGraceP = 43;//49;//53;//83.0;
static float solarBlessingP = 174;//170;//182;//288.0;
static float solarProtectionP = 384;// 398;//420;//539.0;

static int itemHoningLevel = 1;

static float baseHoningSuccessRate = 0;

static bool isIlvl1340Set = false;

static const int MAX_SOLAR_GRACE_AMOUNT = 12;
static const int MAX_SOLAR_BLESSING_AMOUNT = 6;
static const int MAX_SOLAR_PROTECTION_AMOUNT = 2;

static const float GRACE_TO_BLESSING_MULTIPLIER = 2.0; //solar blessing is twice as effective as solar grace 
static const float BLESSING_TO_PROTECTION_MULTIPLIER = 3.0; //solar protection is 3 times more effective as solar blessing

static const float ARTISANS_ENERGY_MULTIPLIER = 0.465;//the multiplier to the base chance that gets added as artisans energy on a failed honing attempt

static int maHonorLeapstoneA = 0;
static int maHonorLeapstoneC = 0;
static int maGreaterHonorLeapstoneA = 0;
static int maGreaterHonorLeapstoneC = 0;
static int maSimpleFusionA = 0;
static int maSimpleFusionC = 0;
static int maBasicFusionA = 0;
static int maBasicFusionC = 0;
static int maDestructionStoneA = 0;
static int maDestructionStoneC = 0;
static int maGuardianStoneA = 0;
static int maGuardianStoneC = 0;
static int maGraceA = 0;
static int maGraceC = 0;
static int maBlessingeA = 0;
static int maBlessingC = 0;
static int maProtectionA = 0;
static int maProtectionC = 0;

static void replaceConfig()
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

static bool parseConfig()
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

		crystalPrice = std::stoi(lines.at(currentLineCounter++)[1]) / ( (float) std::stoi(lines.at(currentLineCounter++)[1]) );

		silverAmountPerGold = std::stoi(lines.at(currentLineCounter++)[1]);

		leapstoneP = std::stod(lines.at(currentLineCounter++)[1]);
		greaterLeapstoneP = std::stod(lines.at(currentLineCounter++)[1]);

		simpleFusionP = std::stod(lines.at(currentLineCounter++)[1]);
		basicFusionP = std::stod(lines.at(currentLineCounter++)[1]);

		destructionStoneP = std::stod(lines.at(currentLineCounter++)[1]) / 10.0;//price per 10 stones
		guardianStoneP = std::stod(lines.at(currentLineCounter++)[1]) / 10.0;

		solarGraceP = std::stod(lines.at(currentLineCounter++)[1]);
		solarBlessingP = std::stod(lines.at(currentLineCounter++)[1]);
		solarProtectionP = std::stod(lines.at(currentLineCounter++)[1]);

		auto& line = lines.at(currentLineCounter++)[1];
		isIlvl1340Set = line == "True" || line == "true" ? true : false;
		baseHoningSuccessRate = std::stod(lines.at(currentLineCounter++)[1]);
		itemHoningLevel = std::stoi(lines.at(currentLineCounter++)[1]);

		maHonorLeapstoneA = std::stoi(lines.at(currentLineCounter++)[1]);
		maHonorLeapstoneC = std::stoi(lines.at(currentLineCounter++)[1]);
		maGreaterHonorLeapstoneA = std::stoi(lines.at(currentLineCounter++)[1]);
		maGreaterHonorLeapstoneC = std::stoi(lines.at(currentLineCounter++)[1]);
		maSimpleFusionA = std::stoi(lines.at(currentLineCounter++)[1]);
		maSimpleFusionC = std::stoi(lines.at(currentLineCounter++)[1]);
		maBasicFusionA = std::stoi(lines.at(currentLineCounter++)[1]);
		maBasicFusionC = std::stoi(lines.at(currentLineCounter++)[1]);
		maDestructionStoneA = std::stoi(lines.at(currentLineCounter++)[1]);
		maDestructionStoneC = std::stoi(lines.at(currentLineCounter++)[1]);
		maGuardianStoneA = std::stoi(lines.at(currentLineCounter++)[1]);
		maGuardianStoneC = std::stoi(lines.at(currentLineCounter++)[1]);
		maGraceA = std::stoi(lines.at(currentLineCounter++)[1]);
		maGraceC = std::stoi(lines.at(currentLineCounter++)[1]);
		maBlessingeA = std::stoi(lines.at(currentLineCounter++)[1]);
		maBlessingC = std::stoi(lines.at(currentLineCounter++)[1]);
		maProtectionA = std::stoi(lines.at(currentLineCounter++)[1]);
		maProtectionC = std::stoi(lines.at(currentLineCounter++)[1]);

		return true;
	}
	catch ( const std::exception& e )
	{
		std::cout << std::string(std::string("config loading error(please check, or delete your config file): ") + e.what(), 1);
		return false;
	}
}

struct HoningResult
{
	//starts at 1 for the first try and not at zero
	int tryNumber = 0;
	//the honing percent points (0-100) only for this step
	float honingRate = -1.0f;
	//the honing cost only for this step
	float honingCost = -1.0f;
	//the base honing rate without solar blessings (but increased from failing)
	float baseHoningRate = 0.0f;
	const float artisansEnergy = 0.0f;
	const int solarGraceUsed = 0;
	const int solarBlessingUsed = 0;
	const int solarProtectionUsed = 0;

	HoningResult(int solarGraceUsed, int solarBlessingUsed, int solarProtectionUsed)
		: solarGraceUsed(solarGraceUsed), solarBlessingUsed(solarBlessingUsed), solarProtectionUsed(solarProtectionUsed)
	{

	}

	HoningResult(int tryNumber, float newArtisantsEnergy, int solarGraceUsed, int solarBlessingUsed, int solarProtectionUsed)
		: tryNumber(tryNumber), artisansEnergy(newArtisantsEnergy), solarGraceUsed(solarGraceUsed), solarBlessingUsed(solarBlessingUsed), solarProtectionUsed(solarProtectionUsed)
	{

	}

	//returns the correct honing cost, also for the last element with tryNumber==-1 
	inline float getHoningCost(float& honingCostMultiplier) const
	{
		if ( tryNumber == -1 )
		{
			float cost = 0.0f;
			float nextArtisansEnergy = artisansEnergy;
			for ( float currentHoningRate = honingRate; currentHoningRate < 99.99f; currentHoningRate += 1.0f )
			{
				cost += honingCost * honingCostMultiplier;
				honingCostMultiplier *= 1.0f - currentHoningRate / 100.0f;
				if ( nextArtisansEnergy > 99.499f )
				{
					break;
				}
				nextArtisansEnergy += ARTISANS_ENERGY_MULTIPLIER * currentHoningRate;
			}
			return cost;
		}
		return honingCost * honingCostMultiplier;
	}

	//returns the correct average tries, also for the last element with tryNumber==-1 
	inline float getAverageTries(float& tryMultiplier) const
	{
		if ( tryNumber == -1 )
		{
			float tries = 0.0f;
			float nextArtisansEnergy = artisansEnergy;
			for ( float currentHoningRate = honingRate; currentHoningRate < 99.99f; currentHoningRate += 1.0f )
			{
				tries += tryMultiplier;
				tryMultiplier *= 1.0f - honingRate / 100.0f;
				if ( nextArtisansEnergy > 99.499f )
				{
					break;
				}
				nextArtisansEnergy += ARTISANS_ENERGY_MULTIPLIER * currentHoningRate;
			}
			return tries;
		}
		return tryMultiplier;
	}

	inline void output()
	{
		if ( tryNumber == -1 )
			std::cout << "The remaining Honing tries";
		else
			std::cout << "Honing try " << tryNumber;

		if ( tryNumber == -1 )
			std::cout << " (After Base Honing Rate " << baseHoningRate << "%) with ";
		else
			std::cout << " (Base Honing Rate " << baseHoningRate << "%) with ";

		int counter = 0;
		if ( solarGraceUsed > 0 )
		{
			std::cout << solarGraceUsed << " Solar Grace";
			counter++;
		}
		if ( solarBlessingUsed > 0 )
		{
			if ( counter > 0 )
				std::cout << ", ";
			std::cout << solarBlessingUsed << " Solar Blessing";
			counter++;
		}
		if ( solarProtectionUsed > 0 )
		{
			if ( counter > 0 )
				std::cout << ", ";
			std::cout << solarProtectionUsed << " Solar Protection";
			counter++;
		}
		if ( counter > 0 )
		{
			std::cout << " used and with ";
		}
		if ( tryNumber == -1 )
		{
			float averageTryMultiplier = 1.0f;
			std::cout << getAverageTries(averageTryMultiplier) << " more tries on average ";
		}

		if ( tryNumber != -1 )
		{
			std::cout << "a Success Rate of ";
			std::cout << honingRate << "% would cost " << honingCost;
		}
		else
		{
			float averageCostMultiplier = 1.0f;
			std::cout << "would cost " << getHoningCost(averageCostMultiplier);
		}


		if ( tryNumber == -1 )
			std::cout << " more gold on average.";
		else
			std::cout << " gold.";
		std::cout << std::endl;
	}

	inline HoningResult createNewParams(float newArtisansEnergy, int newSolarGraceAmount, int newSolarBlessingAmount, int newSolarProtectionAmount) const
	{
		return HoningResult(tryNumber + 1, newArtisansEnergy, newSolarGraceAmount, newSolarBlessingAmount, newSolarProtectionAmount);
	}

};


inline float getAverageCostOfHoningChain(const std::vector<HoningResult>& honingChain)
{
	float cost = 0.0f;
	float honingCostMultiplier = 1.0f;
	for ( const HoningResult& result : honingChain )
	{
		cost += result.getHoningCost(honingCostMultiplier);
		honingCostMultiplier *= 1.0f - result.honingRate / 100.0f;
	}
	return cost;
}
inline float getAverageTriesOfHoningChain(const std::vector<HoningResult>& honingChain)
{
	float tries = 0.0f;
	float tryMultiplier = 1.0f;
	for ( const HoningResult& result : honingChain )
	{
		tries += result.getAverageTries(tryMultiplier);
		tryMultiplier *= 1.0f - result.honingRate / 100.0f;
	}
	return tries;
}

//all success chances are in decimal point percent chance
struct HoningRates
{
	float successRateAsDecimal = 100;
	float solarGraceChance = 1.25f;
	float solarBlessingChance = 2.5f;
	float solarProtectionChance = 7.5f;
	int maxSolarGraceAmount = 12;
	int maxSolarBlessingAmount = 6;
	int maxSolarProtectionAmount = 2;
	//branches of the honing chain tree
	int maxAmountOfBranches = 12 * 6 * 2;
};

struct HoningChainElement
{
	HoningResult result;
	std::vector<HoningChainElement> elements;
	HoningChainElement(const HoningResult& result)
		: result(result)
	{

	}
	HoningChainElement(const HoningResult& result, const HoningChainElement& nextElement)
		: result(result)
	{
		elements.push_back(nextElement);
	}

	HoningChainElement(const HoningResult& result, int amountOfBranches)
		: result(result)
	{
		elements.reserve(amountOfBranches);
	}


	inline std::vector<HoningResult> getBestChain(std::vector<HoningResult> previousElements)
	{
		std::vector<HoningResult> bestHoningChain;
		float bestHoningCost = -1;

		if ( previousElements.capacity() <= previousElements.size() + 1 )
		{
			previousElements.reserve(previousElements.size() + 4);
		}

		previousElements.push_back(result);
		for ( HoningChainElement& branch : elements )
		{
			std::vector<HoningResult> bestSubChain = branch.getBestChain(previousElements);
			float honingCost = getAverageCostOfHoningChain(bestSubChain);
			if ( bestHoningCost < -0.99f || honingCost < bestHoningCost )
			{
				bestHoningCost = honingCost;
				bestHoningChain = std::move(bestSubChain);
			}
		}
		if ( bestHoningCost < -0.99f )
		{
			return previousElements;
		}
		return bestHoningChain;
	}

	inline std::vector<HoningResult> getBestChain()
	{
		std::vector<HoningResult> previousElements;
		return getBestChain(previousElements);
	}

};


//successRateAsDecimal is the percent chance for success
inline HoningRates getBaseHoningRatesForLevel(int targetItemHoningLevel)
{
	HoningRates result;
	float totalAdditionalMaterialSuccessRate = 60.00f;//the additional success chance in decimal percent points that is achievable with solar materials. so default 60%
	int maxSolarAmountMultiplier = 1;//with +12 and afterwards double the amount of solar pieces can be used, but each one will have a lower chance

	if ( targetItemHoningLevel >= 7 )
	{
		result.successRateAsDecimal -= 40;
	}
	if ( targetItemHoningLevel >= 8 )
	{
		result.successRateAsDecimal -= 15;
		totalAdditionalMaterialSuccessRate -= 15;
	}
	if ( targetItemHoningLevel >= 9 )
	{
		result.successRateAsDecimal -= 15;
		totalAdditionalMaterialSuccessRate -= 15;
	}
	if ( targetItemHoningLevel >= 12 )
	{
		maxSolarAmountMultiplier = 2;
		result.successRateAsDecimal -= 15;
		totalAdditionalMaterialSuccessRate -= 15;
	}
	if ( targetItemHoningLevel >= 15 )
	{
		result.successRateAsDecimal -= 5;
		totalAdditionalMaterialSuccessRate -= 5;
	}
	if ( targetItemHoningLevel >= 20 )
	{
		result.successRateAsDecimal -= 2;
		totalAdditionalMaterialSuccessRate -= 2;
	}

	result.maxSolarGraceAmount = MAX_SOLAR_GRACE_AMOUNT * maxSolarAmountMultiplier;
	result.maxSolarBlessingAmount = MAX_SOLAR_BLESSING_AMOUNT * maxSolarAmountMultiplier;
	result.maxSolarProtectionAmount = MAX_SOLAR_PROTECTION_AMOUNT * maxSolarAmountMultiplier;
	result.maxAmountOfBranches = result.maxSolarGraceAmount * result.maxSolarBlessingAmount * result.maxSolarProtectionAmount;
	//the total weighting of all solar stuff used together for their maximum success rate 
	float totalWeighting = result.maxSolarGraceAmount + result.maxSolarBlessingAmount * GRACE_TO_BLESSING_MULTIPLIER
		+ result.maxSolarProtectionAmount * GRACE_TO_BLESSING_MULTIPLIER * BLESSING_TO_PROTECTION_MULTIPLIER;
	result.solarGraceChance = 1.0f / totalWeighting * totalAdditionalMaterialSuccessRate;
	result.solarBlessingChance = 1.0f / totalWeighting * GRACE_TO_BLESSING_MULTIPLIER * totalAdditionalMaterialSuccessRate;
	result.solarProtectionChance = 1.0f / totalWeighting * GRACE_TO_BLESSING_MULTIPLIER * BLESSING_TO_PROTECTION_MULTIPLIER * totalAdditionalMaterialSuccessRate;
	return result;
}

inline float getBaseHoningCostWeapon(int targetItemHoningLevel, bool isIlvl1340Set)
{
	int upgradeStones = 138;
	int leapStones = 4;
	int fusionMat = 0;
	int rawGoldCost = 0;
	int silverCost = 16000;

	if ( targetItemHoningLevel >= 3 )
	{
		upgradeStones += 60;
		leapStones += 2;
		silverCost += 500;
	}
	if ( targetItemHoningLevel >= 4 )
	{
		fusionMat += 2;
		silverCost += 500;
	}
	if ( targetItemHoningLevel >= 7 )
	{
		upgradeStones += 60;
		leapStones += 2;
		fusionMat += 2;
		rawGoldCost += 120;
		silverCost += 1000;
	}
	if ( targetItemHoningLevel >= 10 )
	{
		upgradeStones += 62;
		leapStones += 2;
		silverCost += 2000;
	}
	if ( targetItemHoningLevel >= 13 )
	{
		upgradeStones += 60;
		fusionMat += 2;
		silverCost += 1000;
	}
	if ( targetItemHoningLevel >= 14 )
	{
		leapStones += 2;
		silverCost += 1000;
	}

	if ( isIlvl1340Set )
	{
		//todo: override values and calculate with other table: https://lost-ark.maxroll.gg/resources/gear-honing-system
		throw std::runtime_error("Item Level 1340 Set is not supported yet");
	}

	float goldCost = rawGoldCost + silverCost * ( 1.0f / static_cast<float>( silverAmountPerGold ) );
	if ( isIlvl1340Set )
		goldCost += leapStones * greaterLeapstoneP + fusionMat * basicFusionP;
	else
		goldCost += leapStones * leapstoneP + fusionMat * simpleFusionP;
	goldCost += upgradeStones * destructionStoneP;
	return goldCost;
}

inline float getBaseHoningCostArmour(int targetItemHoningLevel, bool isIlvl1340Set)
{
	int upgradeStones = 82;
	int leapStones = 2;
	int fusionMat = 0;
	int rawGoldCost = 0;
	int silverCost = 11000;

	if ( targetItemHoningLevel >= 3 )
	{
		leapStones += 2;
		silverCost += 500;
	}
	if ( targetItemHoningLevel >= 4 )
	{
		upgradeStones += 38;
		fusionMat += 2;
		silverCost += 500;
	}
	if ( targetItemHoningLevel >= 7 )
	{
		upgradeStones += 36;
		rawGoldCost += 70;
		silverCost += 500;
	}
	if ( targetItemHoningLevel >= 10 )
	{
		upgradeStones += 36;
		leapStones += 2;
		fusionMat += 2;
		silverCost += 1000;
	}
	if ( targetItemHoningLevel >= 13 )
	{
		upgradeStones += 36;
		silverCost += 1000;
	}
	if ( targetItemHoningLevel >= 14 )
	{
		leapStones += 2;
		silverCost += 1000;
	}

	if ( isIlvl1340Set )
	{
		//todo: override values and calculate with other table: https://lost-ark.maxroll.gg/resources/gear-honing-system
		throw std::runtime_error("Item Level 1340 Set is not supported yet");
	}

	float goldCost = rawGoldCost + silverCost * ( 1.0f / static_cast<float>( silverAmountPerGold ) );
	if ( isIlvl1340Set )
		goldCost += leapStones * greaterLeapstoneP + fusionMat * basicFusionP;
	else
		goldCost += leapStones * leapstoneP + fusionMat * simpleFusionP;
	goldCost += upgradeStones * guardianStoneP;
	return goldCost;
}

//IMPORTANT: the input (-1) will be replaced with the counter variable AND YOU SHOULD PUT EXACTLY ONE OF THE PARAMETER AS -1 
//startvalue should be 0 in first run and then 1
inline void iterateHelper(int startValue, int maxCount, int addToCounter, const std::function<void(int, int, int)>& function, int graceInput, int blessingInput, int protectionInput)
{
	for ( int counter = startValue; counter <= maxCount; counter+= addToCounter )
	{
		if ( graceInput == -1 )
			function(counter, blessingInput, protectionInput );
		else if( blessingInput == -1 )
			function( graceInput, counter, protectionInput );
		else if (protectionInput == -1 )
			function( graceInput, blessingInput, counter );
	}
}

//values from 0 to 2 in the order grace, blessing, protection
inline void getCounters(int& firstVarUsed, int& secondVarUsed, int& thirdVarUsed)
{
	bool isGraceCheaperThanBlessing = solarGraceP * GRACE_TO_BLESSING_MULTIPLIER < solarBlessingP;
	bool isGraceCheaperThanProtection = solarGraceP * GRACE_TO_BLESSING_MULTIPLIER * BLESSING_TO_PROTECTION_MULTIPLIER < solarProtectionP;
	bool isBlessingCheaperThanProtection = solarBlessingP * BLESSING_TO_PROTECTION_MULTIPLIER < solarProtectionP;
	if ( isGraceCheaperThanBlessing )
	{
		if ( isGraceCheaperThanProtection )
		{
			firstVarUsed = 0;
			if ( isBlessingCheaperThanProtection )
			{
				secondVarUsed = 1;
				thirdVarUsed = 2;
			}
			else
			{
				secondVarUsed = 2;
				thirdVarUsed = 1;
			}
		}
		else
		{
			firstVarUsed = 2;
			if ( isGraceCheaperThanBlessing )
			{
				secondVarUsed = 0;
				thirdVarUsed = 1;
			}
			else
			{
				secondVarUsed = 1;
				thirdVarUsed = 0;
			}
		}
	}
	else
	{
		if ( isBlessingCheaperThanProtection )
		{
			firstVarUsed = 1;
			if ( isGraceCheaperThanProtection )
			{
				secondVarUsed = 0;
				thirdVarUsed = 2;
			}
			else
			{
				secondVarUsed = 2;
				thirdVarUsed = 0;
			}
		}
		else
		{
			firstVarUsed = 2;
			if ( isBlessingCheaperThanProtection )
			{
				secondVarUsed = 0;
				thirdVarUsed = 2;
			}
			else
			{
				secondVarUsed = 2;
				thirdVarUsed = 0;
			}
		}
	}
}

//starts at 1 for the first try and not at zero
inline HoningChainElement honingChainStep(HoningResult resultParams, HoningRates baseHoningRates, float baseHoningCost, float additionalHoningChanceFromFailing)
{
	HoningResult& result = resultParams;
	const int& solarGraceAmountToUse = resultParams.solarGraceUsed;
	const int& solarBlessingAmountToUse = resultParams.solarBlessingUsed;
	const int& solarProtectionAmountToUse = resultParams.solarProtectionUsed;
	const float& previousArtisansEnergy = resultParams.artisansEnergy;
	result.baseHoningRate = baseHoningRates.successRateAsDecimal + additionalHoningChanceFromFailing;
	result.honingRate = result.baseHoningRate;

	float nextAdditionalHoningChanceFromFail = result.honingRate * 0.1f;
	if ( additionalHoningChanceFromFailing > 0.01f )
	{
		nextAdditionalHoningChanceFromFail = additionalHoningChanceFromFailing + 1.0f;
	}
	float additionalSolarHoningChance = solarGraceAmountToUse * baseHoningRates.solarGraceChance + solarBlessingAmountToUse * baseHoningRates.solarBlessingChance + solarProtectionAmountToUse * baseHoningRates.solarProtectionChance;
	if ( additionalSolarHoningChance > 34.01f )
	{//upper limit for honing materials 

		result.honingCost = 9999999999999.0f;
		return HoningChainElement(result);
	}
	result.honingRate += additionalSolarHoningChance;

	float nextArtisansEnergy = previousArtisansEnergy + ARTISANS_ENERGY_MULTIPLIER * result.honingRate;
	if ( previousArtisansEnergy > 99.499f )
		result.honingRate = 100.0f;

	result.honingCost = baseHoningCost + solarGraceAmountToUse * solarGraceP + solarBlessingAmountToUse * solarBlessingP + solarProtectionAmountToUse * solarProtectionP;

	if ( result.honingRate > 99.499f )
	{//last honing in the chain, because it's guaranteed
		return HoningChainElement(result);
	}

	if ( nextArtisansEnergy > 99.499f )
	{
		return HoningChainElement(result, honingChainStep(resultParams.createNewParams(nextArtisansEnergy, 0, 0, 0), baseHoningRates, baseHoningCost, nextAdditionalHoningChanceFromFail));
	}

	if ( resultParams.tryNumber == 6 )
	{
		resultParams.tryNumber = -1;
		return HoningChainElement(result);
	}

	HoningChainElement combinedHoningChain { result, baseHoningRates.maxAmountOfBranches };

	//combinedHoningChain.elements.push_back(honingChainStep(resultParams.createNewParams(nextArtisansEnergy, 0, 0, 0), baseHoningRates, baseHoningCost, nextAdditionalHoningChanceFromFail));


	auto createFunction  = [&] (int graceCounter, int blessingCounter, int protectionCounter)
	{
		combinedHoningChain.elements.push_back(honingChainStep(resultParams.createNewParams(nextArtisansEnergy, graceCounter, blessingCounter, protectionCounter), baseHoningRates, baseHoningCost, nextAdditionalHoningChanceFromFail));
	};

	
	static int firstVarUsed;//from 0 to 2 in the order grace, blessing, protection
	static int secondVarUsed;
	static int thirdVarUsed;
	getCounters(firstVarUsed, secondVarUsed, thirdVarUsed);

	static int maxAmountTable[] = { baseHoningRates.maxSolarGraceAmount,baseHoningRates.maxSolarBlessingAmount,baseHoningRates.maxSolarProtectionAmount };
	static int addToCounterTable[] = {3,2,1 };//the steps the counter moves for each solar thing
	static int maxSolarAmountMultiplier = baseHoningRates.maxSolarGraceAmount / MAX_SOLAR_GRACE_AMOUNT;

	int varTable[] = { 0,0,0 };


	varTable[firstVarUsed] = -1;
	iterateHelper(0, maxAmountTable[firstVarUsed], addToCounterTable[firstVarUsed] * maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[firstVarUsed] = maxAmountTable[firstVarUsed];
	varTable[secondVarUsed] = -1;
	iterateHelper(1, maxAmountTable[secondVarUsed], addToCounterTable[secondVarUsed] * maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[secondVarUsed] = maxAmountTable[secondVarUsed];
	varTable[thirdVarUsed] = -1;
	iterateHelper(1, maxAmountTable[thirdVarUsed], addToCounterTable[thirdVarUsed] * maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);

	return combinedHoningChain;
}

//starts at 1 for the first try and not at zero
inline std::vector<HoningResult> startHoningChain(HoningRates baseHoningRates, float baseHoningCost)
{
	std::vector<HoningResult> bestHoningChain;
	float bestHoningCost = -1.0f;


	auto createFunction = [&] (int graceCounter, int blessingCounter, int protectionCounter)
	{
		HoningChainElement chainElement = honingChainStep(HoningResult(graceCounter, blessingCounter, protectionCounter), baseHoningRates, baseHoningCost, 0.0f);
		std::vector<HoningResult> bestChainAsVector = chainElement.getBestChain();
		float honingCost = getAverageCostOfHoningChain(bestChainAsVector);
		if ( bestHoningCost < -0.99f || honingCost < bestHoningCost )
		{
			bestHoningCost = honingCost;
			bestHoningChain = std::move(bestChainAsVector);
		}
	};

	static int firstVarUsed;//from 0 to 2 in the order grace, blessing, protection
	static int secondVarUsed;
	static int thirdVarUsed;
	getCounters(firstVarUsed, secondVarUsed, thirdVarUsed);

	static int maxAmountTable[] = { baseHoningRates.maxSolarGraceAmount,baseHoningRates.maxSolarBlessingAmount,baseHoningRates.maxSolarProtectionAmount };
	static int addToCounterTable[] = { 3,2,1 };//the steps the counter moves for each solar thing
	static int maxSolarAmountMultiplier = baseHoningRates.maxSolarGraceAmount / MAX_SOLAR_GRACE_AMOUNT;

	int varTable[] = { 0,0,0 };
	std::cout << "Calculating 1/3... " << std::endl;
	varTable[firstVarUsed] = -1;
	iterateHelper(0, maxAmountTable[firstVarUsed], addToCounterTable[firstVarUsed] * maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[firstVarUsed] = maxAmountTable[firstVarUsed];
	varTable[secondVarUsed] = -1;
	std::cout << "Calculating 2/3... " << std::endl;
	iterateHelper(1, maxAmountTable[secondVarUsed], addToCounterTable[secondVarUsed] * maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[secondVarUsed] = maxAmountTable[secondVarUsed];
	varTable[thirdVarUsed] = -1;
	std::cout << "Calculating 3/3... " << std::endl;
	iterateHelper(1, maxAmountTable[thirdVarUsed], addToCounterTable[thirdVarUsed] * maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);

	return bestHoningChain;
}


inline void calculateHoningForLevel(int currentItemHoningLevel, bool isIlvl1340Set)
{
	HoningRates baseHoningRates = getBaseHoningRatesForLevel(currentItemHoningLevel + 1);
	float baseHoningCostWeapon = getBaseHoningCostWeapon(currentItemHoningLevel + 1, isIlvl1340Set);
	float baseHoningCostArmour = getBaseHoningCostArmour(currentItemHoningLevel + 1, isIlvl1340Set);

	std::cout << "Calculating Weapon Honing cost." << std::endl;
	auto honingChainWeapon = startHoningChain(baseHoningRates, baseHoningCostWeapon);
	float avgGoldCostWeapon = getAverageCostOfHoningChain(honingChainWeapon);

	std::cout << "Calculating Armour Honing cost." << std::endl;
	auto honingChainArmour= startHoningChain(baseHoningRates, baseHoningCostArmour);
	float avgGoldCostArmour = getAverageCostOfHoningChain(honingChainArmour);


	std::cout << std::endl << std::endl << std::endl << "Honing Calculation for your Gear from +" << currentItemHoningLevel << " to +" << currentItemHoningLevel + 1 << std::endl << std::endl << std::endl;

	std::cout << std::endl << "Your Weapon has an average cost of " << avgGoldCostWeapon << " gold and needs on average " << getAverageTriesOfHoningChain(honingChainWeapon) << " tries." << std::endl << std::endl;

	for ( auto& element : honingChainWeapon )
	{
		element.output();
	}
	

	std::cout << std::endl << "Your Armour has an average cost of " << avgGoldCostArmour << " gold and needs on average " << getAverageTriesOfHoningChain(honingChainArmour) <<" tries." << std::endl << std::endl;

	for ( auto& element : honingChainArmour )
	{
		element.output();
	}

	//double finalGoldCost = ( 1 / ( totalSuccessRate / 100 ) ) * tempGoldCost;
}

inline void printMarysShop()
{
	std::cout << std::endl << "Printing how much cheaper the following Materials would be from Marys Shop" << std::endl;
	std::cout << std::endl << "Negative Numbers mean that it's more expensive!" << std::endl;

	std::cout << std::endl << "Honor Leapstone: " <<  ( 1.0f - (( maHonorLeapstoneC * crystalPrice) / maHonorLeapstoneA) / leapstoneP) * 100.0f << "% cheaper" << std::endl;
	std::cout << std::endl << "Greater Honor Leapstone: " <<  ( 1.0f - (( maGreaterHonorLeapstoneC * crystalPrice) / maGreaterHonorLeapstoneA) / greaterLeapstoneP) * 100.0f << "% cheaper" << std::endl;

	std::cout << std::endl << "Simple Fusion Material: " <<  ( 1.0f - (( maSimpleFusionC * crystalPrice) / maSimpleFusionA) / simpleFusionP) * 100.0f << "% cheaper" << std::endl;
	std::cout << std::endl << "Basic Fusion Material: " <<  ( 1.0f - (( maBasicFusionC * crystalPrice) / maBasicFusionA) / basicFusionP) * 100.0f << "% cheaper" << std::endl;

	std::cout << std::endl << "Destruction Stone Crystal: " <<  ( 1.0f - (( maDestructionStoneC * crystalPrice) / maDestructionStoneA) / destructionStoneP) * 100.0f << "% cheaper" << std::endl;
	std::cout << std::endl << "Guardian Stone Crystal: " <<  ( 1.0f - (( maGuardianStoneC * crystalPrice) / maGuardianStoneA) / guardianStoneP) * 100.0f << "% cheaper" << std::endl;

	std::cout << std::endl << "Solar Grace: " <<  ( 1.0f - (( maGraceC * crystalPrice) / maGraceA) / solarGraceP) * 100.0f << "% cheaper" << std::endl;
	std::cout << std::endl << "Solar Blessing: " <<  ( 1.0f - (( maBlessingC * crystalPrice) / maBlessingeA) / solarBlessingP) * 100.0f << "% cheaper" << std::endl;
	std::cout << std::endl << "Solar Protection: " <<  ( 1.0f - (( maProtectionC * crystalPrice) / maProtectionA) / solarProtectionP) * 100.0f << "% cheaper" << std::endl;
	
	std::cout << std::endl << std::endl;
}

static void lostArkHoning()
{
	try
	{
		std::cout << std::endl << "Started T3 Lost Ark Calculator" << std::endl;
		std::cout << std::endl << "This will show you the most effecient way to hone your gear including artisans energy and everything" << std::endl << std::endl;
		std::cout << std::endl << "After +12 The Program can take a few minutes to calculate and needs 8 GB of free RAM!" << std::endl << std::endl;
		if ( !parseConfig() )
			return;

		printMarysShop();

		calculateHoningForLevel(itemHoningLevel, isIlvl1340Set);

	}
	catch ( std::exception e )
	{
		std::cout << "Error running Lost Ark Calculator: " << e.what() << std::endl;
	}
}



