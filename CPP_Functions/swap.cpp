#include <iostream>
using std::cout;

void swap(double& d1, double& d2)
{
	auto temp{d1};      // copy d1's value to temp
	d1 = d2;               // copy d2's value to d1
	d2 = temp;             // copy d1's old value to d2
}

int main() {
	double x{1};
	double y{2};
	cout << "x==" << x << " y==" << y << "\n"; // write: x==1 y==2
	swap(x,y);
	cout << "x==" << x << " y==" << y << "\n"; // write: x==2 y==1

	return 0;
}
