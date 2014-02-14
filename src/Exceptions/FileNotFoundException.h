#ifndef FILE_NOT_FOUND_EXCEPTION_H
#define FILE_NOT_FOUND_EXCEPTION_H

#include <iostream>
#include <exception>

using namespace std;

class FileNotFoundException : public exception {
	public:
		virtual const char* what() const throw() {
			return "File not found";
		}
};

#endif