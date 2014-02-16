#ifndef RANDOM_CSV_SIMPLE_READER_H
#define RANDOM_CSV_SIMPLE_READER_H

#include <vector>

#include "SourceReader.h"

/*
* class RandomCSVSimpleReader
*	generates random CVS buy/sell data
* 	
*	data format:
*		type,company,cost,volume
*
*	where: type    = sell | buy
*		   company = string representing company
*		   cost    = integer representing price of the share
*		   volume  = number of shares
*/
class RandomCSVSimpleReader : public SourceReader {
	private:
		const static string randomCompanies[];
		const static int maxRndCompanies; // maximum number of random companies

		bool first = true;
		vector<string> companies;

	public:
		/*
		* generate random number of companies for which data
		* will be generated
		*/
		RandomCSVSimpleReader();

		/*
		* data will ge generated for companies given
		* in input vector<string>
		*/
		RandomCSVSimpleReader(vector<string>);

		/*
		* generate next data
		*/
		string ReadNextData();

		/*
		* get a string representation of a integer
		*/
		string IntToString(int);
};

#endif