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

	int armourLeapstoneCost;
	int weaponLeapstoneCost;
	int destructionStoneCost;
	int guardianStoneCost;
	int rawGoldCost;

	bool isIlvl1340Set;

	//successRateAsDecimal is the percent chance for success
	HoningParameter(int targetItemHoningLevel, bool isIlvl1340Set)
		: isIlvl1340Set(isIlvl1340Set)
	{
		//the additional success chance in decimal percent points that is achievable with solar materials. so default 60%
		float totalAdditionalMaterialSuccessRate = 60.00f;
		//with +12 and afterwards double the amount of solar pieces can be used, but each one will have a lower chance
		maxSolarAmountMultiplier = 1;
		successRateAsDecimal = 100;

		if ( targetItemHoningLevel >= 7 )
		{
			successRateAsDecimal -= 40;
			if (!isIlvl1340Set && HoningConfig::useT3StrongholdHoningResearch)
			{
				//todo: test if the research also affects the solar material chance 
				successRateAsDecimal += HoningConfig::T3_STRONGHOLD_HONING_RESEARCH_ADD;
			}
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

		if (isIlvl1340Set)
		{
			baseHoningCostWeapon = getMediumBaseHoningCostWeapon(targetItemHoningLevel);
			baseHoningCosArmour = getMediumBaseHoningCostArmour(targetItemHoningLevel);
		}
		else
		{
			baseHoningCostWeapon = getLowBaseHoningCostWeapon(targetItemHoningLevel);
			baseHoningCosArmour = getLowBaseHoningCostArmour(targetItemHoningLevel);
		}
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
			return getHoningBookCost(isWeapon, isIlvl1340Set) / HoningConfig::HONING_BOOK_CHANCE;
	}

	//the order of solar materials and book to use in relation to the normalized gold price per percent chance added 
	inline const std::vector<MaterialOrderElement>& getMaterialUseOrder(bool isWeapon) const
	{
		if ( isWeapon )
			return materialUseOrderWeapon;
		else
			return materialUseOrderArmour;
	}

private:
	//ilvl 1340 set 
	inline float getMediumBaseHoningCostWeapon(int targetItemHoningLevel)
	{
		destructionStoneCost = 358;
		weaponLeapstoneCost = 6;
		int fusionMat = 4;
		rawGoldCost = 300;
		int silverCost = 27500;
		float honorShards = 84;

		if (targetItemHoningLevel >= 2)
		{
			weaponLeapstoneCost += 2;
			silverCost += 900;
		}
		if (targetItemHoningLevel >= 4)
		{
			destructionStoneCost += 158;
			weaponLeapstoneCost += 2;
			fusionMat += 2;
			silverCost += 900;
			honorShards += 36;
		}
		if (targetItemHoningLevel >= 6)
		{
			weaponLeapstoneCost += 2;
			silverCost += 1800;
			rawGoldCost += 20;
		}
		if (targetItemHoningLevel >= 7)
		{
			destructionStoneCost += 156;
			silverCost += 900;
			honorShards += 30;
		}
		if (targetItemHoningLevel >= 8)
		{
			weaponLeapstoneCost += 2;
			silverCost += 900;
		}
		if (targetItemHoningLevel >= 9)
		{
			fusionMat += 2;
			silverCost += 900;
		}
		if (targetItemHoningLevel >= 10)
		{
			destructionStoneCost += 158;
			honorShards += 36;
			weaponLeapstoneCost += 2;
			silverCost += 900;
		}
		if (targetItemHoningLevel >= 11)
		{
			silverCost += 900;
			rawGoldCost += 10;
		}
		if (targetItemHoningLevel >= 12)
		{
			weaponLeapstoneCost += 2;
			silverCost += 900;
		}
		if (targetItemHoningLevel >= 13)
		{
			destructionStoneCost += 138;
			fusionMat += 2;
			silverCost += 900;
			honorShards += 36;
		}
		if (targetItemHoningLevel >= 14)
		{
			weaponLeapstoneCost += 2;
			silverCost += 1400;
		}
		if (targetItemHoningLevel >= 16)
		{
			destructionStoneCost += 176;
			weaponLeapstoneCost += 2;
			honorShards += 82;
			fusionMat += 2;
			silverCost += 1400;
			rawGoldCost += 80;
		}
		if (targetItemHoningLevel >= 17)
		{
			weaponLeapstoneCost += 2;
			honorShards += 112;
			fusionMat += 2;
			silverCost += 900;
			rawGoldCost += 70;
		}
		if (targetItemHoningLevel >= 18)
		{
			weaponLeapstoneCost += 4;
			honorShards += 150;
			fusionMat += 2;
			silverCost += 900;
			rawGoldCost += 60;
		}
		if (targetItemHoningLevel >= 19)
		{
			destructionStoneCost += 156;
			weaponLeapstoneCost += 2;
			honorShards += 204;
			fusionMat += 2;
			silverCost += 900;
			rawGoldCost += 100;
		}
		if (targetItemHoningLevel >= 20)
		{
			weaponLeapstoneCost += 2;
			honorShards += 278;
			fusionMat += 2;
			silverCost += 900;
			rawGoldCost += 90;
		}

		float goldCost = rawGoldCost + silverCost * (1.0f / static_cast<float>(HoningConfig::silverAmountPerGold));
		goldCost += weaponLeapstoneCost * getGreaterHonorLeapstoneCost() + fusionMat * getBasicFusionCost();
		goldCost += destructionStoneCost * getDestructionStoneCost();
		goldCost += honorShards / 500.0f * getSmallHonorShardPouchCost();
		return goldCost;
	}

	//ilvl 1340 set 
	inline float getMediumBaseHoningCostArmour(int targetItemHoningLevel)
	{
		guardianStoneCost = 216;
		armourLeapstoneCost = 4;
		int fusionMat = 4;
		rawGoldCost = 160;
		int silverCost = 19300;
		float honorShards = 58;

		if (targetItemHoningLevel >= 2)
		{
			armourLeapstoneCost += 2;
			silverCost += 500;
		}
		if (targetItemHoningLevel >= 4)
		{
			guardianStoneCost += 94;
			silverCost += 1000;
			honorShards += 24;
			rawGoldCost += 10;
		}
		if (targetItemHoningLevel >= 6)
		{
			armourLeapstoneCost += 2;
			silverCost += 1000;
		}
		if (targetItemHoningLevel >= 7)
		{
			guardianStoneCost += 94;
			fusionMat += 2;
			silverCost += 500;
			honorShards += 26;
		}
		if (targetItemHoningLevel >= 8)
		{
			armourLeapstoneCost += 2;
			silverCost += 400;
		}
		if (targetItemHoningLevel >= 10)
		{
			guardianStoneCost += 94;
			honorShards += 24;
			fusionMat += 2;
			silverCost += 1000;
		}
		if (targetItemHoningLevel >= 12)
		{
			armourLeapstoneCost += 2;
			silverCost += 1200;
		}
		if (targetItemHoningLevel >= 13)
		{
			guardianStoneCost += 94;
			honorShards += 26;
			silverCost += 1000;
		}
		if (targetItemHoningLevel >= 15)
		{
			rawGoldCost += 10;
			silverCost += 1000;
		}
		if (targetItemHoningLevel >= 16)
		{
			guardianStoneCost += 94;
			honorShards += 58;
			armourLeapstoneCost += 2;
			fusionMat += 2;
			rawGoldCost += 30;
			silverCost += 700;
		}
		if (targetItemHoningLevel >= 17)
		{
			honorShards += 76;
			armourLeapstoneCost += 2;
			rawGoldCost += 40;
			silverCost += 600;
		}
		if (targetItemHoningLevel >= 18)
		{
			honorShards += 104;
			fusionMat += 2;
			rawGoldCost += 30;
			silverCost += 600;
		}
		if (targetItemHoningLevel >= 19)
		{
			guardianStoneCost += 94;
			honorShards += 140;
			armourLeapstoneCost += 2;
			fusionMat += 2;
			rawGoldCost += 40;
			silverCost += 600;
		}
		if (targetItemHoningLevel >= 20)
		{
			honorShards += 192;
			armourLeapstoneCost += 2;
			rawGoldCost += 30;
			silverCost += 600;
		}

		float goldCost = rawGoldCost + silverCost * (1.0f / static_cast<float>(HoningConfig::silverAmountPerGold));
		goldCost += armourLeapstoneCost * getGreaterHonorLeapstoneCost() + fusionMat * getBasicFusionCost();
		goldCost += guardianStoneCost * getGuardianStoneCost();
		goldCost += honorShards / 500.0f * getSmallHonorShardPouchCost();
		return goldCost;
	}

	inline float getLowBaseHoningCostWeapon(int targetItemHoningLevel)
	{
		destructionStoneCost = 138;
		weaponLeapstoneCost = 4;
		int fusionMat = 0;
		rawGoldCost = 0;
		int silverCost = 16000;
		float honorShards = 50;

		if ( targetItemHoningLevel >= 3 )
		{
			destructionStoneCost += 60;
			weaponLeapstoneCost += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 4 )
		{
			fusionMat += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 7 )
		{
			destructionStoneCost += 60;
			weaponLeapstoneCost += 2;
			fusionMat += 2;
			rawGoldCost += 120;
			silverCost += 1000;
			honorShards += 30;
		}
		if ( targetItemHoningLevel >= 10 )
		{
			destructionStoneCost += 62;
			weaponLeapstoneCost += 2;
			silverCost += 2000;
		}
		if ( targetItemHoningLevel >= 13 )
		{
			destructionStoneCost += 60;
			fusionMat += 2;
			silverCost += 1000;
		}
		if ( targetItemHoningLevel >= 14 )
		{
			weaponLeapstoneCost += 2;
			silverCost += 1000;
		}

		float goldCost = rawGoldCost + silverCost * ( 1.0f / static_cast<float>( HoningConfig::silverAmountPerGold ) );
		goldCost += weaponLeapstoneCost * getHonorLeapstoneCost() + fusionMat * getSimpleFusionCost();
		goldCost += destructionStoneCost * getDestructionStoneCost();
		goldCost += honorShards / 500.0f * getSmallHonorShardPouchCost();
		return goldCost;
	}

	inline float getLowBaseHoningCostArmour(int targetItemHoningLevel)
	{
		guardianStoneCost = 82;
		armourLeapstoneCost = 2;
		int fusionMat = 0;
		rawGoldCost = 0;
		int silverCost = 11000;
		float honorShards = 28;

		if ( targetItemHoningLevel >= 3 )
		{
			armourLeapstoneCost += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 4 )
		{
			guardianStoneCost += 38;
			fusionMat += 2;
			silverCost += 500;
		}
		if ( targetItemHoningLevel >= 7 )
		{
			guardianStoneCost += 36;
			rawGoldCost += 70;
			silverCost += 500;
			honorShards += 27;
		}
		if ( targetItemHoningLevel >= 10 )
		{
			guardianStoneCost += 36;
			armourLeapstoneCost += 2;
			fusionMat += 2;
			silverCost += 1000;
		}
		if ( targetItemHoningLevel >= 13 )
		{
			guardianStoneCost += 36;
			silverCost += 1000;
		}
		if ( targetItemHoningLevel >= 14 )
		{
			armourLeapstoneCost += 2;
			silverCost += 1000;
		}

		float goldCost = rawGoldCost + silverCost * ( 1.0f / static_cast<float>( HoningConfig::silverAmountPerGold ) );
		goldCost += armourLeapstoneCost * getHonorLeapstoneCost() + fusionMat * getSimpleFusionCost();
		goldCost += guardianStoneCost * getGuardianStoneCost();
		goldCost += honorShards / 500.0f * getSmallHonorShardPouchCost();
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