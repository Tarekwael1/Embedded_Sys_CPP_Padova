#include <iostream>
#include <list>
#include <algorithm>
#include <string>

using namespace std;

int main() 
{
    list<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

    // Print all the elements in the list
    cout << "The list contains: \n";
    for (string car : cars) 
    {
        cout << car << endl;
    }
    cout << "\n";

    // Get the first element
    cout << "This is the first element: " << cars.front();  // Outputs Volvo

    cout << "\n";

    // Get the last element
    cout << "This is the last element: " << cars.back();  // Outputs Mazda
    
    cout << "\n";

    // Change the value of the first element
    cars.front() = "Opel";

    // Change the value of the last element
    cars.back() = "Toyota";

    cout << "The first element after modification: " << cars.front(); // Now outputs Opel instead of Volvo
    cout << "\n";
    cout << "The last element after modification: " << cars.back();  // Now outputs Toyota instead of Mazda

    cout << "\n";

    // Add an element at the beginning
    cars.push_front("Tesla");

    cout << "\n";

    // Add an element at the end
    cars.push_back("VW");
    
    cout << "\n";

    cout << "The list after adding two elements: \n";
    for (string car : cars) 
    {
        cout << car << endl;
    }
    
    cout << "\n";


    // Remove the first element
    cars.pop_front();

    // Remove the last element
    cars.pop_back();

    cout << "The list after removing first andlast elements: \n";
    for (const string& car : cars) 
    {
        cout << car << "\n";
    }

    cout << "\n";

    cout << "The size of the list is: " << cars.size();  // Outputs 4

    cout <<"Is the list empty? "<< cars.empty();  // Outputs 0 (The list is not empty)
    return 0;
}