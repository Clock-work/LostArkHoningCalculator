#pragma once 
#include "HoningResult.h"
#include <iostream>
#include <vector>

inline float getAverageCostOfHoningChain(const std::vector<HoningResult>& honingChain, const HoningParameter& honingParameter, bool isWeapon)
{
	float cost = 0.0f;
	float honingCostMultiplier = 1.0f;
	for ( const HoningResult& result : honingChain )
	{
		cost += result.honingInput.getTotalHoningCost(honingParameter, isWeapon) * honingCostMultiplier;
		honingCostMultiplier *= 1.0f - result.honingInput.getTotalHoningSuccessRate(honingParameter) / 100.0f;
	}
	return cost;
}
inline float getAverageTriesOfHoningChain(const std::vector<HoningResult>& honingChain, const HoningParameter& honingParameter)
{
	float tries = 0.0f;
	float tryMultiplier = 1.0f;
	for ( const HoningResult& result : honingChain )
	{
		tries += tryMultiplier;
		tryMultiplier *= 1.0f - result.honingInput.getTotalHoningSuccessRate(honingParameter) / 100.0f;
	}
	return tries;
}

struct BestHoningChain;

//has a vector of other honing chain branches 
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


	inline std::vector<HoningResult> getBestChain(std::vector<HoningResult> previousElements, const HoningParameter& honingParameter, bool isWeapon) const
	{
		std::vector<HoningResult> bestHoningChain;
		float bestHoningCost = -1;

		if ( previousElements.capacity() <= previousElements.size() + 1 )
		{
			previousElements.reserve(previousElements.size() + 4);
		}

		previousElements.push_back(result);
		for ( const HoningChainElement& branch : elements )
		{
			std::vector<HoningResult> bestSubChain = branch.getBestChain(previousElements, honingParameter, isWeapon);
			float honingCost = getAverageCostOfHoningChain(bestSubChain, honingParameter, isWeapon);
			if ( isLess(bestHoningCost, -0.99f) || isLess(honingCost, bestHoningCost) )
			{
				bestHoningCost = honingCost;
				bestHoningChain = std::move(bestSubChain);
			}
		}
		if ( isLess(bestHoningCost, -0.99f) )
		{
			return previousElements;
		}
		return bestHoningChain;
	}

	//hadBranches should be true if this honing chain was used with different combinations of materials used
	inline BestHoningChain getBestChain(const HoningParameter& honingParameter, bool isWeapon, bool hadBranches) const;

};

//contains the best honing chain from all the possible branches 
struct BestHoningChain
{
	std::vector<HoningResult> elements;
	const HoningParameter honingParameter;
	const bool isWeapon;
	//if this honing chain had branches and different combinations of materials used
	const bool hadBranches;
	BestHoningChain()
		: honingParameter(0, false), isWeapon(false), hadBranches(false)
	{

	}
	BestHoningChain(const HoningParameter& honingParameter, bool isWeapon, bool hadBranches)
		: honingParameter(honingParameter), isWeapon(isWeapon), hadBranches(hadBranches)
	{

	}

	BestHoningChain(const HoningParameter& honingParameter, bool isWeapon, bool hadBranches, 
					std::vector<HoningResult>&& elements)
		: honingParameter(honingParameter), isWeapon(isWeapon), hadBranches(hadBranches), elements(std::move(elements))
	{

	}

	BestHoningChain(BestHoningChain&& other)
		: honingParameter(other.honingParameter), isWeapon(other.isWeapon), hadBranches(other.hadBranches), 
		elements(std::move(other.elements))
	{

	}

	BestHoningChain& operator=(BestHoningChain&& other)
	{
		elements = std::move(other.elements);
		const_cast<HoningParameter&>(honingParameter) = other.honingParameter;
		const_cast<bool&>(isWeapon) = other.isWeapon;
		const_cast<bool&>(hadBranches) = other.hadBranches;
		return *this;
	}

	inline float getAverageCost() const
	{
		return ::getAverageCostOfHoningChain(elements, honingParameter, isWeapon);
	}
	inline float getAverageTries() const
	{
		return ::getAverageTriesOfHoningChain(elements, honingParameter);
	}

	//logs the average stats of the honing with the text "beginning" being the start of the log before " has an average cost..."
	//one additional line break at the end and beginning will be printed to the console if additionalLineBreaks is true
	void outputAverageHoning(const std::string& beginning, bool additionalLineBreaks = false) const
	{
		if (additionalLineBreaks)
			std::cout << std::endl;
		float averageTries = getAverageTries();
		std::cout << std::endl << beginning << " has an average cost of " << getAverageCost() << " gold and needs on average "
			<< averageTries << " tries, " << honingParameter.weaponLeapstoneCost * averageTries << " leapstones and "
			<< honingParameter.destructionStoneCost * averageTries << " destruction stones. And it has a maximum of "
			<< elements.size() << " tries." << std::endl;
		if (additionalLineBreaks)
			std::cout << std::endl;
	}

	//logs the average stats of the honing with the text "Your Armour", or "Your Weapon"
	//one additional line break at the end and beginning will be printed to the console
	void outputAverageHoning() const
	{
		std::cout << std::endl;
		std::string beginning = "Your Armour";
		if (isWeapon)
			beginning = "Your Weapon";
		outputAverageHoning(beginning, true);
	}

	//logs every honing step with its stats
	void outputHoningTries() const
	{
		for (const HoningResult& element : elements)
		{
			element.output(honingParameter, isWeapon);
		}
	}

	void outputFirstTry() const
	{
		elements.at(0).output(honingParameter, isWeapon);
	}

	//returns if this honing chain was used with no material used and with no additional branches
	bool isNoMaterialUse() const
	{
		return !hadBranches && elements.at(0).honingInput.hasEqualMaterialsUsed(HoningInput::getEmptyInput());
	}

	//returns if this honing chain was used with no material used and with no additional branches
	bool isFullMaterialUse() const
	{
		return !hadBranches && elements.at(0).honingInput.hasEqualMaterialsUsed(HoningInput::getFullInput(honingParameter));
	}

	//full output of average and tries for the different kinds of honing chains
	void output() const
	{
		std::string beginning = "Armour";
		if (isWeapon)
			beginning = "Weapon";
		if (isNoMaterialUse())
		{
			outputAverageHoning(beginning + " with no material use");
			outputFirstTry();
		}
		else if (isFullMaterialUse())
		{
			outputAverageHoning(beginning + " with full material use");
			outputFirstTry();
		}
		else
		{
			outputAverageHoning();
			outputHoningTries();
		}
	}

	//same materials used at the start
	bool hasSameMaterialsUsed(const BestHoningChain& other) const
	{
		return elements.at(0).honingInput.hasEqualMaterialsUsed(other.elements.at(0).honingInput);
	}

};

inline BestHoningChain HoningChainElement::getBestChain(const HoningParameter& honingParameter, bool isWeapon, bool hadBranches) const
{
	std::vector<HoningResult> previousElements;
	previousElements = getBestChain(previousElements, honingParameter, isWeapon);
	return BestHoningChain(honingParameter, isWeapon, hadBranches, std::move(previousElements));
}