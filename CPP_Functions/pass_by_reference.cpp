#include <iostream>
using std::cout;

// pass-by-reference (let the function refer back to the variable passed)
int f(int& x)
{
	x = x+1;
	return x;
}

int main()
{
	int xx{0};
	cout << f(xx) << '\n';  // write: 1
	cout << xx << '\n';     // write:1; f() changed the value of xx

	int yy{7};
	cout << f(yy) << '\n';  // write: 8
	cout << yy << '\n';     // write: 8; f() changed the value of yy

	return 0;
}
