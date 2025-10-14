#include <iostream>
using std::cout;

void g(int a, int& r, const int& cr)
{
	++a;           // change the local a
	++r;           // change the object referred to by r
	int x{cr};     // read the object referred to by cr
}

int main()
{
	int x{0};
	int y{0};
	int z{0};

	g(x,y,z);     // x == 0; y == 1; z == 0
	g(1,2,3);     // error: reference argument r needs a variable to refer to
	g(1,y,3);     // OK: since cr is const we can pass a literal

	return 0;
}
