#include "FileReader.h"
#include "../Exceptions/FileNotFoundException.h"

FileReader::FileReader(string filename) :
	filename(filename)
{
	// create a handle to a file
	fileHandle = fopen(filename.c_str(), "r");
	if (fileHandle == NULL)
		throw FileNotFoundException();

	buffer = NULL;
	// set default bufferSize
	SetBufferSize(1024);
}

FileReader::~FileReader() {
	if (fileHandle)
		fclose(fileHandle);

	if (buffer)
		delete[] buffer;
}

string FileReader::ReadNextData() {
	if (fileHandle == NULL) throw FileNotFoundException();

	// if we have no more data, return empty string
	if (fgets(buffer, bufferSize, fileHandle) == NULL)
		return string();

	return string(buffer);
}

void FileReader::SetBufferSize(int bufferSize) {
	this->bufferSize = bufferSize;

	// deallocate previously allocated buffer
	if (buffer != NULL)
		delete [] buffer;

	// allocate buffer
	buffer = new char[bufferSize];
}