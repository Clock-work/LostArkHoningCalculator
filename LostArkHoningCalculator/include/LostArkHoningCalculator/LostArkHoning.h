#pragma once
#include "HoningParameter.h"
#include "HoningInput.h"
#include "HoningResult.h"
#include "HoningChain.h"
#include <functional>

//the amount of combinations that will be used
short getMaxAmountOfBranches(const HoningInput& honingInput, char* addToCounterTable)
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

//starts at 1 for the first try and not at zero
inline std::vector<HoningResult> startHoningChain(const HoningParameter& honingParameter, bool isWeapon)
{
	std::vector<HoningResult> bestHoningChain;
	float bestHoningCost = -1.0f;

	//the steps the counter moves for each solar thing IN ORDER OF THE SOLAR TYPES grace, blessing, protection, book
	char addToCounterTable[] = { 6 * honingParameter.maxSolarAmountMultiplier, 3 * honingParameter.maxSolarAmountMultiplier, 1 * honingParameter.maxSolarAmountMultiplier, 1 };

	auto createFunction = [&] (char graceCounter, char blessingCounter, char protectionCounter, char honingBookCounter)
	{
		HoningChainElement chainElement = honingChainStep(honingParameter, isWeapon, HoningInput(1, 0.0f, graceCounter, blessingCounter, protectionCounter, honingBookCounter));
		std::vector<HoningResult> bestChainAsVector = chainElement.getBestChain(honingParameter, isWeapon);
		float honingCost = getAverageCostOfHoningChain(bestChainAsVector, honingParameter, isWeapon);
		if ( isless(bestHoningCost, -0.99f) || isLess(honingCost, bestHoningCost) )
		{
			bestHoningCost = honingCost;
			bestHoningChain = std::move(bestChainAsVector);
		}
	};

	startNextChains(createFunction, honingParameter, HoningInput::getMaxInput(honingParameter), isWeapon, addToCounterTable);
	return bestHoningChain;
}

inline HoningChainElement getFullChain(const HoningParameter& honingParameter, bool isWeapon, const HoningInput& honingInput)
{
	HoningResult result { honingInput };

	if ( isMore(honingInput.getTotalHoningSuccessRate(honingParameter), 99.9f) )
	{//last honing in the chain, because it's guaranteed
		return HoningChainElement(result);
	}

	if ( isMore(honingInput.getNextArtisansEnergy(honingParameter), 99.9f) )
	{
		return HoningChainElement(result, getFullChain(honingParameter, isWeapon, honingInput.getHoningInputForNextTry(honingParameter, 0, 0, 0, 0)));
	}

	return HoningChainElement(result, 
							  getFullChain(honingParameter, isWeapon, 
														   honingInput.getHoningInputForNextTry(honingParameter)));
}

