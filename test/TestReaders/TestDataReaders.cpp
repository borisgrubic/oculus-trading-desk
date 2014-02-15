#include <iostream>
#include <cstdio>
#include <vector>

#include "TestDataReaders.h"
#include "DataTypes/CSVDataType.h"
#include "DataReaders/DataReader.h"
#include "DataReaders/CSVDataReader.h"
#include "DataReaders/LineDataReader.h"
#include "SourceReaders/FileReader.h"

using namespace std;

bool TestDataReaders::TestCSVDataReader(SourceReader* sourceReader, vector<CSVDataType*> results) {
	bool ok = true;
	DataReader* dataReader = new CSVDataReader(sourceReader);

	for (int i = 0; i < results.size(); ++i) {
		CSVDataType* csvDataType = (CSVDataType*) dataReader->GetNextData();

		if (csvDataType == NULL || results[i] == NULL) {
			if (csvDataType != NULL || results[i] != NULL)
				ok = false;
		} else {
			if (csvDataType->GetColNames() != results[i]->GetColNames())
				ok = false;
			else {
				int size = csvDataType->GetColNames().size();
				for (int j = 0; j < size; ++j) {
					if (csvDataType->GetValue(j) != results[i]->GetValue(j)) {
						ok = false;
					}
				}
			}

			delete csvDataType;
		}
	}

	delete dataReader;
	return ok;
}

bool TestDataReaders::TestCSVDataReader() {
	bool ok = true;
	printf("Testing CSVDataReader... ");

	vector<string> colNames; colNames.clear();
	colNames.push_back("ColA");
	colNames.push_back("ColB");
	colNames.push_back("ColC");
	vector<CSVDataType*> res; res.clear();
	res.push_back(new CSVDataType("1,A,ABC", colNames));
	res.push_back(new CSVDataType("2,B,BCD", colNames));
	res.push_back(new CSVDataType("3,C,CDE", colNames));
	res.push_back(NULL);
	ok &= TestCSVDataReader(new FileReader("SimpleCSVFile.txt"), res);

	for (int i = 0; i < res.size() - 1; ++i)
		delete res[i];
	
	if (ok) printf("OK!\n");
	else printf("Fail!\n");

	return ok;
}

bool TestDataReaders::TestLineDataReader(SourceReader* sourceReader, vector<StringDataType> results) {
	bool ok = true;
	DataReader* dataReader = new LineDataReader(sourceReader);

	for (int i = 0; i < results.size(); ++i) {
		StringDataType* stringDataType = (StringDataType*) dataReader->GetNextData();

		if (stringDataType->GetString() != results[i].GetString())
			ok = false;

		delete stringDataType;
	}

	delete dataReader;
	return ok;
}

bool TestDataReaders::TestLineDataReader() {
	bool ok = true;
	printf("Testing LineDataReader... ");

	vector<StringDataType> result; result.clear();
	result.push_back(StringDataType("test 1"));
	result.push_back(StringDataType("test 2"));
	result.push_back(StringDataType(""));
	result.push_back(StringDataType(""));
	ok &= TestLineDataReader(new FileReader("SimpleFile.txt"), result);
	
	if (ok) printf("OK!\n");
	else printf("Fail!\n");

	return ok;
}

bool TestDataReaders::Test() {
	bool ok = true;

	ok &= TestCSVDataReader();
	ok &= TestLineDataReader();

	return ok;
}