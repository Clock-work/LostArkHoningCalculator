#include "Config.h"

struct ChaosExchangeMat
{
	std::string name;
	float marketPrice;
	std::vector<int> shardCosts;

	ChaosExchangeMat(const std::string& name, float marketPrice, std::vector<int> shardCosts)
		: name(name), marketPrice(marketPrice), shardCosts(shardCosts)
	{

	}
};

void printChaosExchange()
{
	std::vector<ChaosExchangeMat> mats;
	mats.emplace_back("Honor Leapstone", MarketPrices::honorLeapstone, std::vector<int>{100, 130, 170, 200, 230});
	mats.emplace_back("Greater Honor Leapstone", MarketPrices::greaterHonorLeapstone, std::vector<int>{130, 170, 200, 240, 280});

}