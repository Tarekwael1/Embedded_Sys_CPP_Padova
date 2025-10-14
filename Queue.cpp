//Unlike vectors, elements in the queue are not accessed by index numbers
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    //You cannot add elements to the queue at the time of declaration, like you can with vectors:
    queue<string> cars; //cannot be changed after its been declared.

    cars.push("Volvo");
    cars.push("BMW");
    cars.push("Ford");
    cars.push("Mazda");

    // Access the first element
    cout << cars.front();  // Outputs "Volvo"
    cout << "\n";

    // Change the value of the first element
    cars.front() = "Tesla";

    // Access the first element
    cout << cars.front();  // Now outputs "Tesla" instead of "Volvo"   
    cout << "\n";

    // Remove the first added element (Tesla)
    cars.pop();

    // Access the first element (Now BMW)
    cout << cars.front();  // Outputs "BMW" 
    cout << "\n";

    cout << "Size of the queue: " << cars.size();  // Outputs 3
    cout << "\n";

    // Check if the queue is empty
    if(cars.empty() == false) {
        cout << "Is not empty (False)" << endl;
    } else {
        cout << "Is empty (True)" << endl;
    }   
    cout << "\n";

    // Change the value of the front element
    cars.front() = "Tesla";

    // Change the value of the back element
    cars.back() = "VW";

    // Access the front element
    cout << cars.front();  // Now outputs "Tesla" instead of "Volvo"
    cout << "\n";
    // Access the back element
    cout << cars.back();  // Now outputs "VW" instead of "Mazda"
    cout << "\n";

    
    return 0;
}