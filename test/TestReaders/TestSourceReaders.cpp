#include <iostream>
#include <vector>

#include "TestSourceReaders.h"
#include "SourceReaders/SourceReader.h"
#include "SourceReaders/FileReader.h"
#include "SourceReaders/RandomCSVSimpleReader.h"
#include "SourceReaders/YahooFinanceReader.h"
#include "Exceptions/FileNotFoundException.h"
#include "Exceptions/Exception.h"
#include "DataTypes/CSVDataType.h"
#include "DataReaders/DataReader.h"
#include "DataReaders/CSVDataReader.h"

using namespace std;

bool TestSourceReaders::TestFileReader(string filename, vector<string> result) {
	bool ok = true;

	SourceReader* sourceReader = new FileReader(filename);

	for (int i = 0; i < result.size(); ++i) {
		string str = sourceReader->ReadNextData();
		if (str != result[i])
			ok = false;
	}

	delete sourceReader;
	return ok;
}

bool TestSourceReaders::TestFileReader() {
	bool ok = true;
	printf("Testing FileReader... ");

	vector<string> result; result.clear();
	result.push_back("test 1\n");
	result.push_back("test 2\n");
	result.push_back("\n");
	result.push_back("");
	ok &= TestFileReader("SimpleFile.txt", result);

	try {
		TestFileReader("NonExistingFile.txt", result);

		// we should get exception
		ok = false;
	} catch (FileNotFoundException e) {
	}

	if (ok) printf("OK!\n");
	else printf("Fail!\n");

	return ok;
}

bool IsNumber(string str) {
	for (int i = 0; i < str.size(); ++i)
		if (str[i] < '0' || str[i] > '9')
			return false;
	return true;
}

bool Contains(vector<string> companies, string company) {
	for (int i = 0; i < companies.size(); ++i)
		if (companies[i] == company)
			return true;
	return false;
}

bool TestSourceReaders::TestRandomCSVSimpleReader(vector<string> companies, bool empty) {
	bool ok = true;

	SourceReader* sourceReader;
	if (empty) {
		sourceReader = new RandomCSVSimpleReader();
	} else {
		sourceReader = new RandomCSVSimpleReader(companies);
	}

	for (int i = 0; ok && i < 7; ++i) {
		string str = sourceReader->ReadNextData();
		vector<string> res = CSVDataType::ExtractValues(str);

		if (res.size() != 4) {
			ok = false;
		} else {
			if (res[3][res[3].size() - 1] != '\n') ok = false;
			else {
				res[3] = res[3].substr(0, res[3].size() - 1);
			}

			if (i == 0) {
				if (res[0] != "type" || res[1] != "company" ||
					res[2] != "cost" || res[3] != "volume")
					ok = false;
			} else {
				if (res[0] != "sell" && res[0] != "buy") ok = false;
				if (!empty) {
					ok &= Contains(companies, res[1]);
				}
				if (!IsNumber(res[2])) ok = false;
				if (!IsNumber(res[3])) ok = false;
			}
		}
	}

	delete sourceReader;
	return ok;
}

bool TestSourceReaders::TestRandomCSVSimpleReader() {
	bool ok = true;
	printf("Testing CSVSimpleReader... ");

	vector<string> companies; companies.clear();
	companies.push_back("GOOG");
	companies.push_back("MSFT");
	ok &= TestRandomCSVSimpleReader(companies, false);

	companies.clear();
	try {
		TestRandomCSVSimpleReader(companies, false);

		// we should get exception
		ok = false;
	} catch (Exception e) {
		if (string(e.what()) != "Number of companies can't be 0")
			ok = false;
	}

	ok &= TestRandomCSVSimpleReader(companies, true);

	if (ok) printf("OK!\n");
	else printf("Fail!\n");

	return ok;
}


bool TestSourceReaders::TestYahooFinanceReader(string ticker) {
    bool ok = true;

    DataReader* dataReader = new CSVDataReader(new YahooFinanceReader(ticker));

    for (int i = 0; i < 5; ++i) {
        CSVDataType* csvDataType = (CSVDataType*) dataReader->GetNextData();
        vector<string> colName = csvDataType->GetColNames();

        if (colName.size() != 7) ok = false;
        if (ok) {
            if (colName[0] != "Date") ok = false;
        }

        delete csvDataType;
    }

	delete dataReader;
	return ok;
}

bool TestSourceReaders::TestYahooFinanceReader() {
    bool ok = true;
    printf("Testing YahooFinanceReader... ");

    ok &= TestYahooFinanceReader("MSFT");

    if (ok) printf("OK!\n");
    else printf("Fail!\n");

    return ok;
}

bool TestSourceReaders::Test() {
	bool ok = true;

	ok &= TestFileReader();
	ok &= TestRandomCSVSimpleReader();
    ok &= TestYahooFinanceReader();

	return ok;
}
