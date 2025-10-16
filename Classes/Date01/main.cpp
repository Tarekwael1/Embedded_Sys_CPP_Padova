#include <iostream>
using std::cout;
using std::endl;
#include "Date.h"

int main()
{
	Date today;
	init_day(today,1978,6,25);
	Date tomorrow = today;
	add_day(tomorrow,1);
	cout << "Today: " << today << endl;
	cout << "Tomorrow: " << tomorrow << endl;
	Date invalid_date;
	init_day(invalid_date,2004,13,-5);
}
