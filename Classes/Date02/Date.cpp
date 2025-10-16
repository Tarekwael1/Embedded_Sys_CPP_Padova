#include <ostream>
using std::ostream;
#include <iostream>
using std::cerr;
#include "Date.h"

// helper functions
Date::Date(int y, int m, int d)
	:y{y}, m{m}, d{d}
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
ostream& operator<<(ostream& os, const Date& d)
{
	return os << '(' << d.y
		<< ',' << d.m
		<< ',' << d.d << ')';
}
