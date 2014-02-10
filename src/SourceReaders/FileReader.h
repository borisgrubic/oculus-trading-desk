#ifndef FILE_READER_H
#define FILE_READER_H

#include <cstdio>

#include "SourceReader.h"

class FileReader : public SourceReader {
	private:
		string filename;
		FILE * fileHandle;
		char * buffer;

		int bufferSize;

	public:
		FileReader(string filename);
		~FileReader();
		string ReadNextData();
		void SetBufferSize(int bufferSize);
};

#endif