#include <iostream>
#include <stack>
#include <string>
#include <algorithm>

using namespace std;


//In the stack we cannot access the elements like in a vector using an index.
//We can only access the top element of the stack.

int main()
{
    stack<string> cars;

    cars.push("Volvo");
    cars.push("BMW");
    cars.push("Ford");
    cars.push("Mazda");
    
    // Access the top element
    cout << cars.top();  // Outputs "Mazda"

    // Change the value of the top element
    cars.top() = "Tesla";

    // Access the top element
    cout << cars.top();  // Now outputs "Tesla" instead of "Mazda"   
    cout << "\n";

    // Remove the last added element (Mazda)
    cars.pop();

    // Access the top element (Now Ford)
    cout << cars.top();  // Outputs "Ford" 
    cout << "\n";

    cout << "Size of the stack: " << cars.size();  // Outputs 3
    cout << "\n";

    // Check if the stack is empty
    if(cars.empty() == false) {
        cout << "Is not empty (False)" << endl;
    } else {
        cout << "Is empty (True)" << endl;
    }   
    cout << "\n";

    


    return 0;
}