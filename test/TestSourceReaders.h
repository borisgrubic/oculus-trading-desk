#ifndef TEST_SOURCE_READERS_H
#define TEST_SOURCE_READERS_H

#include <iostream>

using namespace std;

class TestSourceReaders {
	public:
		bool TestFileReader(string filename, vector<string> result);
		bool TestFileReader();
		bool TestRandomCSVSimpleReader(vector<string>, bool);
		bool TestRandomCSVSimpleReader();
		bool Test();
};

#endif