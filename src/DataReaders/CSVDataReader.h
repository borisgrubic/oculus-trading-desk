#ifndef CSV_DATA_READER_H
#define CSV_DATA_READER_H

#include <iostream>
#include <vector>

#include "DataReader.h"

using namespace std;

class CSVDataReader : public DataReader {
	private:
		vector<string> colNames;

	public:
		CSVDataReader(SourceReader*);

		/*
		 * returns next formatted data
		 * - CSVDataReader.GetNextData() returns a CSVDataType type
		 * 	 containing next row of a csv
 		*/
		DataType* GetNextData();
};

#endif