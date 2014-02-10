#ifndef TEST_DATA_READERS_H
#define TEST_DATA_READERS_H

#include "../src/SourceReaders/SourceReader.h"
#include "../src/DataTypes/CSVDataType.h"
#include "../src/DataTypes/StringDataType.h"

class TestDataReaders {
	public:
		bool TestCSVDataReader(SourceReader*, vector<CSVDataType*>);
		bool TestCSVDataReader();
		bool TestLineDataReader(SourceReader*, vector<StringDataType>);
		bool TestLineDataReader();
		bool Test();
};

#endif