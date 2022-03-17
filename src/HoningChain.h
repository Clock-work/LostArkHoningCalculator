#pragma once 
#include "HoningResult.h"


inline float getAverageCostOfHoningChain(const std::vector<HoningResult>& honingChain, const HoningParameter& honingParameter, bool isWeapon)
{
	float cost = 0.0f;
	float honingCostMultiplier = 1.0f;
	for ( const HoningResult& result : honingChain )
	{
		cost += result.getHoningCost(honingParameter, isWeapon, honingCostMultiplier);
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
		tries += result.getAverageTries(honingParameter, tryMultiplier);
		tryMultiplier *= 1.0f - result.honingInput.getTotalHoningSuccessRate(honingParameter) / 100.0f;
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


	inline std::vector<HoningResult> getBestChain(std::vector<HoningResult> previousElements, const HoningParameter& honingParameter, bool isWeapon)
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

	inline std::vector<HoningResult> getBestChain(const HoningParameter& honingParameter, bool isWeapon)
	{
		std::vector<HoningResult> previousElements;
		return getBestChain(previousElements, honingParameter, isWeapon);
	}

};
