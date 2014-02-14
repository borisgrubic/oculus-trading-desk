#ifndef SOURCE_READER_H
#define SOURCE_READER_H

#include <iostream>

using namespace std;

/*
 * Class SourceReader
 *  - abstract class that all source readers should extend
 */
class SourceReader {
	public:
		/*
		 * returns next portion of data from the source
 		*/
	    virtual string ReadNextData() = 0;

	    virtual ~SourceReader() {}
};

#endif
