#pragma once
#include "HoningParameter.h"
#include "HoningInput.h"
#include "HoningResult.h"
#include "HoningChain.h"
#include <functional>
#include <thread>

//the amount of combinations that will be used
inline short getMaxAmountOfBranches(const HoningInput& honingInput, char* addToCounterTable)
{
	short amountOfBranches = 1;
	if ( honingInput.solarGraceToUse > 1 )
	{
		amountOfBranches += honingInput.solarGraceToUse / static_cast<float>( addToCounterTable[MaterialToUse::GRACE] + 1 );
	}
	else
	{
		amountOfBranches += honingInput.solarGraceToUse;
	}
	if ( honingInput.solarBlessingToUse > 1 )
	{
		amountOfBranches += honingInput.solarBlessingToUse / static_cast<float>( addToCounterTable[MaterialToUse::BLESSING] + 1 );
	}
	else
	{
		amountOfBranches += honingInput.solarBlessingToUse;
	}
	if ( honingInput.solarProtectionToUse > 1 )
	{
		amountOfBranches += honingInput.solarProtectionToUse / static_cast<float>( addToCounterTable[MaterialToUse::PROTECTION] + 1 );
	}
	else
	{
		amountOfBranches += honingInput.solarProtectionToUse;
	}
	if ( honingInput.honingBookToUse > 1 )
	{
		amountOfBranches += honingInput.honingBookToUse / static_cast<float>( addToCounterTable[MaterialToUse::BOOK] + 1 );
	}
	else
	{
		amountOfBranches += honingInput.honingBookToUse;
	}
	return amountOfBranches;
}

//calls the createfunction with the parameter configuration. below is an example for the parameter to control the iterating
/*
	the counters will be incremented from the amount to use in the honingInput until they reach zero 

	//the steps the counter moves for each solar thing IN ORDER OF THE SOLAR TYPES grace, blessing, protection, honing book
	char addToCounterTable[] = { 1 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier, 1 };


	for the first step all solar amount used should be set to the maximum and then the current amount used should be used

*/
inline void startNextChains(const std::function<void(char, char, char, char)>& createFunction, const HoningParameter& honingParameter, const HoningInput& honingInput, bool isWeapon, char* addToCounterTable)
{
	char counters[] = { honingInput.solarGraceToUse, honingInput.solarBlessingToUse, honingInput.solarProtectionToUse, honingInput.honingBookToUse };

	const std::vector<MaterialOrderElement>& materialUseOrder = honingParameter.getMaterialUseOrder(isWeapon);

	char materialUseCounter = 0;

	createFunction(counters[MaterialToUse::GRACE], counters[MaterialToUse::BLESSING], counters[MaterialToUse::PROTECTION],
				   counters[MaterialToUse::BOOK]);//on first try use the material count from the last chain(or start)

	while ( true )
	{
		counters[materialUseOrder.at(materialUseCounter).material] -= addToCounterTable[materialUseOrder.at(materialUseCounter).material];
		if ( counters[materialUseOrder.at(materialUseCounter).material] < 0 && counters[materialUseOrder.at(materialUseCounter).material] > -addToCounterTable[materialUseOrder.at(materialUseCounter).material] )
		{
			counters[materialUseOrder.at(materialUseCounter).material] = 0;//if the counter skips the 0, then that try will be repeated
		}

		if ( counters[materialUseOrder.at(materialUseCounter).material] < 0 )
		{
			counters[materialUseOrder.at(materialUseCounter).material] = 0;
			do
			{
				materialUseCounter++;
				if ( materialUseCounter == materialUseOrder.size() )//no more counters to modify, so all should be zero for the last try above
					return;
			}
			while ( counters[materialUseOrder.at(materialUseCounter).material] == 0 );
		}
		else
		{
			createFunction(counters[MaterialToUse::GRACE], counters[MaterialToUse::BLESSING], counters[MaterialToUse::PROTECTION],
						   counters[MaterialToUse::BOOK]);//on first try use the material count from the last chain(or start)
		}

	}
}

