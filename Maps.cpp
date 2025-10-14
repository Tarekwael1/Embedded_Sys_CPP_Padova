// A map stores elements in "key/value" pairs.

// Elements in a map are:

// Accessible by keys (not index), and each key is unique.
// Automatically sorted in ascending order by their keys (is a little bit like a set).

#include <iostream>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    // Create a map called people that will store strings as keys and integers as values
    map<string, int> people{ {"John", 32}, {"Adele", 45}, {"Bo", 29} };

    // Get the value associated with the key "John"
    cout << "John is: " << people["John"] << "\n";

    // Get the value associated with the key "Adele"
    cout << "Adele is: " << people["Adele"] << "\n";

    //The .at() function is often preferred over square brackets [] because it throws an error message if the element does not exist:
     // Get the value associated with the key "Adele"
    cout << "Adele is: " << people.at("Adele") << "\n";

    // Get the value associated with the key "Bo"
    cout << "Bo is: " << people.at("Bo") << "\n";

    // Change the value associated with the key "John"
    // Change John's value to 50 instead of 32
    people["John"] = 50;

    cout << "John is: " << people["John"];  // Now outputs John is: 50
    cout << "\n";

    // Change Bo's value to 50 instead of 32
    people.at("Bo") = 50;

    cout << "Bo is: " << people.at("Bo");  // Now outputs Bo is: 50
    cout << "\n";

    // Add new elements 
    people["Zoe"] = 21;
    people["Mia"] = 29;
    people["Oliver"] = 25;
    people["Noah"] = 26;

    // Add new elements using insert()
    people.insert({"Jenny", 22});
    people.insert({"Liam", 24});
    people.insert({"Kasper", 20});
    people.insert({"Anja", 30});

    // Trying to add two elements with equal keys
    people.insert({"Jenny", 30});

    // Remove an element by key
    people.erase("John");

    cout << "Size of the map is: " << people.size();  // Outputs 10
    cout << "\n";

    
    cout << "Is John exists?? "<< people.count("John");  // Outputs 1 (John exists)
    cout << "\n";

    // You should use the auto keyword (introduced in C++ version 11) inside the for loop. This allows the compiler to automatically determine the correct data type for each key-value pair.
    // Since map elements consist of both keys and values, you have to include .first to access the keys, and .second to access values in the loop.
    // Elements in the map are sorted automatically in ascending order by their keys:

    for (auto person : people) {
        cout << person.first << " is: " << person.second << "\n";
    }
    cout << "\n";
//If you want to reverse the order, you can use the greater<type> functor inside the angle brackets, like this:




    return 0;
}