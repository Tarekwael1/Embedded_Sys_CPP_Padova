#include <iostream>
using std::cout;
using std::endl;
#include "Date.h"

int main()
{
	Date today{1978,Date::Month::jun,25};  // colloquial style
	Date tomorrow = today;  // copy constructor
	tomorrow.add_day(1);
	cout << "Today: " << today << endl;
	cout << "Tomorrow: " << tomorrow << endl;
	Date invalid_date = Date{2004,Date::Month::dec,-5};   // verbose style
}
