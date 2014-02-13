#include "Date.h"

Date::Date(int day, int month, int year) :
    day(day),
    month(month),
    year(year)
{
}

string Date::GetDay() {
    string dayString = IntToString(day);
    if (day < 10) dayString = "0" + dayString;
    return dayString; 
}

string Date::GetMonth() {
    string monthString = IntToString(day);
    if (day < 10) monthString = "0" + monthString;
    return monthString; 
}

string Date::GetYear() {
    string yearString = IntToString(year);
    return yearString;
}

string Date::IntToString(int x) {
    if (x == 0) return "0";
    string ret = "";
    while (x > 0) {
        int dig = x % 10;
        ret += (char)('0' + dig);
        x /= 10;
    }
    return string(ret.rbegin(), ret.rend());
}
