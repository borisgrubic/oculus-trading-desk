#include "TestDataTypes.h"
#include "TestSourceReaders.h"
#include "TestDataReaders.h"

int main(int argc, char *argv[]) {
	string test = string(argv[1]);
	bool passed = true;

	if (test == "TestDataTypes" || test == "All") {
		TestDataTypes testDataTypes;
		passed &= testDataTypes.Test();
	}

	if (test == "TestSourceReaders" || test == "All") {
		TestSourceReaders testSourceReaders;
		passed &= testSourceReaders.Test();
	}

	if (test == "TestDataReaders" || test == "All") {
		TestDataReaders testDataReaders;
		passed &= testDataReaders.Test();
	}

	if (passed) return 0;
	return 1;
}