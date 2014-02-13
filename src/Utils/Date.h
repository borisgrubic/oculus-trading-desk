#ifndef DATE_H
#define DATE_H

#include <iostream>

using namespace std;

class Date {
    private:
        int day;
        int month;
        int year;

    public:
        Date(int day, int month, int year);
        string GetDay();
        string GetMonth();
        string GetYear();
        static string IntToString(int x);
};

#endif
