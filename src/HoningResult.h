#pragma once 
#include "HoningParameter.h"
#include "HoningInput.h"

struct HoningResult
{
	HoningInput honingInput;

	HoningResult(const HoningInput& honingInput)
		: honingInput(honingInput)
	{

	}

	inline void output(const HoningParameter& honingParameter, bool isWeapon)
	{
		std::cout << "Honing try " << honingInput.tryNumber;
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

		std::cout << "a Success Rate of ";
		std::cout << honingInput.getTotalHoningSuccessRate(honingParameter) << "% would cost " << honingInput.getTotalHoningCost(honingParameter, isWeapon);
		std::cout << " gold.";
		std::cout << std::endl;
	}

};