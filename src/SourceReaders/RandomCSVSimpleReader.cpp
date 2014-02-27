#include "RandomCSVSimpleReader.h"

#include "Exceptions/Exception.h"

#include <cstdlib>

const string RandomCSVSimpleReader::randomCompanies[] = {"GOOG", "FB", "AAPL", "MSFT", "AMZN"};
const int RandomCSVSimpleReader::maxRndCompanies = 5;

RandomCSVSimpleReader::RandomCSVSimpleReader() :
	first(true)
{
	srand(time(NULL));
	int compNum = 5; // rand() % maxRndCompanies + 1;

	for (int i = 0; i < compNum; ++i) {
		companies.push_back(randomCompanies[i]);
	}
}

RandomCSVSimpleReader::RandomCSVSimpleReader(vector<string> companies) :
	first(true),
	companies(companies)
{
	if (companies.empty()) {
		throw Exception("Number of companies can't be 0");
	}
}

string RandomCSVSimpleReader::ReadNextData() {
	if (first) {
		first = false;
		return "type,company,cost,volume\n";
	} else {
		string type = ((rand() % 10 < 5) ? "sell" : "buy");
		string company = companies[rand() % companies.size()];
		int cost;
		if (type == "buy") {
			cost = 91 + (rand() % 15);
		} else {
			cost = 119 - (rand() % 15);
		}
		int volume = rand() % 10 + 1;
		return type + "," + company + "," + IntToString(cost) + "," + IntToString(volume) + "\n";
	}
}

string RandomCSVSimpleReader::IntToString(int x) {
	if (x == 0) return "0";
	string ret = "";
	while (x > 0) {
		int dig = x % 10;
		ret += (char)('0' + dig);
		x /= 10;
	}
	return string(ret.rbegin(), ret.rend());
}
