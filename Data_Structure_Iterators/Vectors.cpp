//This file is spefied to contain functions that use vectors and define what vectors are.
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;




int main()
{
    vector<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};
//This is the first way to print out the elements of a vector.
    cout << "1- I like " << cars[0] << ", " << cars[1] << ", " << cars[2] << ", and " << cars[3] << ".";

//This is the second way to print out the elements of a vector.
    cout << "\n\n2- ";
    for(int i = 0; i < cars.size(); i++) {
        cout << "\nI like " << cars[i] << ".";
    }
//This is the third way to print out the elements of a vector.
    cout << "\n\n3- ";
    for(string i : cars) {
        cout << "\nI like " << i << ".";
    }
    cout << "\n\n";


    cout << cars[0];
    cars[0] = "Opel";   
    cout << "\n" << cars[0] << endl;



    cout << cars.front() << endl; //Prints the first element of the vector.
    cout << cars.back() << endl; //Prints the last element of the vector.


    cout << cars.at(2) << endl; //"This is equal to cout << cars[2] "//Prints the element at index 2 of the vector.
    
    
    /*The error of this is like this 
    D:/MYSS2/ucrt64/include/c++/15.2.0/bits/stl_vector.h:1263: std::vector<_Tp, _Alloc>::reference std::vector<_Tp, _Alloc>::operator[](size_type) [with _Tp = std::__cxx11::basic_string<char>; _Alloc = std::allocator<std::__cxx11::basic_string<char> >; reference = std::__cxx11::basic_string<char>&; size_type = long long unsigned int]: Assertion '__n < this->size()' failed.*/
// cout << cars[7] << endl; //This will cause an error because there is no index 7 in the vector.
    

    /*But the error of this will be more descriptive
    terminate called after throwing an instance of 'std::out_of_range'
    what():  vector::_M_range_check: __n (which is 7) >= this->size() (which is 4)*/
//cout << cars.at(7) << endl; //This will cause an error because there is no index 7 in the vector, but it will be a more descriptive error message.
    
    //also when I try to change any element, it is brefer to use at() function.
    cars.at(3) = "Toyota"; //This will change the element at index 3 to "Toyota".
    cout << cars[3] << endl; //This will print "Toyota".

    cars.push_back("Audi"); //This will add "Audi" to the end of the vector.
    cout << cars.back() << endl; //This will print "Audi".

    cars.pop_back(); //This will remove the last element of the vector.
    cout << cars.back() << endl; //This will print "Toyota".



    for(string i : cars) {
        cout << "\nI like " << i << ".";
    }
    cout << "\n\n";
    
    
    
    cout << "\nVector size: " << cars.size() << endl; //This will print the size of the vector.
    cout << "Vector capacity: " << cars.capacity() << endl; //This will print
    
    
    // cout << cars.empty() << endl; //This will print 0 because the vector is not empty.
    // cars.clear(); //This will remove all elements from the vector.
    // cout << cars.empty() << endl; //This will print 1 because the vector is empty now.
    // cout << "Vector size: " << cars.size() << endl; //This will print
    // cout << "Vector capacity: " << cars.capacity() << endl; //This will print the capacity of the vector, which is not necessarily 0.
    
    
    cout << "\nUsing iterators to loop through the vector:\n";
    // Create a vector iterator called it
    vector<string>::iterator it;
    auto it2 = cars.begin(); //This is another way to declare an iterator using auto keyword.
    cout << "\n\n" << *it2 << "\n\n\n"; //This will print the first element of the vector.

    cout << "Using iterator it:\n";
    // Loop through the vector with the iterator
    for (it = cars.begin(); it != cars.end(); ++it) {
    cout << *it << "\n";
    }
    

    cout << "\nRemoving elements from the vector while iterating:\n";
    // Loop through vector elements
    for (auto it = cars.begin(); it != cars.end(); ) {
    if (*it == "BMW") {
        it = cars.erase(it); // Remove the BMW element
    } else {
        ++it;
    }
    }

    cout << "\nVector after removing BMW:\n";
    // Iterate in reverse order
    for (auto it = cars.rbegin(); it != cars.rend(); ++it) {
    cout << *it << "\n";
    }


    cout << "\nSorting the vector:\n";
    // Sort cars in alphabetical order
    sort(cars.begin(), cars.end());

    // Print cars in alphabetical order
    for (string car : cars) {
        cout << car << "\n";
    }

    cout << "\nSorting the vector in reverse order:\n";
    // Create a vector called numbers that will store integers
    vector<int> numbers = {1, 7, 3, 5, 9, 2};

    // Sort numbers numerically
    sort(numbers.begin(), numbers.end());

    for (int num : numbers) {
        cout << num << "\n";
    }
    cout << "\n";
    cout << "Sorting the vector in reverse order:\n";
// Sort numbers in reverse order
    // Sort numbers numerically in reverse order
    sort(numbers.rbegin(), numbers.rend());

    for (int num : numbers) {
        cout << num << "\n";
    }



    cars.erase(cars.begin() +1, cars.begin() +2); //This will remove the elements at index 2 and 3 (4 is not included).
    cout << "\n\nAfter erasing elements at index 2 and 3:\n";
    for (string car : cars) {
        cout << car << "\n";
    }

    return 0;
}