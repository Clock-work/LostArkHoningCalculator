#pragma once 
#include "Config.h"

//all success chances are in decimal point percent chance
//the rates are from https://lost-ark.maxroll.gg/resources/gear-honing-system and might change in the future 
struct HoningParameter
{
	//is the percent chance for success
	float successRateAsDecimal;
	float solarGraceChance;
	float solarBlessingChance;
	float solarProtectionChance;
	//the multiplier that is multiplied to the default solar amount values 
	int maxSolarAmountMultiplier;

	float baseHoningCostWeapon;
	float baseHoningCosArmour;

	//successRateAsDecimal is the percent chance for success
	HoningParameter(int targetItemHoningLevel, bool isIlvl1340Set)
	{
		//the additional success chance in decimal percent points that is achievable with solar materials. so default 60%
		float totalAdditionalMaterialSuccessRate = 60.00f;
		//with +12 and afterwards double the amount of solar pieces can be used, but each one will have a lower chance
		maxSolarAmountMultiplier = 1;
		successRateAsDecimal = 100;

		if ( targetItemHoningLevel >= 7 )
		{
			successRateAsDecimal -= 40;
		}
		if ( targetItemHoningLevel >= 8 )
		{
			successRateAsDecimal -= 15;
			totalAdditionalMaterialSuccessRate -= 15;
		}
		if ( targetItemHoningLevel >= 9 )
		{
			successRateAsDecimal -= 15;
			totalAdditionalMaterialSuccessRate -= 15;
		}
		if ( targetItemHoningLevel >= 12 )
		{
			maxSolarAmountMultiplier = 2;
			successRateAsDecimal -= 15;
			totalAdditionalMaterialSuccessRate -= 15;
		}
		if ( targetItemHoningLevel >= 15 )
		{
			successRateAsDecimal -= 5;
			totalAdditionalMaterialSuccessRate -= 5;
		}
		if ( targetItemHoningLevel >= 20 )
		{
			successRateAsDecimal -= 2;
			totalAdditionalMaterialSuccessRate -= 2;
		}

		//the total weighting of all solar stuff used together for their maximum success rate 
		float totalWeighting = getMaxSolarGraceAmount() + getMaxSolarBlessingAmount() * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER
			+ getMaxSolarProtectionAmount() * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER * HoningConfig::BLESSING_TO_PROTECTION_MULTIPLIER;
		solarGraceChance = 1.0f / totalWeighting * totalAdditionalMaterialSuccessRate;
		solarBlessingChance = 1.0f / totalWeighting * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER * totalAdditionalMaterialSuccessRate;
		solarProtectionChance = 1.0f / totalWeighting * HoningConfig::GRACE_TO_BLESSING_MULTIPLIER * HoningConfig::BLESSING_TO_PROTECTION_MULTIPLIER * totalAdditionalMaterialSuccessRate;

		baseHoningCostWeapon = getBaseHoningCostWeapon(targetItemHoningLevel, isIlvl1340Set);
		baseHoningCosArmour = getBaseHoningCostArmour(targetItemHoningLevel, isIlvl1340Set);
	}

	//default max solar amount multiplied with the multiplier 
	inline int getMaxSolarGraceAmount() const
	{
		return HoningConfig::DEFAULT_MAX_SOLAR_GRACE_AMOUNT * maxSolarAmountMultiplier;
	}

	//default max solar amount multiplied with the multiplier 
	inline int getMaxSolarBlessingAmount() const
	{
		return HoningConfig::DEFAULT_MAX_SOLAR_BLESSING_AMOUNT* maxSolarAmountMultiplier;
	}

	//default max solar amount multiplied with the multiplier 
	inline int getMaxSolarProtectionAmount() const
	{
		return HoningConfig::DEFAULT_MAX_SOLAR_PROTECTION_AMOUNT* maxSolarAmountMultiplier;
	}

	inline float getBaseHoningCost(bool isWeapon) const
	{
		return isWeapon ? baseHoningCostWeapon : baseHoningCosArmour;
	}

private:
	inline float getBaseHoningCostWeapon(int targetItemHoningLevel, bool isIlvl1340Set)
	{
		int upgradeStones = 138;
		int leapStones = 4;
		int fusionMat = 0;
		int rawGoldCost = 0;
		int silverCost = 16000;

		if ( targetItemHoningLevel >= 3 )
		{
			upgradeStones += 60;
			leapStones += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 4 )
		{
			fusionMat += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 7 )
		{
			upgradeStones += 60;
			leapStones += 2;
			fusionMat += 2;
			rawGoldCost += 120;
			silverCost += 1000;
		}
		if ( targetItemHoningLevel >= 10 )
		{
			upgradeStones += 62;
			leapStones += 2;
			silverCost += 2000;
		}
		if ( targetItemHoningLevel >= 13 )
		{
			upgradeStones += 60;
			fusionMat += 2;
			silverCost += 1000;
		}
		if ( targetItemHoningLevel >= 14 )
		{
			leapStones += 2;
			silverCost += 1000;
		}

		if ( isIlvl1340Set )
		{
			//todo: override values and calculate with other table: https://lost-ark.maxroll.gg/resources/gear-honing-system
			throw std::runtime_error("Item Level 1340 Set is not supported yet");
		}

		float goldCost = rawGoldCost + silverCost * ( 1.0f / static_cast<float>( HoningConfig::silverAmountPerGold ) );
		if ( isIlvl1340Set )
			goldCost += leapStones * MarketPrices::greaterHonorLeapstone + fusionMat * MarketPrices::basicFusion;
		else
			goldCost += leapStones * MarketPrices::honorLeapstone + fusionMat * MarketPrices::simpleFusion;
		goldCost += upgradeStones * MarketPrices::destructionStone;
		return goldCost;
	}

	inline float getBaseHoningCostArmour(int targetItemHoningLevel, bool isIlvl1340Set)
	{
		int upgradeStones = 82;
		int leapStones = 2;
		int fusionMat = 0;
		int rawGoldCost = 0;
		int silverCost = 11000;

		if ( targetItemHoningLevel >= 3 )
		{
			leapStones += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 4 )
		{
			upgradeStones += 38;
			fusionMat += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 7 )
		{
			upgradeStones += 36;
			rawGoldCost += 70;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 10 )
		{
			upgradeStones += 36;
			leapStones += 2;
			fusionMat += 2;
			silverCost += 1000;
		}
		if ( targetItemHoningLevel >= 13 )
		{
			upgradeStones += 36;
			silverCost += 1000;
		}
		if ( targetItemHoningLevel >= 14 )
		{
			leapStones += 2;
			silverCost += 1000;
		}

		if ( isIlvl1340Set )
		{
			//todo: override values and calculate with other table: https://lost-ark.maxroll.gg/resources/gear-honing-system
			throw std::runtime_error("Item Level 1340 Set is not supported yet");
		}

		float goldCost = rawGoldCost + silverCost * ( 1.0f / static_cast<float>( HoningConfig::silverAmountPerGold ) );
		if ( isIlvl1340Set )
			goldCost += leapStones * MarketPrices::greaterHonorLeapstone + fusionMat * MarketPrices::basicFusion;
		else
			goldCost += leapStones * MarketPrices::honorLeapstone + fusionMat * MarketPrices::simpleFusion;
		goldCost += upgradeStones * MarketPrices::guardianStone;
		return goldCost;
	}

};