#pragma once 
#include "HoningParameter.h"
#include "HoningInput.h"

struct HoningResult
{
	HoningInput honingInput;
	short tryNumber;

	HoningResult(const HoningInput& honingInput)
		: honingInput(honingInput), tryNumber(honingInput.tryNumber)
	{

	}

	//returns the correct honing cost, also for the last element with tryNumber==-1 
	inline float getHoningCost(const HoningParameter& honingParameter, bool isWeapon, float& honingCostMultiplier) const
	{
		if ( tryNumber == -2 ) //maximum solar materials chance
			return 99999999999.9f;
		if ( tryNumber == -1 ) // maximum recursion depth
		{
			float cost = 0.0f;
			float nextArtisansEnergy = honingInput.artisansEnergy;
			int tryNumber = HoningConfig::MAX_HONING_TRIES;
			for ( float currentHoningRate = honingInput.getTotalHoningSuccessRate(honingParameter); currentHoningRate < 99.99f; currentHoningRate += 1.0f )
			{
				cost += honingInput.getTotalHoningCost(honingParameter, isWeapon) * honingCostMultiplier;
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
		return honingInput.getTotalHoningCost(honingParameter, isWeapon) * honingCostMultiplier;
	}

	//returns the correct average tries, also for the last element with tryNumber==-1 
	inline float getAverageTries(const HoningParameter& honingParameter, float& tryMultiplier) const
	{
		if ( tryNumber == -2 ) //maximum solar materials chance
			return 99999999999.9f;
		if ( tryNumber == -1 ) // maximum recursion depth
		{
			float tries = 0.0f;
			float nextArtisansEnergy = honingInput.artisansEnergy;
			int tryNumber = HoningConfig::MAX_HONING_TRIES;
			for ( float currentHoningRate = honingInput.getTotalHoningSuccessRate(honingParameter); currentHoningRate < 99.99f; currentHoningRate += 1.0f )
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

	inline void output(const HoningParameter& honingParameter, bool isWeapon)
	{
		if ( tryNumber == -1 )
			std::cout << "The remaining Honing tries";
		else
			std::cout << "Honing try " << tryNumber;

		if ( tryNumber == -1 )
			std::cout << " (After Base Honing Rate " << honingInput.getBaseHoningSuccessRate(honingParameter) << "%) with ";
		else
			std::cout << " (Base Honing Rate " << honingInput.getBaseHoningSuccessRate(honingParameter) << "%) with ";

		int counter = 0;
		if ( honingInput.solarGraceToUse > 0 )
		{
			std::cout << (int) honingInput.solarGraceToUse << " Solar Grace";
			counter++;
		}
		if ( honingInput.solarBlessingToUse > 0 )
		{
			if ( counter > 0 )
				std::cout << ", ";
			std::cout << (int)honingInput.solarBlessingToUse << " Solar Blessing";
			counter++;
		}
		if ( honingInput.solarProtectionToUse > 0 )
		{
			if ( counter > 0 )
				std::cout << ", ";
			std::cout << (int) honingInput.solarProtectionToUse << " Solar Protection";
			counter++;
		}
		if ( counter > 0 )
		{
			std::cout << " used and with ";
		}
		if ( tryNumber == -1 )
		{
			float averageTryMultiplier = 1.0f;
			std::cout << getAverageTries(honingParameter, averageTryMultiplier) << " more tries on average ";
		}

		if ( tryNumber != -1 )
		{
			std::cout << "a Success Rate of ";
			std::cout << honingInput.getTotalHoningSuccessRate(honingParameter) << "% would cost " << honingInput.getTotalHoningCost(honingParameter, isWeapon);
		}
		else
		{
			float averageCostMultiplier = 1.0f;
			std::cout << "would cost " << getHoningCost(honingParameter, isWeapon, averageCostMultiplier);
		}


		if ( tryNumber == -1 )
			std::cout << " more gold on average.";
		else
			std::cout << " gold.";
		std::cout << std::endl;
	}

};