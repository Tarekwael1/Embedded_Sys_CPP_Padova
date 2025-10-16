#ifndef DATE_H
#define DATE_H

#include <ostream>

// guarantee initialisation with constructor
// provide some notational convenience
struct Date {
	int y, m, d;                // year,month,day
	Date(int y, int m, int d);  // check for valid date and initialise
	void add_day(int n);        // increase the Date by n days
};

// operators
std::ostream& operator<<(std::ostream& os, const Date& d);

#endif
