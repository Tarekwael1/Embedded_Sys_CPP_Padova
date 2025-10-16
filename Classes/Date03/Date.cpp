#include <ostream>
using std::ostream;
#include <iostream>
using std::cerr;
#include "Date.h"

// helper functions
Date::Date(int yy, int mm, int dd)
	:y{yy}, m{mm}, d{dd}
{
	if (m<1 || m>12 || d<1 || d>31) {
		cerr << "invalid date\n";
		exit(1);
	}
}

void Date::add_day(int n)
{
	d += n;
	if (d > 31) {
		cerr << "add_day() results in invalid date\n";
		exit(1);
	}
}

// operators
// Date& d not const because Date::year() etc. are not declared const yetq
// (I don't "know" about that yet)
ostream& operator<<(ostream& os, Date& d)
{
	return os << '(' << d.year()
		<< ',' << d.month()
		<< ',' << d.day() << ')';
}