inline void calculateHoningForLevel(int currentItemHoningLevel, bool isIlvl1340Set)
{
	HoningParameter honingParameter { currentItemHoningLevel + 1, isIlvl1340Set };

	int honingBookAmount = 0;

	if (isIlvl1340Set)
	{
		std::cout << "For the ilvl 1340 set it's best to buy the weapon and 1 armour from abyss dungeon and 2 armour pieces from argos in the first week" << std::endl;
		std::cout << "Then upgrade the weapon to +11, chest and leg armour to +8 and the other armour pieces to +17 to reach ilvl 1415" << std::endl << std::endl;
	}


	std::cout << "Calculating Weapon Honing cost..." << std::endl;
	auto honingChainWeapon = startHoningChain(honingParameter, true);
	float avgGoldCostWeapon = getAverageCostOfHoningChain(honingChainWeapon, honingParameter, true);
	float avgTriesWeapon = getAverageTriesOfHoningChain(honingChainWeapon, honingParameter);

	std::cout << "Calculating Armour Honing cost..." << std::endl;
	auto honingChainArmour = startHoningChain(honingParameter, false);
	float avgGoldCostArmour = getAverageCostOfHoningChain(honingChainArmour, honingParameter, false);
	float avgTriesArmour = getAverageTriesOfHoningChain(honingChainArmour, honingParameter);


	std::cout << std::endl << "Honing Calculation for your Gear from +" << currentItemHoningLevel << " to +" << currentItemHoningLevel + 1 << std::endl << std::endl;

	if ( HoningConfig::useMarysShopPrices )
		std::cout << "Marys shop prices will also be used for this calculation (you can change this in the config)" << std::endl << std::endl;
	else
		std::cout << "Marys shop prices will not be used for this calculation (you can change this in the config)" << std::endl << std::endl;


	std::cout << std::endl << "Your Weapon has an average cost of " << avgGoldCostWeapon << " gold and needs on average " << avgTriesWeapon << " tries, "
		<< honingParameter.weaponLeapstoneCost * avgTriesWeapon << " leapstones and "
		<< honingParameter.destructionStoneCost * avgTriesWeapon << " destruction stones." << std::endl << std::endl;

	for ( auto& element : honingChainWeapon )
	{
		element.output(honingParameter, true);
	}

	auto fullChainWeapon = getFullChain(honingParameter, true, HoningInput(1, 0.0f, honingParameter.getMaxSolarGraceAmount(), honingParameter.getMaxSolarBlessingAmount(),
																		   honingParameter.getMaxSolarProtectionAmount(), honingBookAmount)).getBestChain(honingParameter, true);
	auto emptyChainWeapon = getFullChain(honingParameter, true, HoningInput(1, 0.0f, 0, 0, 0, 0)).getBestChain(honingParameter, true);

	std::cout << std::endl << "Weapon with full material use would cost " << getAverageCostOfHoningChain(fullChainWeapon, honingParameter, true) 
		<< " gold and needs on average " << getAverageTriesOfHoningChain(fullChainWeapon, honingParameter) << " tries with a maximum of " << fullChainWeapon.size() << " tries." << std::endl;
	fullChainWeapon.at(0).output(honingParameter, true);

	std::cout << std::endl << "Weapon with no material use would cost " << getAverageCostOfHoningChain(emptyChainWeapon, honingParameter, true) 
		<< " gold and needs on average " << getAverageTriesOfHoningChain(emptyChainWeapon, honingParameter) << " tries with a maximum of " << emptyChainWeapon.size() << " tries." << std::endl;
	emptyChainWeapon.at(0).output(honingParameter, true);

	std::cout << std::endl;

	std::cout << std::endl << "Your Armour has an average cost of " << avgGoldCostArmour << " gold and needs on average " << avgTriesArmour << " tries, "
		<< honingParameter.armourLeapstoneCost * avgTriesArmour << " leapstones and "
		<< honingParameter.guardianStoneCost * avgTriesArmour << " guardian stones." << std::endl << std::endl;

	for ( auto& element : honingChainArmour )
	{
		element.output(honingParameter, false);
	}
	
	auto fullChainArmour = getFullChain(honingParameter, false, HoningInput(1, 0.0f, honingParameter.getMaxSolarGraceAmount(), honingParameter.getMaxSolarBlessingAmount(),
																			honingParameter.getMaxSolarProtectionAmount(), honingBookAmount)).getBestChain(honingParameter, false);
	auto emptyChainArmour = getFullChain(honingParameter, false, HoningInput(1, 0.0f, 0, 0, 0, 0)).getBestChain(honingParameter, false);

	std::cout << std::endl << "Armour with full material use would cost " << getAverageCostOfHoningChain(fullChainArmour, honingParameter, false)
		<< " gold and needs on average " << getAverageTriesOfHoningChain(fullChainArmour, honingParameter) << " tries with a maximum of " << fullChainArmour.size() << " tries." << std::endl;
	fullChainArmour.at(0).output(honingParameter, false);

	std::cout << std::endl << "Armour with no material use would cost " << getAverageCostOfHoningChain(emptyChainArmour, honingParameter, false)
		<< " gold and needs on average " << getAverageTriesOfHoningChain(emptyChainArmour, honingParameter) << " tries with a maximum of " << emptyChainArmour.size() << " tries." << std::endl;
	emptyChainArmour.at(0).output(honingParameter, false);

	std::cout << std::endl << std::endl;

	//double finalGoldCost = ( 1 / ( totalSuccessRate / 100 ) ) * tempGoldCost;
}

static void printHoningCalculation()
{
	calculateHoningForLevel(HoningConfig::itemHoningLevel, HoningConfig::isIlvl1340Set);
}



