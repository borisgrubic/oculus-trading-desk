#include <iostream>
#include <vector>
#include <cstdio>

#include "TestDataTypes.h"

#include "../src/DataTypes/StringDataType.h"
#include "../src/DataTypes/CSVDataType.h"
#include "../src/Exceptions/Exception.h"

using namespace std;

bool TestDataTypes::TestStringDataType(string str, string result) {
	bool ok = true;

	DataType* dataType = new StringDataType(str);
	StringDataType* stringDataType = (StringDataType*)dataType;

	string getString = stringDataType->GetString();
	if (getString != result)
		ok = false;

	delete dataType;

	return ok;
}

bool TestDataTypes::TestStringDataType() {
	bool ok = true;
	printf("Testing StringDataType... ");

	ok &= TestStringDataType("Hello", "Hello");
	ok &= TestStringDataType("", "");
	ok &= TestStringDataType("test test", "test test");

	if (ok) printf("OK!\n");
	else printf("Fail!\n");

	return ok;
}

bool TestDataTypes::TestCSVDataType(string str, vector<string> colNames, vector<string> results) {
	bool ok = true;

	DataType* dataType = new CSVDataType(str, colNames);
	CSVDataType* csvDataType = (CSVDataType*) dataType;

	for (int i = 0; i < colNames.size(); ++i) {
		string value = csvDataType->GetValue(colNames[i]);
		string value2 = csvDataType->GetValue(i);

		if (value != value2 || value != results[i])
			ok = false;
	}

	delete dataType;

	return ok;
}

bool TestDataTypes::TestCSVDataType() {
	bool ok = true;
	printf("Testing CSVDataType... ");

	vector<string> colNames; colNames.clear();
	colNames.push_back("First Name");
	colNames.push_back("Last Name");
	colNames.push_back("University");
	colNames.push_back("Course");
	colNames.push_back("Year");

	vector<string> results; results.clear();
	results.push_back("Boris");
	results.push_back("Grubic");
	results.push_back("Cambridge");
	results.push_back("CS");
	results.push_back("IB");

	ok &= TestCSVDataType("Boris,Grubic,Cambridge,CS,IB", colNames, results);

	try {
		TestCSVDataType("Boris,Grubic,Cambridge,CS!IB", colNames, results);

		// we should get exception
		ok = false;
	} catch (Exception e) {
		if (string(e.what()) != "CSVDataType string not well formatted")
			ok = false;
	}

	results.clear();
	for (int i = 0; i < 5; ++i) results.push_back("");

	ok &= TestCSVDataType(",,,,", colNames, results);

	colNames.clear();
	colNames.push_back("Test");

	results.clear();
	results.push_back("test");
	ok &= TestCSVDataType("test", colNames, results);

	try {
		TestCSVDataType("test,test", colNames, results);

		// we should get exception
		ok = false;
	} catch (Exception e) {
		if (string(e.what()) != "CSVDataType string not well formatted")
			ok = false;
	}

	if (ok) printf("OK!\n");
	else printf("Fail!\n");

	return ok;
}

bool TestDataTypes::Test() {
	bool ok = true;

	ok &= TestStringDataType();
	ok &= TestCSVDataType();

	return ok;
}