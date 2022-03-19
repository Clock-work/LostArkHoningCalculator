#pragma once
#include "HoningParameter.h"
#include "HoningInput.h"
#include "HoningResult.h"
#include "HoningChain.h"
#include <functional>

enum SolarToUse : char
{
	GRACE, BLESSING, PROTECTION
};

//signals that this parameter should iterate through the function with a for loop
const static char ITERATE_COUNTER = -1;

//startvalue, maxcount and addtocounter are for the steps of the for loop that calls the function 
//the inputs will be passed to the function as parameter, but the one with the value of ITERATE_COUNTER will be iterated through in the for loop
//the iterating will operate with "counter<=maxCount" if addToCounter is positive and otherwise with "counter>=maxCount"
inline void iterateFunction(char startValue, char maxCount, char addToCounter, const std::function<void(char, char, char)>& function, char graceInput, char blessingInput, char protectionInput)
{
	if ( addToCounter > 0 )
	{
		for ( char counter = startValue; counter <= maxCount; )
		{
			if ( graceInput == ITERATE_COUNTER )
				function(counter, blessingInput, protectionInput);
			else if ( blessingInput == ITERATE_COUNTER )
				function(graceInput, counter, protectionInput);
			else if ( protectionInput == ITERATE_COUNTER )
				function(graceInput, blessingInput, counter);
			counter += addToCounter;
			if ( counter > maxCount && counter < maxCount + addToCounter )
			{
				counter = maxCount;//dont skip last step
			}
		}
	}
	else
	{
		for ( char counter = startValue; counter >= maxCount; )
		{
			if ( graceInput == ITERATE_COUNTER )
				function(counter, blessingInput, protectionInput);
			else if ( blessingInput == ITERATE_COUNTER )
				function(graceInput, counter, protectionInput);
			else if ( protectionInput == ITERATE_COUNTER )
				function(graceInput, blessingInput, counter);
			counter += addToCounter;
			if ( counter < maxCount && counter > maxCount + addToCounter )
			{
				counter = maxCount;//dont skip last step
			}
		}
	}
}

//which solar material should be iterated through first 
inline void getCheapestOrder(SolarToUse& firstVarUsed, SolarToUse& secondVarUsed, SolarToUse& thirdVarUsed)
{
	bool isGraceCheaperThanBlessing = getSolarGraceCost() * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER < getSolarBlessingCost();
	bool isGraceCheaperThanProtection = getSolarGraceCost() * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER * HoningConfig::BLESSING_TO_PROTECTION_MULTIPLIER < getSolarProtectionCost();
	bool isBlessingCheaperThanProtection = getSolarBlessingCost() * HoningConfig::BLESSING_TO_PROTECTION_MULTIPLIER < getSolarProtectionCost();
	if ( isGraceCheaperThanBlessing )
	{
		if ( isGraceCheaperThanProtection )
		{
			firstVarUsed = SolarToUse::GRACE;
			if ( isBlessingCheaperThanProtection )
			{
				secondVarUsed = SolarToUse::BLESSING;
				thirdVarUsed = SolarToUse::PROTECTION;
			}
			else
			{
				secondVarUsed = SolarToUse::PROTECTION;
				thirdVarUsed = SolarToUse::BLESSING;
			}
		}
		else
		{
			firstVarUsed = SolarToUse::PROTECTION;
			if ( isGraceCheaperThanBlessing )
			{
				secondVarUsed = SolarToUse::GRACE;
				thirdVarUsed = SolarToUse::BLESSING;
			}
			else
			{
				secondVarUsed = SolarToUse::BLESSING;
				thirdVarUsed = SolarToUse::GRACE;
			}
		}
	}
	else
	{
		if ( isBlessingCheaperThanProtection )
		{
			firstVarUsed = SolarToUse::BLESSING;
			if ( isGraceCheaperThanProtection )
			{
				secondVarUsed = SolarToUse::GRACE;
				thirdVarUsed = SolarToUse::PROTECTION;
			}
			else
			{
				secondVarUsed = SolarToUse::PROTECTION;
				thirdVarUsed = SolarToUse::GRACE;
			}
		}
		else
		{
			firstVarUsed = SolarToUse::PROTECTION;
			thirdVarUsed = SolarToUse::BLESSING;
			secondVarUsed = SolarToUse::GRACE;
		}
	}
}

