#ifndef TEST_DATA_READERS_H
#define TEST_DATA_READERS_H

#include "SourceReaders/SourceReader.h"
#include "DataTypes/CSVDataType.h"
#include "DataTypes/StringDataType.h"

class TestDataReaders {
	public:
		bool TestCSVDataReader(SourceReader*, vector<CSVDataType*>);
		bool TestCSVDataReader();
		bool TestLineDataReader(SourceReader*, vector<StringDataType>);
		bool TestLineDataReader();
		bool Test();
};

#endif