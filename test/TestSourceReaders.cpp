#include <iostream>
#include <vector>

#include "TestSourceReaders.h"
#include "../src/SourceReaders/SourceReader.h"
#include "../src/SourceReaders/FileReader.h"
#include "../src/SourceReaders/RandomCSVSimpleReader.h"
#include "../src/Exceptions/FileNotFoundException.h"
#include "../src/Exceptions/Exception.h"
#include "../src/DataTypes/CSVDataType.h"

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

	for (int i = 0; ok && i < 5; ++i) {
		string str = sourceReader->ReadNextData();
		vector<string> res = CSVDataType::ExtractValues(str);

		if (res.size() != 4) {
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

bool TestSourceReaders::Test() {
	bool ok = true;

	ok &= TestFileReader();
	ok &= TestRandomCSVSimpleReader();

	return ok;
}