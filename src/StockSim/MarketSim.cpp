#include <time.h>
using namespace std;
double frand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
class MarketSim
{
private:
    vector<Stock*> bidArray;
    vector<Stock*> askArray;
    unsigned int marketSize;
public:
    MarketSim(double maxPrice, double maxVolume, unsigned int sizeOfMarket)
    {
        srand(time(NULL));
        marketSize = sizeOfMarket;
        double oldStockPrice = frand(0.0, maxPrice);
        double newStockPrice;
        double volatility = frand(0.01, 0.12);
        double rnd;
        double change_percent;
        double change_amount;
        int sign;
        for(int i = 0; i < marketSize; i++)
        {
            rnd = frand(0.0,1.0);
            sign = rand() % 10;
            if(sign > 4)
            {
                sign = -1;
            }
            else
            {
                sign = 1;
            }
            change_percent = 2 * volatility * rnd;
            if(change_percent > volatility)
            {
                change_percent -= (2*volatility);
            }
            change_amount = oldStockPrice * change_percent;
            newStockPrice = oldStockPrice + (change_amount*sign);
            Stock* tempStock = new Stock(newStockPrice, frand(0, maxVolume));
            bidArray.push_back(tempStock);
            oldStockPrice = newStockPrice;
        }
        oldStockPrice = frand(0, maxPrice);
        for(int i = 0; i < marketSize; i++)
        {
            rnd = frand(0.0,1.0);
            sign = rand() % 10;
            if(sign > 4)
            {
                sign = -1;
            }
            else
            {
                sign = 1;
            }
            change_percent = 2 * volatility * rnd;
            if(change_percent > volatility)
            {
                change_percent -= (2*volatility);
            }
            change_amount = oldStockPrice * change_percent;
            newStockPrice = oldStockPrice + (change_amount*sign);
            Stock* tempStock = new Stock(newStockPrice, frand(0, maxVolume));
            askArray.push_back(tempStock);
            oldStockPrice = newStockPrice;
        }
    }
    vector<Stock*> getBidData()
    {
        return bidArray;
    }
    vector<Stock*> getAskData()
    {
        return askArray;
    }
    unsigned int getMarketSize()
    {
        return marketSize;
    }
};

