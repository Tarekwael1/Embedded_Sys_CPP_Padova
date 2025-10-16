#ifndef DATE_H
#define DATE_H

#include <ostream>

// simple Date (control access)
class Date {
	int y, m, d;                // year,month,day
public:
	Date(int y, int m, int d);  // check for valid date and initialise
	void add_day(int n);        // increase the Date by n days
	int year() { return y; }
	int month() { return m; }
	int day() { return d; }
};

// operators
std::ostream& operator<<(std::ostream& os, Date& d);  // Date& d not const on purpose

#endif
