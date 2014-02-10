#ifndef STRING_DATA_TYPE_H
#define STRING_DATA_TYPE_H

#include <iostream>

#include "DataType.h"

using namespace std;

class StringDataType : public DataType {
	private:
		string dataString;

	public:
		StringDataType(string dataString) :
			dataString(dataString)
		{
		}

		string GetString() {
			return dataString;
		}
};

#endif