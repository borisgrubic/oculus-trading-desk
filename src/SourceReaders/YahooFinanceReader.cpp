#include "YahooFinanceReader.h"

using namespace std;

YahooFinanceReader::YahooFinanceReader(string ticker) :
	ticker(ticker),
    from(1, 1, 2013),
    to(1, 1, 2014)
{
    string filename = "/tmp/" + ticker + from.GetMonth() + from.GetDay() + from.GetYear() + to.GetMonth() + to.GetDay() + to.GetYear();

    if (FILE *file = fopen(filename.c_str(), "r")) {
        fclose(file);
    } else {
        // our request
        string request = "wget -O " + filename + " \"http://ichart.finance.yahoo.com/table.csv?s=" + ticker
                + "&a=" + from.GetMonth()
                + "&b=" + from.GetDay()
                + "&c=" + from.GetYear()
                + "&d=" + to.GetMonth()
                + "&e=" + to.GetDay()
                + "&f=" + to.GetYear()
                + "&g=d\" --quiet";

        if (system(request.c_str())) {
            throw Exception("Could not download data");
        }
    } 

    string tmpFileName = filename;
    fileReader = new FileReader(tmpFileName);
}

YahooFinanceReader::~YahooFinanceReader() {
    delete fileReader;
}

string YahooFinanceReader::ReadNextData() {
    return fileReader->ReadNextData();
}
