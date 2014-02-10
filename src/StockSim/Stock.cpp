class Stock
{
private:
    double price;
    double volume;
public:
    Stock(double p = 0.0, double v = 0.0)
    {
        price = p;
        volume = v;
    }
    double getPrice()
    {
        return price;
    }
    double getVolume()
    {
        return volume;
    }
};
