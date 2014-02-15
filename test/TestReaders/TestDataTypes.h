#ifndef TEST_DATA_TYPES_H
#define TEST_DATA_TYPES_H

#include <iostream>
#include <vector>

using namespace std;

class TestDataTypes {
	public:
		bool TestStringDataType(string str, string result);
		bool TestStringDataType();
		bool TestCSVDataType(string str, vector<string> colNames, vector<string> results);
		bool TestCSVDataType();
		bool Test();
};

#endif