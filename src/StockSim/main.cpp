#include <iostream>
#include <stdlib.h>
#include <vector>
#include "Stock.cpp"
#include "MarketSim.cpp"
using namespace std;

int main()
{
    MarketSim sim(100.0,100.0,100.0);
    vector<Stock*> bidData= sim.getBidData();
    for(int i = 0; i < sim.getMarketSize(); i++)
    {
        double d = bidData.at(i)->getPrice();
        cout << d;
        cout << endl;
    }
    getwchar();
    return 0;
}
