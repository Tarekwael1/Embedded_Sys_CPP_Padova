// Iterators are used to access and iterate through elements of data structures (vectors, sets, etc.), by "pointing" to them.

// It is called an "iterator" because "iterating" is the technical term for looping.
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>

using namespace std;


int main()
{
    // Create a vector called cars that will store strings
    vector<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};
    // Create a vector iterator called it
    vector<string>::iterator it;

    // Create a set called numbers that will store integers
    set<int> numbers = {1, 7, 3, 2, 5, 9};
    // Create a set iterator called itset
    set<int>::iterator itset;

    // Create a list called cars that will store strings
    list<string> LL = {"Volvo", "BMW", "Ford", "Mazda"};

    // Create a deque called cars that will store strings
    deque<string> DD = {"Volvo", "BMW", "Ford", "Mazda"};

    // Create a set called cars that will store strings
    set<string> SS = {"Volvo", "BMW", "Ford", "Mazda"};

    // Create a map that will store strings and integers
    map<string, int> people = { {"John", 32}, {"Adele", 45}, {"Bo", 29} };



    // Loop through the vector with the iterator
    for (it = cars.begin(); it != cars.end() ; ++it) 
    {
        cout << *it << "\n";
    }
    cout << "\n";
    
    // Loop through the set with the iterator
    for (itset = numbers.begin(); itset != numbers.end(); itset++) 
    {
        cout << *itset << "\n";
    }

    cout << "\n";

    // Point to the first element in the vector
    it = cars.begin();

    // Modify the value of the first element
    *it = "Tesla";// Volvo is now Tesla

    // Instead of this:
    // vector<string>::iterator it = cars.begin();
    // You can simply write this:
    // auto it = cars.begin();

    for (auto it = cars.begin(); it != cars.end(); ++it) {
        cout << *it << "\n";
    }
    cout << "\n";
    // Loop through vector elements
    for (auto it = cars.begin(); it != cars.end(); ) {
        if (*it == "BMW") {
        it = cars.erase(it); // Remove the BMW element
        } else {
            ++it;
        }
    }
    cout << "The vector after removing BMW: \n";
    // Print vector elements
    for (const string& car : cars) {
        cout << car << "\n";
    }
    cout << "\n";
    
    // Iterate in reverse order
    for (auto it = cars.rbegin(); it != cars.rend(); ++it) {
    cout << *it << "\n";
    }
    cout << "\n";

    // Loop through the list with an iterator
    for (auto it = LL.begin(); it != LL.end(); ++it) {
        cout << *it << "\n";
    }
    cout << "\n";

    // Loop through the deque with an iterator
    for (auto it = DD.begin(); it != DD.end(); ++it) {
        cout << *it << "\n";
    }
    cout << "\n";

    // Loop through the set with an iterator
    for (auto it = SS.begin(); it != SS.end(); ++it) {
        cout << *it << "\n";
    }   
    cout << "\n";

    // Loop through the map with an iterator
    for (auto it = people.begin(); it != people.end(); ++it) {
        cout << it->first << " is: " << it->second << "\n";
    }   
    cout << "\n";









    return 0;
}


