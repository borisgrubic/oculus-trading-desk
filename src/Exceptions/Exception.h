#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <exception>

using namespace std;

class Exception : public exception {
	private:
		string message;

	public:
		Exception(string message);

		~Exception() throw() {}

		const char* what() const throw();
};

#endif