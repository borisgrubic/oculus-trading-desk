#include "CSVDataReader.h"
#include "../DataTypes/CSVDataType.h"

CSVDataReader::CSVDataReader(SourceReader *sourceReader) :
	DataReader(sourceReader)
{
	colNames = CSVDataType::ExtractValues(ReadNextLine());
}

DataType* CSVDataReader::GetNextData() {
	string nextLine = ReadNextLine();
	if (nextLine == "")
		return NULL;
	return new CSVDataType(nextLine, colNames);
}