#ifndef DATE_H
#define DATE_H

#include <ostream>

struct Date {
	int y; // year
	int m; // month in year
	int d; // day of month
};

// helper functions
void init_day(Date& dd, int y, int m, int d);
void add_day(Date& dd, int m);

// operators
std::ostream& operator<<(std::ostream& os, const Date& d);

#endif
