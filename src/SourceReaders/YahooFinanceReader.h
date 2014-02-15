#ifndef YAHOO_FINANCE_READER_H
#define YAHOO_FINANCE_READER_H

#include <cstdio>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>

#include "SourceReader.h"
#include "FileReader.h"
#include "Utils/Date.h"
#include "Exceptions/Exception.h"

class YahooFinanceReader : public SourceReader {
	private:
        string ticker;
        Date from, to;
        FileReader* fileReader;

	public:
		YahooFinanceReader(string ticker);
		~YahooFinanceReader();
		string ReadNextData();
};

#endif
