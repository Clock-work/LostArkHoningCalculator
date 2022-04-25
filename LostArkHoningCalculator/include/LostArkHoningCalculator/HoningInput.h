#pragma once
#include "HoningParameter.h"
#include <math.h>

//the input parameter for the try with the given tryNumber
struct HoningInput
{
	//starts at 1 for the first try and not at zero
	const short tryNumber;
	const char solarGraceToUse;
	const char solarBlessingToUse;
	const char solarProtectionToUse;
	//currently only 0, or 1 is possible 
	const char honingBookToUse;
	//the current artisans energy from the previous fails before this try
	const float artisansEnergy = 0.0f;

	HoningInput(short tryNumber, float artisansEnergy, char solarGraceToUse, char solarBlessingToUse, char solarProtectionToUse, char honingBookToUse)
		: tryNumber(tryNumber), artisansEnergy(artisansEnergy), solarGraceToUse(solarGraceToUse), solarBlessingToUse(solarBlessingToUse), solarProtectionToUse(solarProtectionToUse), honingBookToUse(honingBookToUse)
	{

	}

	//all materials used set to maximum
	inline static HoningInput getMaxInput(const HoningParameter& honingParameter)
	{
		return HoningInput(1, 0.0f, honingParameter.getMaxSolarGraceAmount(), honingParameter.getMaxSolarBlessingAmount(), 
						   honingParameter.getMaxSolarProtectionAmount(), HoningConfig::MAX_HONING_BOOK_AMOUNT);
	}

	//no material use input
	inline static HoningInput getEmptyInput()
	{
		return HoningInput(1, 0.0f, 0, 0, 0, 0);
	}

	//full material use input (is the same as getMaxInput, but this method here can have honing book amount set to 0)
	inline static HoningInput getFullInput(const HoningParameter& honingParameter, bool isIlvl1340Set)
	{
		bool shouldUseBooks = !isIlvl1340Set && HoningConfig::useBooksForFullMaterialCalc;
		return HoningInput(1, 0.0f, honingParameter.getMaxSolarGraceAmount(), honingParameter.getMaxSolarBlessingAmount(),
						   honingParameter.getMaxSolarProtectionAmount(), 
						   shouldUseBooks ? HoningConfig::MAX_HONING_BOOK_AMOUNT : 0);
	}

	HoningInput getHoningInputForNextTry(const HoningParameter& honingParameter, char solarGraceToUse, char solarBlessingToUse, char solarProtectionToUse, char honingBookToUse) const
	{
		return HoningInput(tryNumber + 1, getNextArtisansEnergy(honingParameter), solarGraceToUse, solarBlessingToUse, solarProtectionToUse, honingBookToUse);
	}

	HoningInput getHoningInputForNextTry(const HoningParameter& honingParameter) const
	{
		return HoningInput(tryNumber + 1, getNextArtisansEnergy(honingParameter), solarGraceToUse, solarBlessingToUse, solarProtectionToUse, honingBookToUse);
	}

	//the additional chance if all honing before this try failed
	float getAdditionalHoningChanceFromFailing(const HoningParameter& honingParameter) const
	{
		float additionalHoningChance = 0.0f;
		for ( short i = 1; i < std::min(tryNumber, (short) 11); ++i )//after 10 failed tries there is no additional base chance (only 100% of the base chance can be added)
		{
			//if ( i == 1 )// 10% of base honing chance added on first failure
			//	additionalHoningChance += baseHoningRates.successRateAsDecimal * 0.1f;
			//else// for every following failure, 1% chance is added 
				//additionalHoningChance += 1.0f;
			additionalHoningChance += honingParameter.successRateAsDecimal * 0.1f;//nevermind, NEW: always 10% of chance is added 
		}
		return additionalHoningChance;
	}

		//returns the total successrate including the additional base chance from failing
	float getBaseHoningSuccessRate(const HoningParameter& honingParameter) const
	{
		return honingParameter.successRateAsDecimal + getAdditionalHoningChanceFromFailing(honingParameter);
	}

	//the additional chance with the selected amount of solar materials 
	//DOES NOT INCLUDE the additional honing book chance 
	//can not be higher than the base success rate from honingparameter
	float getAdditionalSolarSuccessRate(const HoningParameter& honingParameter) const
	{
		float solarRate =  solarGraceToUse * honingParameter.solarGraceChance + solarBlessingToUse * honingParameter.solarBlessingChance
			+ solarProtectionToUse * honingParameter.solarProtectionChance;
		if (isMore(solarRate, honingParameter.successRateAsDecimal))
		{
			solarRate = honingParameter.successRateAsDecimal;
		}
		return solarRate;
	}

	float getAdditionalHoningBookSuccessRate() const
	{
		return honingBookToUse * HoningConfig::HONING_BOOK_CHANCE;
	}

	//returns the total successrate including the additional base chance from failing and also the solar amount used and also the artisansEnergy
	//also includes the honing books
	//will not be above 100.0f
	float getTotalHoningSuccessRate(const HoningParameter& honingParameter) const
	{
		if ( isMore(artisansEnergy, 99.9f) )//if artisans energy is at 100, then this try is always successful
			return 100.0f;
		float totalRate = getBaseHoningSuccessRate(honingParameter) + getAdditionalSolarSuccessRate(honingParameter) + getAdditionalHoningBookSuccessRate();
		return std::min(totalRate, 100.0f);
	}

	//returns the additional cost of the solar pieces used
	//DOES NOT INCLUDE the additional honing book cost
	float getAdditionalSolarCost() const
	{
		return solarGraceToUse * getSolarGraceCost() + solarBlessingToUse * getSolarBlessingCost() + 
			solarProtectionToUse * getSolarProtectionCost();
	}

	float getAdditionalHoningBookCost(bool isWeapon, bool isIlvl1340Set) const
	{
		return honingBookToUse * getHoningBookCost(isWeapon, isIlvl1340Set);
	}

	//returns the total cost including the additional cost of the solar pieces used and the cost of the base honing materials 
	//also includes the honing books 
	//if the additional solar materials used are too much, then it will return the highest possible cost
	float getTotalHoningCost(const HoningParameter& honingParameter, bool isWeapon) const
	{
		if ( isMore(getAdditionalSolarSuccessRate(honingParameter), HoningConfig::MAX_ADDITIONAL_SOLAR_CHANCE) )
		{
			return 999999999999.0f;
		}
		return honingParameter.getBaseHoningCost(isWeapon) + getAdditionalSolarCost() 
			+ getAdditionalHoningBookCost(isWeapon, honingParameter.isIlvl1340Set);
	}

	//returns the artisans energy for the next try after this one failed
	float getNextArtisansEnergy(const HoningParameter& baseHoningRates) const
	{
		return artisansEnergy + HoningConfig::ARTISANS_ENERGY_MULTIPLIER * getTotalHoningSuccessRate(baseHoningRates) 
			+ HoningConfig::ARTISANS_ENERGY_ADDED_FLAT;
	}

	bool hasEqualMaterialsUsed(const HoningInput& other) const
	{
		return this->solarGraceToUse == other.solarGraceToUse && this->solarBlessingToUse == other.solarBlessingToUse &&
			this->solarProtectionToUse == other.solarProtectionToUse && this->honingBookToUse == other.honingBookToUse;
	}

};