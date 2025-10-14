// A set stores unique elements where they:

// Are sorted automatically in ascending order.
// Are unique, meaning equal or duplicate values are ignored.
// Can be added or removed, but the value of an existing element cannot be changed.
// Cannot be accessed by index numbers, because the order is based on sorting and not indexing.
#include <iostream>
#include <set>
#include <string>
#include <algorithm>

using namespace std;


int main()
{
    //The type of the set (e.g. string and int in the examples above) cannot be changed after its been declared.
    set<string> cars = {"Volvo", "BMW", "Ford", "Mazda", "BMW", "Volvo"};
    set<int> numbers = {1, 7, 3, 2, 5, 9};
    //If you want to reverse the order, you can use the greater<type> functor inside the angle brackets
    set<int, greater<int>> rev_numbers = {1, 7, 3, 2, 5, 9};


    // Print all the elements in the set
    cout << "The set contains: \n";
    for (string car : cars) 
    {
        cout << car << endl;
    }
    cout << "\n";

    // Print set elements 
    for (int num : numbers) 
    {
        cout << num << "\n";
    }
    cout << "\n";

    // Print reversed set elements
    for (int num : rev_numbers) 
    {
        cout << num << "\n";
    }   
    cout << "\n";
    
    // Add an element
    numbers.insert(4);
    rev_numbers.insert(4);

    // Add new elements
    cars.insert("Tesla");
    cars.insert("VW");
    cars.insert("Toyota");
    cars.insert("Audi");

    cout << "The set after adding new elements: \n";
    for (string car : cars) 
    {
        cout << car << endl;
    }
    cout << "\n";

    // Remove an element
    cars.erase("Ford");
    numbers.erase(3);
    rev_numbers.erase(3);

    // Remove elements
    cars.erase("Volvo");
    cars.erase("Mazda");

    cout << "The set after removing some elements: \n";
    for (string car : cars) 
    {
        cout << car << endl;
    }
    cout << "\n";

    

    return 0;
}


