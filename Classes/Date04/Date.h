#ifndef DATE_H
#define DATE_H

#include <ostream>

// simple Date (use Month type)
class Date {
public:
	enum class Month {
		jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
	};

	Date(int y, Month m, int d);    // check for valid date and initialise
	void add_day(int n);            // increase the Date by n days
	int year() { return y; }
	Month month() { return m; }
	int day() { return d; }
private:
	int y;      // year
	Month m;
	int d;      // day
};

// operators
std::ostream& operator<<(std::ostream& os, Date& d);  // Date& d not const on purpose

#endif
