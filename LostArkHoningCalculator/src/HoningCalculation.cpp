#include "LostArkHoningCalculator/HoningCalculation.h"

void calculateHoningForLevel(int currentItemHoningLevel, bool isIlvl1340Set)
{
	HoningParameter honingParameter {currentItemHoningLevel + 1, isIlvl1340Set};

	BestHoningChain honingChainWeapon;
	BestHoningChain honingChainArmour;

	std::cout << "Calculating..." << std::endl;
	std::thread t1([&] ()
	{
		honingChainWeapon = startHoningChain(honingParameter, true);
	});
	std::thread t2([&] ()
	{
		honingChainArmour = startHoningChain(honingParameter, false);
	});

	BestHoningChain fullChainWeapon = getFullChain(honingParameter, true, HoningInput::getFullInput(honingParameter));
	BestHoningChain emptyChainWeapon = getFullChain(honingParameter, true, HoningInput::getEmptyInput());
	BestHoningChain fullChainArmour = getFullChain(honingParameter, false, HoningInput::getFullInput(honingParameter));
	BestHoningChain emptyChainArmour = getFullChain(honingParameter, false, HoningInput::getEmptyInput());
	t1.join();
	t2.join();

	setConsoleColour(13);

	std::cout << std::endl << "Honing Calculation for your Gear from +" << currentItemHoningLevel << " to +" << currentItemHoningLevel + 1 << std::endl << std::endl;

	setConsoleColour(15);

	if (HoningConfig::useMarysShopPrices)
		std::cout << "Marys shop prices will also be used for this calculation (you can change this in the config)" << std::endl;
	else
		std::cout << "Marys shop prices will not be used for this calculation (you can change this in the config)" << std::endl;

	setConsoleColour(10);
	honingChainWeapon.output();
	setConsoleColour(12);
	if (!honingChainWeapon.hasSameMaterialsUsed(fullChainWeapon))
		fullChainWeapon.output();
	if (!honingChainWeapon.hasSameMaterialsUsed(emptyChainWeapon))
		emptyChainWeapon.output();
	setConsoleColour(10);
	honingChainArmour.output();
	setConsoleColour(12);
	if (!honingChainArmour.hasSameMaterialsUsed(fullChainArmour))
		fullChainArmour.output();
	if (!honingChainArmour.hasSameMaterialsUsed(emptyChainArmour))
		emptyChainArmour.output();

	std::cout << std::endl;

	//double finalGoldCost = ( 1 / ( totalSuccessRate / 100 ) ) * tempGoldCost;

	setConsoleColour(15);
}