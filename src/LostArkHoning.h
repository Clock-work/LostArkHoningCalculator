#pragma once
#include "HoningParameter.h"
#include <functional>

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
		: tryNumber(1), solarGraceUsed(solarGraceUsed), solarBlessingUsed(solarBlessingUsed), solarProtectionUsed(solarProtectionUsed)
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
			int tryNumber = HoningConfig::MAX_HONING_TRIES;
			for ( float currentHoningRate = honingRate; currentHoningRate < 99.99f; currentHoningRate += 1.0f )
			{
				cost += honingCost * honingCostMultiplier;
				honingCostMultiplier *= 1.0f - currentHoningRate / 100.0f;
				if ( nextArtisansEnergy > 99.499f )
				{
					break;
				}
				nextArtisansEnergy += HoningConfig::ARTISANS_ENERGY_MULTIPLIER * currentHoningRate;
				tryNumber++;
				if ( tryNumber > 10 )
					currentHoningRate -= 1.0f;
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
			int tryNumber = HoningConfig::MAX_HONING_TRIES;
			for ( float currentHoningRate = honingRate; currentHoningRate < 99.99f; currentHoningRate += 1.0f )
			{
				tries += tryMultiplier;
				tryMultiplier *= 1.0f - currentHoningRate / 100.0f;
				if ( nextArtisansEnergy > 99.499f )
				{
					break;
				}
				nextArtisansEnergy += HoningConfig::ARTISANS_ENERGY_MULTIPLIER * currentHoningRate;
				tryNumber++;
				if ( tryNumber > 10 )
					currentHoningRate -= 1.0f;
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

//starts at 1 for the first try and not at zero
inline HoningChainElement honingChainStep(HoningResult resultParams, const HoningParameter& baseHoningRates, bool isWeapon, float additionalHoningChanceFromFailing)
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
	if ( resultParams.tryNumber > 10 ) // no longer additional base chance after 10 failed tries 
	{
		nextAdditionalHoningChanceFromFail = additionalHoningChanceFromFailing;
	}
	float additionalSolarHoningChance = solarGraceAmountToUse * baseHoningRates.solarGraceChance + solarBlessingAmountToUse * baseHoningRates.solarBlessingChance + solarProtectionAmountToUse * baseHoningRates.solarProtectionChance;
	if ( additionalSolarHoningChance > 34.01f )
	{//upper limit for honing materials 

		result.honingCost = 9999999999999.0f;
		return HoningChainElement(result);
	}
	result.honingRate += additionalSolarHoningChance;

	float nextArtisansEnergy = previousArtisansEnergy + HoningConfig::ARTISANS_ENERGY_MULTIPLIER * result.honingRate;
	if ( previousArtisansEnergy > 99.499f )
		result.honingRate = 100.0f;

	result.honingCost = baseHoningRates.getBaseHoningCost(isWeapon) + solarGraceAmountToUse * MarketPrices::solarGrace 
		+ solarBlessingAmountToUse * MarketPrices::solarBlessing + solarProtectionAmountToUse * MarketPrices::solarProtection;

	if ( result.honingRate > 99.499f )
	{//last honing in the chain, because it's guaranteed
		return HoningChainElement(result);
	}

	if ( nextArtisansEnergy > 99.499f )
	{
		return HoningChainElement(result, honingChainStep(resultParams.createNewParams(nextArtisansEnergy, 0, 0, 0), baseHoningRates, isWeapon, nextAdditionalHoningChanceFromFail));
	}

	if ( resultParams.tryNumber == HoningConfig::MAX_HONING_TRIES )
	{
		resultParams.tryNumber = -1;
		return HoningChainElement(result);
	}

	HoningChainElement combinedHoningChain { result, baseHoningRates.getMaxSolarGraceAmount() * baseHoningRates.getMaxSolarBlessingAmount() * baseHoningRates.getMaxSolarProtectionAmount() };

	//combinedHoningChain.elements.push_back(honingChainStep(resultParams.createNewParams(nextArtisansEnergy, 0, 0, 0), baseHoningRates, isWeapon, nextAdditionalHoningChanceFromFail));


	auto createFunction  = [&] (int graceCounter, int blessingCounter, int protectionCounter)
	{
		combinedHoningChain.elements.push_back(honingChainStep(resultParams.createNewParams(nextArtisansEnergy, graceCounter, blessingCounter, protectionCounter), baseHoningRates, isWeapon, nextAdditionalHoningChanceFromFail));
	};

	
	static int firstVarUsed;//from 0 to 2 in the order grace, blessing, protection
	static int secondVarUsed;
	static int thirdVarUsed;
	getCounters(firstVarUsed, secondVarUsed, thirdVarUsed);

	static int maxAmountTable[] = { baseHoningRates.getMaxSolarGraceAmount(),baseHoningRates.getMaxSolarBlessingAmount(),baseHoningRates.getMaxSolarProtectionAmount() };
	static int addToCounterTable[] = {3,2,1 };//the steps the counter moves for each solar thing

	int varTable[] = { 0,0,0 };


	varTable[firstVarUsed] = -1;
	iterateHelper(0, maxAmountTable[firstVarUsed], addToCounterTable[firstVarUsed] * baseHoningRates.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[firstVarUsed] = maxAmountTable[firstVarUsed];
	varTable[secondVarUsed] = -1;
	iterateHelper(1, maxAmountTable[secondVarUsed], addToCounterTable[secondVarUsed] * baseHoningRates.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[secondVarUsed] = maxAmountTable[secondVarUsed];
	varTable[thirdVarUsed] = -1;
	iterateHelper(1, maxAmountTable[thirdVarUsed], addToCounterTable[thirdVarUsed] * baseHoningRates.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);

	return combinedHoningChain;
}

//starts at 1 for the first try and not at zero
inline std::vector<HoningResult> startHoningChain(const HoningParameter& baseHoningRates, bool isWeapon)
{
	std::vector<HoningResult> bestHoningChain;
	float bestHoningCost = -1.0f;


	auto createFunction = [&] (int graceCounter, int blessingCounter, int protectionCounter)
	{
		HoningChainElement chainElement = honingChainStep(HoningResult(graceCounter, blessingCounter, protectionCounter), baseHoningRates, isWeapon, 0.0f);
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

	static int maxAmountTable[] = { baseHoningRates.getMaxSolarGraceAmount(),baseHoningRates.getMaxSolarBlessingAmount(),baseHoningRates.getMaxSolarProtectionAmount() };
	static int addToCounterTable[] = { 3,2,1 };//the steps the counter moves for each solar thing

	int varTable[] = { 0,0,0 };
	std::cout << "Calculating 1/3... " << std::endl;
	varTable[firstVarUsed] = -1;
	iterateHelper(0, maxAmountTable[firstVarUsed], addToCounterTable[firstVarUsed] * baseHoningRates.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[firstVarUsed] = maxAmountTable[firstVarUsed];
	varTable[secondVarUsed] = -1;
	std::cout << "Calculating 2/3... " << std::endl;
	iterateHelper(1, maxAmountTable[secondVarUsed], addToCounterTable[secondVarUsed] * baseHoningRates.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);
	varTable[secondVarUsed] = maxAmountTable[secondVarUsed];
	varTable[thirdVarUsed] = -1;
	std::cout << "Calculating 3/3... " << std::endl;
	iterateHelper(1, maxAmountTable[thirdVarUsed], addToCounterTable[thirdVarUsed] * baseHoningRates.maxSolarAmountMultiplier, createFunction, varTable[0], varTable[1], varTable[2]);

	return bestHoningChain;
}


inline void calculateHoningForLevel(int currentItemHoningLevel, bool isIlvl1340Set)
{
	HoningParameter honingParameter { currentItemHoningLevel + 1, isIlvl1340Set};

	std::cout << "Calculating Weapon Honing cost." << std::endl;
	auto honingChainWeapon = startHoningChain(honingParameter, true);
	float avgGoldCostWeapon = getAverageCostOfHoningChain(honingChainWeapon);

	std::cout << "Calculating Armour Honing cost." << std::endl;
	auto honingChainArmour= startHoningChain(honingParameter, false);
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



static void printHoningCalculation()
{
	calculateHoningForLevel(HoningConfig::itemHoningLevel, HoningConfig::isIlvl1340Set);
}



