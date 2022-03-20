#pragma once 
#include "Config.h"
#include <vector>

//the additional crafting materials numerated from GRACE=0, blessing ,protection, book. 
//MATERIAL_AMOUNT must always be last inside of the enum
enum MaterialToUse : char
{
	GRACE, BLESSING, PROTECTION, BOOK, MATERIAL_AMOUNT
};

struct MaterialOrderElement
{
	MaterialToUse material;
	float price;
	MaterialOrderElement(MaterialToUse material, float price)
		: material(material), price(price)
	{}
};

//all success chances are in decimal point percent chance
//the rates are from https://lost-ark.maxroll.gg/resources/gear-honing-system and might change in the future 
struct HoningParameter
{
private:
	//from most expensive to least expensive material to use
	std::vector<MaterialOrderElement> materialUseOrderWeapon;
	//from most expensive to least expensive material to use
	std::vector<MaterialOrderElement> materialUseOrderArmour;

public:

	//is the percent chance for success
	float successRateAsDecimal;
	float solarGraceChance;
	float solarBlessingChance;
	float solarProtectionChance;
	//the multiplier that is multiplied to the default solar amount values 
	int maxSolarAmountMultiplier;

	//total base gold cost for weapon honing 
	float baseHoningCostWeapon;
	//total base gold cost for armour honing 
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
		calculateMaterialOrder(true);
		calculateMaterialOrder(false);
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

	//price normalized per percent of all materials
	inline float getNormalizedMaterialPrice(MaterialToUse material, bool isWeapon) const
	{
		if ( material == MaterialToUse::GRACE )
			return getSolarGraceCost() / solarGraceChance;
		else if ( material == MaterialToUse::BLESSING )
			return getSolarBlessingCost() / solarBlessingChance;
		else if ( material == MaterialToUse::PROTECTION )
			return getSolarProtectionCost() / solarProtectionChance;
		else if ( material == MaterialToUse::BOOK )
			return getHoningBookCost(isWeapon) / HoningConfig::HONING_BOOK_CHANCE;
	}

	inline const std::vector<MaterialOrderElement>& getMaterialUseOrder(bool isWeapon) const
	{
		if ( isWeapon )
			return materialUseOrderWeapon;
		else
			return materialUseOrderArmour;
	}

private:
	inline float getBaseHoningCostWeapon(int targetItemHoningLevel, bool isIlvl1340Set)
	{
		int upgradeStones = 138;
		int leapStones = 4;
		int fusionMat = 0;
		int rawGoldCost = 0;
		int silverCost = 16000;
		float shardCost = MarketPrices::smallHonorShardPouch / 12.8f;

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
			shardCost = MarketPrices::smallHonorShardPouch / 6.76f;
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
			goldCost += leapStones * getGreaterHonorLeapstoneCost() + fusionMat * getBasicFusionCost();
		else
			goldCost += leapStones * getHonorLeapstoneCost() + fusionMat * getSimpleFusionCost();
		goldCost += upgradeStones * getDestructionStoneCost();
		goldCost += shardCost;
		return goldCost;
	}

	inline float getBaseHoningCostArmour(int targetItemHoningLevel, bool isIlvl1340Set)
	{
		int upgradeStones = 82;
		int leapStones = 2;
		int fusionMat = 0;
		int rawGoldCost = 0;
		int silverCost = 11000;
		float shardCost = MarketPrices::smallHonorShardPouch / 12.8f;

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
			shardCost = MarketPrices::smallHonorShardPouch / 6.76f;
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
			goldCost += leapStones * getGreaterHonorLeapstoneCost() + fusionMat * getBasicFusionCost();
		else
			goldCost += leapStones * getHonorLeapstoneCost() + fusionMat * getSimpleFusionCost();
		goldCost += upgradeStones * getGuardianStoneCost();
		goldCost += shardCost;
		return goldCost;
	}

	inline void calculateMaterialOrder(bool isWeapon)
	{
		std::vector<MaterialOrderElement>* materialUseOrder;
		if ( isWeapon )
			materialUseOrder = &materialUseOrderWeapon;
		else
			materialUseOrder = &materialUseOrderArmour;

		materialUseOrder->reserve(MaterialToUse::MATERIAL_AMOUNT);

		for ( char material = 0; material < MaterialToUse::MATERIAL_AMOUNT; material++ )
		{
			float newMaterialPrice = getNormalizedMaterialPrice(static_cast<MaterialToUse>( material ), isWeapon);
			bool added = false;
			for ( int i = 0; i < materialUseOrder->size(); ++i )
			{
				float orderPrice = materialUseOrder->at(i).price;
				if ( isMore(newMaterialPrice, orderPrice) )
				{
					materialUseOrder->emplace(materialUseOrder->begin() + i, static_cast<MaterialToUse>( material ), newMaterialPrice);
					added = true;
					break;
				}
			}
			if ( !added )
			{
				materialUseOrder->emplace_back(static_cast<MaterialToUse>( material ), newMaterialPrice);
			}
		}
	}

};