//starts at 1 for the first try and not at zero
inline HoningChainElement honingChainStep(const HoningParameter& honingParameter, bool isWeapon, const HoningInput& honingInput)
{
	HoningResult result { honingInput };

	if ( isMore(honingInput.getTotalHoningSuccessRate(honingParameter), 99.9f) )
	{//last honing in the chain, because it's guaranteed
		return HoningChainElement(result);
	}

	if ( isMore(honingInput.getNextArtisansEnergy(honingParameter), 99.9f) )
	{
		return HoningChainElement(result, honingChainStep(honingParameter, isWeapon, honingInput.getHoningInputForNextTry(honingParameter, 0, 0, 0, 0)));
	}

	//the steps the counter moves for each solar thing IN ORDER OF THE SOLAR TYPES grace, blessing, protection, book
	char addToCounterTableVariant1[] = { 1 , 1, 1, 1 };
	char addToCounterTableVariant2[] = { 6 * honingParameter.maxSolarAmountMultiplier, 3 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier, 1 };
	char* addToCounterTable = addToCounterTableVariant1;
	// after 75% artisans energy use smaller increments for the counter which takes more time
	if ( honingInput.getNextArtisansEnergy(honingParameter) < 75.0f )
	{
		addToCounterTable = addToCounterTableVariant2;
	}

	HoningChainElement combinedHoningChain { result, getMaxAmountOfBranches(honingInput, addToCounterTable) };

	auto createFunction = [&] (char graceCounter, char blessingCounter, char protectionCounter, char honingBookCounter)
	{
		combinedHoningChain.elements.push_back(
			honingChainStep(honingParameter,
							isWeapon,
							honingInput.getHoningInputForNextTry(honingParameter,
																 graceCounter,
																 blessingCounter,
																 protectionCounter,
																 honingBookCounter)));
	};

	startNextChains(createFunction, honingParameter, honingInput, isWeapon, addToCounterTable);
	return combinedHoningChain;
}

// returns the full honing chains with all branches (combinations of materials used)
//the first try starts at 1 and not at zero
inline BestHoningChain startHoningChain(const HoningParameter& honingParameter, bool isWeapon)
{
	BestHoningChain result(honingParameter, isWeapon, true);
	float bestHoningCost = -1.0f;

	//the steps the counter moves for each solar thing IN ORDER OF THE SOLAR TYPES grace, blessing, protection, book
	char addToCounterTable[] = { 6 * honingParameter.maxSolarAmountMultiplier, 3 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier, 1 };

	auto createFunction = [&] (char graceCounter, char blessingCounter, char protectionCounter, char honingBookCounter)
	{
		HoningChainElement chainElement = honingChainStep(honingParameter, isWeapon, HoningInput(1, 0.0f, graceCounter, blessingCounter, protectionCounter, honingBookCounter));
		BestHoningChain bestHoningChain = chainElement.getBestChain(honingParameter, isWeapon, true);
		float honingCost = bestHoningChain.getAverageCost();
		if ( isless(bestHoningCost, -0.99f) || isLess(honingCost, bestHoningCost) )
		{
			bestHoningCost = honingCost;
			result = std::move(bestHoningChain);
		}
	};

	startNextChains(createFunction, honingParameter, HoningInput::getMaxInput(honingParameter), isWeapon, addToCounterTable);
	return result;
}

inline HoningChainElement fullChainStep(const HoningParameter& honingParameter, bool isWeapon, const HoningInput& honingInput)
{
	HoningResult result { honingInput };

	if ( isMore(honingInput.getTotalHoningSuccessRate(honingParameter), 99.9f) )
	{//last honing in the chain, because it's guaranteed
		return HoningChainElement(result);
	}

	if ( isMore(honingInput.getNextArtisansEnergy(honingParameter), 99.9f) )
	{
		return HoningChainElement(result, fullChainStep(honingParameter, isWeapon, honingInput.getHoningInputForNextTry(honingParameter, 0, 0, 0, 0)));
	}

	return HoningChainElement(result, 
							  fullChainStep(honingParameter, isWeapon,
														   honingInput.getHoningInputForNextTry(honingParameter)));
}

//only one full chain with the materials used from honingInput without branches
inline BestHoningChain getFullChain(const HoningParameter& honingParameter, bool isWeapon, const HoningInput& honingInput)
{
	return fullChainStep(honingParameter, isWeapon, honingInput).getBestChain(honingParameter, isWeapon, false);
}

void calculateHoningForLevel(int currentItemHoningLevel, bool isIlvl1340Set);

static void printHoningCalculation()
{
	calculateHoningForLevel(HoningConfig::itemHoningLevel, HoningConfig::isIlvl1340Set);
}