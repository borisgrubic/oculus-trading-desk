#include "DataReader.h"

DataReader::DataReader(SourceReader *sourceReader) :
	buffer(""),
	sourceReader(sourceReader)
{
}

string DataReader::ReadNextLine() {
	string ret = "";
	bool found = false;

	// read next line from a source
	while (!found) {
		for (int i = 0; !found && i < buffer.size(); ++i) {
			if (buffer[i] == '\n') {
				buffer = buffer.substr(i + 1, buffer.size() - (i + 1));
				found = true;
			} else {
				ret += buffer[i];
			}
		}

		if (!found) {
			buffer = sourceReader->ReadNextData();
			if (buffer == "")
				found = true;
		}
	}

	return ret;
}