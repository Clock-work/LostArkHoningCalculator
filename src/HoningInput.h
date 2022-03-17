#pragma once
#include "LostArkHoning.h"

//the input parameter for the try with the given tryNumber
struct HoningInput
{
	//starts at 1 for the first try and not at zero
	const short tryNumber;
	const char solarGraceToUse;
	const char solarBlessingToUse;
	const char solarProtectionToUse;
	//the current artisans energy from the previous fails before this try
	const float artisansEnergy = 0.0f;

	HoningInput(short tryNumber, float artisansEnergy, char solarGraceToUse, char solarBlessingToUse, char solarProtectionToUse)
		: tryNumber(tryNumber), artisansEnergy(artisansEnergy), solarGraceToUse(solarGraceToUse), solarBlessingToUse(solarBlessingToUse), solarProtectionToUse(solarProtectionToUse)
	{

	}

	HoningInput getHoningInputForNextTry(const HoningParameter& honingParameter, char solarGraceToUse, char solarBlessingToUse, char solarProtectionToUse) const
	{
		return HoningInput(tryNumber + 1, getNextArtisansEnergy(honingParameter), solarGraceToUse, solarBlessingToUse, solarProtectionToUse);
	}

	//the additional chance if all honing before this try failed
	float getAdditionalHoningChanceFromFailing(const HoningParameter& baseHoningRates) const
	{
		float additionalHoningChance = 0.0f;
		float baseHoningChance = getBaseHoningSuccessRate(baseHoningRates);
		for ( short i = 1; i < std::min(tryNumber, (short) 11); ++i )//after 10 failed tries there is no additional base chance (only 100% of the base chance can be added)
		{
			//if ( i == 1 )// 10% of base honing chance added on first failure
			//	additionalHoningChance += baseHoningRates.successRateAsDecimal * 0.1f;
			//else// for every following failure, 1% chance is added 
				//additionalHoningChance += 1.0f;
			additionalHoningChance += baseHoningRates.successRateAsDecimal * 0.1f;//nevermind, NEW: always 10% of chance is added 
		}
		return additionalHoningChance;
	}

	//the additional chance with the selected amount of solar materials 
	//THIS may not be above the MAX_ADDITIONAL_SOLAR_CHANCE
	float getAdditionalSolarSuccessRate(const HoningParameter& baseHoningRates) const
	{
		return solarGraceToUse * baseHoningRates.solarGraceChance + solarBlessingToUse * baseHoningRates.solarBlessingChance + solarProtectionToUse * baseHoningRates.solarProtectionChance;
	}

	//returns the total successrate including the additional base chance from failing
	float getBaseHoningSuccessRate(const HoningParameter& baseHoningRates) const
	{
		return baseHoningRates.successRateAsDecimal + getAdditionalHoningChanceFromFailing(baseHoningRates);
	}

	//returns the total successrate including the additional base chance from failing and also the solar amount used and also the artisansEnergy
	float getTotalHoningSuccessRate(const HoningParameter& baseHoningRates) const
	{
		if ( isMore(artisansEnergy, 99.9f) )//if artisans energy is at 100, then this try is always successful
			return 100.0f;
		return getBaseHoningSuccessRate(baseHoningRates) + getAdditionalSolarSuccessRate(baseHoningRates);
	}

	//returns the additional cost of the solar pieces used
	float getAdditionalSolarCost() const
	{
		return solarGraceToUse * MarketPrices::solarGrace + solarBlessingToUse * MarketPrices::solarBlessing + solarProtectionToUse * MarketPrices::solarProtection;
	}

	//returns the total cost including the additional cost of the solar pieces used and the cost of the base honing materials 
	float getTotalHoningCost(const HoningParameter& honingParameter, bool isWeapon) const
	{
		return honingParameter.getBaseHoningCost(isWeapon) + getAdditionalSolarCost();
	}

	//returns the artisans energy for the next try after this one failed
	float getNextArtisansEnergy(const HoningParameter& baseHoningRates) const
	{
		return artisansEnergy + HoningConfig::ARTISANS_ENERGY_MULTIPLIER * getTotalHoningSuccessRate(baseHoningRates);
	}

};