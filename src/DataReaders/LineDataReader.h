#ifndef STRING_DATA_READER_H
#define STRING_DATA_READER_H

#include "DataReader.h"

class LineDataReader : public DataReader {
	public:
		LineDataReader(SourceReader*);

		/*
		 * returns next formatted data
		 * - LineDataReader.GetNextData() returns a StringDataType type
		 * 	 containing next line
 		*/
		DataType* GetNextData();
};

#endif