#include "CSVDataType.h"

#include "../Exceptions/Exception.h"

CSVDataType::CSVDataType(string dataString, vector<string> colNames) :
	colNames(colNames)
{
	colValues = ExtractValues(dataString);

	if (colValues.size() != colNames.size())
		throw Exception("CSVDataType string not well formatted");
}

vector<string> CSVDataType::ExtractValues(string dataString) {
	string cur = "";
	int length = dataString.size();
	vector<string> ret; ret.clear();

	for (int i = 0; i < length; ++i) {
		if (dataString[i] == ',') {
			ret.push_back(cur);
			cur = "";
		} else {
			cur += dataString[i];
		}
	}

	ret.push_back(cur);

	return ret;
}

string CSVDataType::GetValue(int column) {
	if (column < 0 || column >= colValues.size())
		throw Exception("Index out of range");
	return colValues[column];
}

string CSVDataType::GetValue(string colName) {
	for (int i = 0; i < colNames.size(); ++i)
		if (colNames[i] == colName)
			return colValues[i];
	throw Exception("Non existing column name");
}


vector<string> CSVDataType::GetColNames() {
	return colNames;
}