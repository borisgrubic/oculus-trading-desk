#include "TestDataTypes.h"
#include "TestSourceReaders.h"
#include "TestDataReaders.h"

int main() {
	TestDataTypes testDataTypes;
	testDataTypes.Test();

	TestSourceReaders testSourceReaders;
	testSourceReaders.Test();

	TestDataReaders testDataReaders;
	testDataReaders.Test();
	return 0;
}