//calls the createfunction with the parameter configuration. below is an example for the parameter to control the iterating
/*
	//the start amount of each solar material to use IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char startSolarAmountTable[] = { 0, 0, 0 };
	//the maximum amount of each solar material to use (always the end of the for loop) IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char maxSolarAmountTable[] = { honingParameter.getMaxSolarGraceAmount(), honingParameter.getMaxSolarBlessingAmount(), honingParameter.getMaxSolarProtectionAmount() };
	//the steps the counter moves for each solar thing IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char addToCounterTable[] = { 1 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier };
*/
inline void startNextChains(const std::function<void(int, int, int)>& createFunction, const HoningParameter& honingParameter, char* startSolarAmountTable, char* maxSolarAmountTable, char* addToCounterTable, bool logSteps)
{
	SolarToUse firstVarUsed;
	SolarToUse secondVarUsed;
	SolarToUse thirdVarUsed;
	getCheapestOrder(firstVarUsed, secondVarUsed, thirdVarUsed);

	//the input of the solar parameter (and the one which is ITERATE_COUNTER will iterate through the for loop) IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char solarInput[] = { 0, 0, 0 };
	solarInput[GRACE] = startSolarAmountTable[GRACE];
	solarInput[BLESSING] = startSolarAmountTable[BLESSING];
	solarInput[PROTECTION] = startSolarAmountTable[PROTECTION];

	bool startAtZero = addToCounterTable[0] > 0;
	char addToStartValue = 0;
	if ( startAtZero )
		addToStartValue++;
	else
		addToStartValue--;

	if(logSteps )
		std::cout << "Calculating 1/3... " << std::endl;
	solarInput[firstVarUsed] = ITERATE_COUNTER;
	//iterate from 0 0 0 to n 0 0
	iterateFunction(startSolarAmountTable[(char) firstVarUsed], maxSolarAmountTable[(char)firstVarUsed], addToCounterTable[firstVarUsed], createFunction, solarInput[GRACE], solarInput[BLESSING], solarInput[PROTECTION]);
	solarInput[firstVarUsed] = maxSolarAmountTable[(char) firstVarUsed];
	solarInput[secondVarUsed] = ITERATE_COUNTER;
	if ( logSteps )
		std::cout << "Calculating 2/3... " << std::endl;
	//iterate from max 1 0 to max n 0
	iterateFunction(startSolarAmountTable[(char) secondVarUsed] + addToStartValue, maxSolarAmountTable[(char) secondVarUsed], addToCounterTable[secondVarUsed], createFunction, solarInput[GRACE], solarInput[BLESSING], solarInput[PROTECTION]);
	solarInput[secondVarUsed] = maxSolarAmountTable[(char) secondVarUsed];
	solarInput[thirdVarUsed] = ITERATE_COUNTER;
	if ( logSteps )
		std::cout << "Calculating 3/3... " << std::endl;
	//iterate from max max 1 to max max n
	iterateFunction(startSolarAmountTable[(char) thirdVarUsed] + addToStartValue, maxSolarAmountTable[(char) thirdVarUsed], addToCounterTable[thirdVarUsed], createFunction, solarInput[GRACE], solarInput[BLESSING], solarInput[PROTECTION]);
	if ( logSteps )
		std::cout << "Calculating Done!" << std::endl;
}

//starts at 1 for the first try and not at zero
inline HoningChainElement honingChainStep(const HoningParameter& honingParameter, bool isWeapon, const HoningInput& honingInput)
{
	HoningResult result { honingInput };

	if ( isMore(honingInput.getAdditionalSolarSuccessRate(honingParameter), HoningConfig::MAX_ADDITIONAL_SOLAR_CHANCE) )
	{//upper limit for solar honing materials used to increase the chance of honing 
		return HoningChainElement(result);
	}

	if ( isMore(honingInput.getTotalHoningSuccessRate(honingParameter), 99.9f) )
	{//last honing in the chain, because it's guaranteed
		return HoningChainElement(result);
	}

	if ( isMore(honingInput.getNextArtisansEnergy(honingParameter), 99.9f) )
	{
		return HoningChainElement(result, honingChainStep(honingParameter, isWeapon, honingInput.getHoningInputForNextTry(honingParameter, 0, 0, 0)));
	}


	//the start amount of each solar material to use IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char startSolarAmountTable[] = { honingInput.solarGraceToUse, honingInput.solarBlessingToUse, honingInput.solarProtectionToUse };
	//the maximum amount of each solar material to use (always the end of the for loop) IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char maxSolarAmountTable[] = { 0, 0, 0 };
	//the steps the counter moves for each solar thing IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char addToCounterTableVariant1[] = { -1 , -1, -1 };
	char addToCounterTableVariant2[] = { -6 * honingParameter.maxSolarAmountMultiplier, -3 * honingParameter.maxSolarAmountMultiplier, -1 * honingParameter.maxSolarAmountMultiplier };
	char* addToCounterTable = addToCounterTableVariant1;
	if ( honingInput.getNextArtisansEnergy(honingParameter) < 50.0f )
	{
		addToCounterTable = addToCounterTableVariant2;
	}


	//the amount of combinations that will be used
	short amountOfBranches = 1;

	if ( std::abs(startSolarAmountTable[GRACE]) > 1 )
	{
		amountOfBranches += startSolarAmountTable[GRACE] / std::abs((float) addToCounterTable[GRACE]) + 1;
	}
	else
	{
		amountOfBranches += startSolarAmountTable[GRACE];
	}
	if ( std::abs(startSolarAmountTable[BLESSING]) > 1 )
	{
		amountOfBranches += startSolarAmountTable[BLESSING] / std::abs((float) addToCounterTable[BLESSING]) + 1;
	}
	else
	{
		amountOfBranches += startSolarAmountTable[BLESSING];
	}
	if ( std::abs(startSolarAmountTable[PROTECTION]) > 1 )
	{
		amountOfBranches += startSolarAmountTable[PROTECTION] / std::abs((float) addToCounterTable[PROTECTION]) + 1;
	}
	else
	{
		amountOfBranches += startSolarAmountTable[PROTECTION];
	}

	HoningChainElement combinedHoningChain { result, amountOfBranches };

	auto createFunction = [&] (char graceCounter, char blessingCounter, char protectionCounter)
	{
		combinedHoningChain.elements.push_back(honingChainStep(honingParameter, isWeapon, honingInput.getHoningInputForNextTry(honingParameter, graceCounter, blessingCounter, protectionCounter)));
	};


	startNextChains(createFunction, honingParameter, startSolarAmountTable, maxSolarAmountTable, addToCounterTable, false);
	return combinedHoningChain;

}

