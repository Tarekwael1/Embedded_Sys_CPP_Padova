#include <ostream>
using std::ostream;
#include <iostream>
using std::cerr;
#include "Date.h"

 // helper functions
 void init_day(Date& dd, int y, int m, int d)
 {
 	if (m<1 || m>12 || d<1 || d>31) {
	    cerr << "invalid date\n";
		exit(1);
	}
	dd.y = y;
	dd.m = m;
	dd.d = d;
}

void add_day(Date& dd, int n)
{
	init_day(dd,dd.y,dd.m,dd.d+n);
}

// operators
ostream& operator<<(ostream& os, const Date& d)
{
	return os 	<< '(' << d.y
				<< ',' << d.m
				<< ',' << d.d << ')';
}
