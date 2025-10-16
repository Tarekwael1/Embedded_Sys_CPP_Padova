#include <ostream>
using std::ostream;
#include <iostream>
using std::cerr;
#include "Date.h"

// helper functions

// check for valid date and initialise
Date::Date(int yy, Month mm, int dd)
	:y{yy}, m{mm}, d{dd}
{
	if (m<Month::jan || m>Month::dec || d<1 || d>31)  {
		cerr << "invalid date\n";
		exit(1);
	}
}

// increase the Date by n days
void Date::add_day(int n)
{
	d += n;
	if (d > 31) {
		cerr << "add_day() results in invalid date\n";
		exit(1);
	}
}

// operators
// Date& d not const because Date::year() etc. are not declared const yet
// (I don't "know" about that yet)
ostream& operator<<(ostream& os, Date& d)
{
	return os << '(' << d.year()
		<< ',' << static_cast<int>(d.month())
		<< ',' << d.day() << ')';
}
