#pragma once 
#include "HoningCalculation.h"

static void printBestTargetGearScoreSelection()
{
	if (isMore(HoningConfig::targetGearScore, 0.0f))
	{
		std::cout << "Computing the best item honing level (1-20) for each individual gear to reach your target Gearscore of " << to_string_noZeros(HoningConfig::targetGearScore) << std::endl;
		std::cout << "This can take a few Minutes!" << std::endl;

		bool isIlvl1340Set = HoningConfig::isIlvl1340Set;
		int highestIlvl = isIlvl1340Set ? HoningConfig::mediumSetMaxHoningLevel : HoningConfig::lowSetMaxHoningLevel;

		std::vector<BestHoningChain> bestWeaponChains;
		std::vector<BestHoningChain> bestArmourChains;
		bestWeaponChains.reserve(25);
		bestArmourChains.reserve(25);


		std::thread t1([&] ()
		{
			for (int honingLevel = 1; honingLevel <= highestIlvl; honingLevel++)
			{
				std::cout << "Calculating " << honingLevel << "/" << highestIlvl << "..." << std::endl;
				bestWeaponChains.push_back(startHoningChain(HoningParameter {honingLevel, isIlvl1340Set}, true));
			}
		});
		std::thread t2([&] ()
		{
			for (int honingLevel = 1; honingLevel <= highestIlvl; honingLevel++)
			{
				bestArmourChains.push_back(startHoningChain(HoningParameter {honingLevel, isIlvl1340Set}, false));
			}
		});


		static const float lowSetBaseGearScore = 1310;
		static const int lowSetBaseHoningLevel = 3;
		//at +0 base 
		static const float mediumSetBaseGearScore = 1340;
		static const int mediumSetBaseHoningLevel = 0;

	}
}