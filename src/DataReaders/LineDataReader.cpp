#include "LineDataReader.h"
#include "../DataTypes/StringDataType.h"

LineDataReader::LineDataReader(SourceReader *sourceReader) :
	DataReader(sourceReader)
{
}

DataType* LineDataReader::GetNextData() {
	return new StringDataType(ReadNextLine());
}