//starts at 1 for the first try and not at zero
inline std::vector<HoningResult> startHoningChain(const HoningParameter& honingParameter, bool isWeapon)
{
	std::vector<HoningResult> bestHoningChain;
	float bestHoningCost = -1.0f;

	//the start amount of each solar material to use IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char startSolarAmountTable[] = { 0, 0, 0 };
	//the maximum amount of each solar material to use (always the end of the for loop) IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char maxSolarAmountTable[] = { honingParameter.getMaxSolarGraceAmount(), honingParameter.getMaxSolarBlessingAmount(), honingParameter.getMaxSolarProtectionAmount() };
	//the steps the counter moves for each solar thing IN ORDER OF THE SOLAR TYPES grace, blessing, protection
	char addToCounterTable[] = { 6 * honingParameter.maxSolarAmountMultiplier, 3 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier };

	auto createFunction = [&] (char graceCounter, char blessingCounter, char protectionCounter)
	{
		HoningChainElement chainElement = honingChainStep(honingParameter, isWeapon, HoningInput(1, 0.0f, graceCounter, blessingCounter, protectionCounter));
		std::vector<HoningResult> bestChainAsVector = chainElement.getBestChain(honingParameter, isWeapon);
		float honingCost = getAverageCostOfHoningChain(bestChainAsVector, honingParameter, isWeapon);
		if ( isless(bestHoningCost,-0.99f) || isLess(honingCost, bestHoningCost) )
		{
			bestHoningCost = honingCost;
			bestHoningChain = std::move(bestChainAsVector);
		}
	};

	startNextChains(createFunction, honingParameter, startSolarAmountTable, maxSolarAmountTable, addToCounterTable, true);
	return bestHoningChain;
}

inline void calculateHoningForLevel(int currentItemHoningLevel, bool isIlvl1340Set)
{
	HoningParameter honingParameter { currentItemHoningLevel + 1, isIlvl1340Set};

	std::cout << "Calculating Weapon Honing cost." << std::endl;
	auto honingChainWeapon = startHoningChain(honingParameter, true);
	float avgGoldCostWeapon = getAverageCostOfHoningChain(honingChainWeapon, honingParameter, true);

	std::cout << "Calculating Armour Honing cost." << std::endl;
	auto honingChainArmour= startHoningChain(honingParameter, false);
	float avgGoldCostArmour = getAverageCostOfHoningChain(honingChainArmour, honingParameter, false);


	std::cout << std::endl << "Honing Calculation for your Gear from +" << currentItemHoningLevel << " to +" << currentItemHoningLevel + 1 << std::endl << std::endl;

	if ( HoningConfig::useMarysShopPrices )
		std::cout << "Marys shop prices will also be used for this calculation (you can change this in the config)" << std::endl << std::endl;
	else
		std::cout << "Marys shop prices will not be used for this calculation (you can change this in the config)" << std::endl << std::endl;


	std::cout << std::endl << "Your Weapon has an average cost of " << avgGoldCostWeapon << " gold and needs on average " << getAverageTriesOfHoningChain(honingChainWeapon, honingParameter) << " tries." << std::endl << std::endl;

	for ( auto& element : honingChainWeapon )
	{
		element.output(honingParameter, true);
	}
	

	std::cout << std::endl << "Your Armour has an average cost of " << avgGoldCostArmour << " gold and needs on average " << getAverageTriesOfHoningChain(honingChainArmour, honingParameter) <<" tries." << std::endl << std::endl;

	for ( auto& element : honingChainArmour )
	{
		element.output(honingParameter, false);
	}

	//double finalGoldCost = ( 1 / ( totalSuccessRate / 100 ) ) * tempGoldCost;
}



static void printHoningCalculation()
{
	calculateHoningForLevel(HoningConfig::itemHoningLevel, HoningConfig::isIlvl1340Set);
}



