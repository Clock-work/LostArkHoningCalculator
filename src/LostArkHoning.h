#pragma once
#include "HoningParameter.h"
#include "HoningInput.h"
#include "HoningResult.h"
#include "HoningChain.h"
#include <functional>


//IMPORTANT: the input (-1) will be replaced with the counter variable AND YOU SHOULD PUT EXACTLY ONE OF THE PARAMETER AS -1 
//startvalue should be 0 in first run and then 1
inline void iterateHelper(char startValue, char maxCount, char addToCounter, const std::function<void(char, char, char)>& function, char graceInput, char blessingInput, char protectionInput)
{
	for ( char counter = startValue; counter <= maxCount; counter+= addToCounter )
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
inline void getCounters(char& firstVarUsed, char& secondVarUsed, char& thirdVarUsed)
{
	bool isGraceCheaperThanBlessing = MarketPrices::solarGrace * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER < MarketPrices::solarBlessing;
	bool isGraceCheaperThanProtection = MarketPrices::solarGrace * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER * HoningConfig::BLESSING_TO_PROTECTION_MULTIPLIER < MarketPrices::solarProtection;
	bool isBlessingCheaperThanProtection = MarketPrices::solarBlessing * HoningConfig::BLESSING_TO_PROTECTION_MULTIPLIER < MarketPrices::solarProtection;
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

//IMPORTANT: the input (-1) will be replaced with the counter variable AND YOU SHOULD PUT EXACTLY ONE OF THE PARAMETER AS -1 
//startvalue should be 0 in first run and then 1
inline void startNextChains(const std::function<void(int, int, int)>& createFunction, const HoningParameter& honingParameter, bool logSteps)
{
	static char firstVarUsed;//from 0 to 2 in the order grace, blessing, protection
	static char secondVarUsed;
	static char thirdVarUsed;
	getCounters(firstVarUsed, secondVarUsed, thirdVarUsed);

	static char maxAmountTable[] = { honingParameter.getMaxSolarGraceAmount(), honingParameter.getMaxSolarBlessingAmount(), honingParameter.getMaxSolarProtectionAmount() };
	static char addToCounterTable[] = { 3,2,1 };//the steps the counter moves for each solar thing

	char varTable[] = { 0,0,0 };
	if(logSteps )
		std::cout << "Calculating 1/3... " << std::endl;
	varTable[firstVarUsed] = -1;
	iterateHelper(0, maxAmountTable[firstVarUsed], addToCounterTable[firstVarUsed] * honingParameter.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[firstVarUsed] = maxAmountTable[firstVarUsed];
	varTable[secondVarUsed] = -1;
	if ( logSteps )
		std::cout << "Calculating 2/3... " << std::endl;
	iterateHelper(1, maxAmountTable[secondVarUsed], addToCounterTable[secondVarUsed] * honingParameter.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[secondVarUsed] = maxAmountTable[secondVarUsed];
	varTable[thirdVarUsed] = -1;
	if ( logSteps )
		std::cout << "Calculating 3/3... " << std::endl;
	iterateHelper(1, maxAmountTable[thirdVarUsed], addToCounterTable[thirdVarUsed] * honingParameter.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	if ( logSteps )
		std::cout << "Calculating Done!" << std::endl;
}

//starts at 1 for the first try and not at zero
inline HoningChainElement honingChainStep(const HoningParameter& honingParameter, bool isWeapon, const HoningInput& honingInput)
{
	HoningResult result { honingInput };

	if ( isMore(honingInput.getAdditionalSolarSuccessRate(honingParameter), HoningConfig::MAX_ADDITIONAL_SOLAR_CHANCE) )
	{//upper limit for solar honing materials used to increase the chance of honing 

		result.tryNumber = -2.0;
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

	if ( honingInput.tryNumber == HoningConfig::MAX_HONING_TRIES )
	{//upper limit for amount of honing tries, or recursion depth 
		result.tryNumber = -1;
		return HoningChainElement(result);
	}

	HoningChainElement combinedHoningChain { result, honingParameter.getMaxSolarGraceAmount() * honingParameter.getMaxSolarBlessingAmount() * honingParameter.getMaxSolarProtectionAmount() };

	//combinedHoningChain.elements.push_back(honingChainStep(honingParameter, isWeapon, honingInput.getHoningInputForNextTry(honingParameter, 0, 0, 0)));


	auto createFunction = [&] (char graceCounter, char blessingCounter, char protectionCounter)
	{
		combinedHoningChain.elements.push_back(honingChainStep(honingParameter, isWeapon, honingInput.getHoningInputForNextTry(honingParameter, graceCounter, blessingCounter, protectionCounter)));
	};


	startNextChains(createFunction, honingParameter, false);
	return combinedHoningChain;

}

//starts at 1 for the first try and not at zero
inline std::vector<HoningResult> startHoningChain(const HoningParameter& honingParameter, bool isWeapon)
{
	std::vector<HoningResult> bestHoningChain;
	float bestHoningCost = -1.0f;


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

	startNextChains(createFunction, honingParameter, true);
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


	std::cout << std::endl << std::endl << std::endl << "Honing Calculation for your Gear from +" << currentItemHoningLevel << " to +" << currentItemHoningLevel + 1 << std::endl << std::endl << std::endl;

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



