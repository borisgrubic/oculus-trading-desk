#ifndef CSV_DATA_TYPE_H
#define CSV_DATA_TYPE_H

#include <iostream>
#include <vector>

#include "DataType.h"

using namespace std;

class CSVDataType : public DataType {
	private:
		vector<string> colNames;
		vector<string> colValues;

	public:
		CSVDataType(string, vector<string>);

		/*
		* returns the value of a specified column
		*/
		string GetValue(int);

		/*
		* returns the value of a specified column
		*/
		string GetValue(string);

		/*
		* returns all the column names
		*/
		vector<string> GetColNames();

		/*
		* extract values from a string containing csv
		*/
		static vector<string> ExtractValues(string);
};

#endif