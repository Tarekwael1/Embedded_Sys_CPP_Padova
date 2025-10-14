//A deque (stands for double-ended queue) however, is more flexible, as elements can be added and removed from both ends (at the front and the back). You can also access elements by index numbers.
#include <iostream>
#include <deque>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    //The type of the deque (string in our example) cannot be changed after its been declared.
    //You can access a deque element by referring to the index number inside square brackets [].
    deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

    // Print all the elements in the deque
    cout << "The deque contains: \n";
    for (string car : cars) 
    {
        cout << car << endl;
    }
    cout << "\n";

    // Get the first element
    cout << "This is the first element: " << cars.front();  // Outputs Volvo
    cout << "\n";

    // Get the first element
    cout << "Or cars[0] = "<< cars[0];  // Outputs Volvo
    cout << "\n";
    
    cout << "Or cars.at(0) = "<< cars.at(0);  // Outputs Volvo
    cout << "\n";

    // Get the last element
    cout << "This is the last element: " << cars.back();  // Outputs Mazda
    cout << "\n";
    
    cout << "Or cars[3] = "<< cars[3];  // Outputs Mazda
    cout << "\n";
    
    cout << "Or cars[cars.size()-1] = "<< cars[cars.size()-1];  // Outputs Mazda
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

    cout << "The deque after adding two elements: \n";
    for (string car : cars) 
    {
        cout << car << endl;
    }
    
    cout << "\n";

    // Remove the first element
    cars.pop_front();

    // Remove the last element
    cars.pop_back();

    cout << "The deque after removing two elements: \n";
    for (int i = 0; i < cars.size(); i++) {
        cout << cars[i] << "\n";
    }
    
    cout << "\n";
    return 0;
}