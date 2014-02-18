#ifndef DATA_READER_H
#define DATA_READER_H

#include <iostream>

#include "SourceReaders/SourceReader.h"
#include "DataTypes/DataType.h"

using namespace std;

class DataReader {
	private:
		string buffer;

	protected:
		SourceReader *sourceReader;

	public:
		DataReader(SourceReader*); 

		/*
		 * returns next formatted data
 		*/
		virtual DataType* GetNextData() = 0;

		/*
		* returns next line from a source
		*/
		string ReadNextLine();

		virtual ~DataReader() {
			delete sourceReader;
		}
};

#endif