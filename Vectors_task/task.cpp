#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "Helper_Functions.h"

using namespace std;
using namespace PST;







int main()
{
/**************************************Ex1 Start**********************VECTOR********************************* */

    // Create a vector containing specific elements: 1, 2, 3, 4
    vector<int> myNumbers = {1, 2, 3, 4};

    // Create a second vector containing 12 copies of the `long` value `61`
    //vector<long> secondvec{61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61};
    
    // get third element of first vector
    int thirdvalue = myNumbers.at(2);
    cout << "The third element in myNumbers is now: " << thirdvalue << endl;
    
    // set third element of first vector to value 4
    myNumbers.at(2) = 4;
    cout << "The third element in myNumbers is changed to: " << myNumbers.at(2) << endl;
    
    // print the number of elements in the vector
    cout << "myNumbers contains " << myNumbers.size() << " elements." << endl;

    // add element to the end
    myNumbers.push_back(5);
    cout << "myNumbers after add an element contains " << myNumbers.size() << " elements." << endl;
    
    // print return last element
    cout << "The last element in myNumbers is: " << myNumbers.back() << endl;
    int lastvalue = myNumbers.back();
    cout << "The last element in myNumbers is stored in variable lastvalue: " <<lastvalue << endl;
    
    // remove last element (but do not return it)
    myNumbers.pop_back();
    
    // print true iff `size() == 0`
    if(myNumbers.empty() == true) {
        cout << "Is empty (True)" << endl;
    } else {
        cout << "Is not empty (False)" << endl;
    }

    cout << "\n";

    // Print vector contents
    cout << "myNumbers contains: ";
    for(int i = 0; i < myNumbers.size(); i++) {
        cout << myNumbers.at(i) << " ";
    }

    // erase all elements
    myNumbers.clear(); 

    // print the size of the vector
    cout << "myNumbers contains " << myNumbers.size() << " elements." << endl;
    cout << "\n\nDone!\n";
    
/**************************************Ex1 END******************************************************* */

/**************************************Ex2 START******************ITERATOR************************************* */

/*Write a loop that prints every other member of a vector of ints named a (so indexes 0,
2, 4, and so forth), but use iterators rather than indexes. This is trickier than it might
appear!*/
    vector<int> a = {1,2,3,4,5,6,7,8,9,10};
    cout << "Every other member of vector 'a' using ITERATOR: ";
    for(auto it = a.begin(); it < a.end(); it += 2) {
        cout << *it << " ";
    }
    cout << "\n\nDONE!!\n";
    cout << "\n\n";
/**************************************Ex2 END******************************************************* */

/**************************************Ex3 START******************ITERATOR************************************* */

    /** Return an iterator that points at the first occurrence of `value` in `a`.
    * For example, if `a` holds values [1, 2, 3], then `find_first(a, 1)`
    * should return `a.begin()`. */
    // Example usage:
    // auto it = find_first(a, 1);
    // if (it != a.end()) { cout << "Found!"; }

    /*vector<int>::const_iterator find_first(const std::vector<int>& a, int value) 
    {
        for (auto it = a.begin(); it != a.end(); ++it) {
            if (*it == value) {
                return it;
            }
        }
        return a.end();
    }*/

    auto it = find_first(a, 1);
    if (it != a.end()) 
    { 
        cout << "Found!"; 
    } 
    else 
    {
        cout << "Not Found!";
    }
    cout << "\n\nDONE!!\n";
    cout << "\n\n";
/**************************************Ex3 END******************************************************* */

/**************************************Ex4 START******************Change vectors************************************* */

    vector<int> v{ 1, 2, 3, 4, 5 };
    // v.size() == 5
    // v[3] == 4    
    v.push_back(6);

    // v.size() == 6
    // v.back() == 6

    v.erase(v.begin(), v.begin() + 1);
    // v.size() == 5
    // v.back() == 6
    // v[0] == 2

    v.insert(v.begin() + 1, 10);
    // v.size() == 6
    // v[0] == 2
    // v[1] == 10


/**************************************Ex4 END******************************************************* */

/**************************************Ex5 START******************Change vectors************************************* */

    /*void remove_even_elements(std::vector<int> &v) 
    {
        for (auto it = v.begin(); it != v.end(); ) 
        {
            if (*it % 2 == 0) {
                it = v.erase(it);
            } else {
                ++it;
            }
        }
    }*/
    vector<int> v2{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    remove_even_elements(v2);
    for (int n : v2) {
        cout << n << ' '; 
    }
    cout << "\n\nDONE!!\n\n\n";
/**************************************Ex5 END******************************************************* */

/**************************************Ex6 START******************MAPS************************************* */
    // Map char to integers
    map<char, int> myMap = { {'a', 1}, {'b', 2}, {'c', 3} };

    // Insert without overwriting (leaves map unchanged if key present)
    myMap.insert({'b', 4}); 
    myMap.insert({'e', 5}); 

    // Print Number of keys in map
    myMap.size(); // 4

    // Test if key 'a' is in map
    /*
    
    for(auto it = myMap.begin(); it != myMap.end(); ++it)
    {
        cout << it->first << ": " << it->second << endl;
        if(it->first == 'a') {
            cout << "'a'Is exist: " << it->second << endl;
        }else if(it->first == 'b') {
            cout << "Found key 'b' with value: " << it->second << endl; 
        }
}*/
    int exists = myMap.find('a') != myMap.end();
    cout << "'a' Exists?? " << exists << endl;
    int Exists = myMap.find('d') != myMap.end();
    cout << "'d' Exists?? " << Exists << endl;
    cout << "\n\n";


    // Find matching element; return iterator if found, returns `m.end()` if not found
    for(auto it = myMap.begin(); it != myMap.end(); ++it)
    {        
        auto search = myMap.find(it->first);
        if (search != myMap.end()) {    
            cout << "Found " << search->first << " with value " << search->second << '\n';
        } else {
            cout << "Not found\n";
            break;
        }
    }
    cout << "\n";
    
    

    



    // Insert into map key:'a' value: 61 (with overwrite semantics)
    myMap.at('a') = 61;
    // Insert into map key:'b' value: 10 (with overwrite semantics)
    myMap.at('b') = 10;


    // Remove key 'b'
    myMap.erase('b'); 



    myMap.insert({'e', 5});
    myMap.insert({'b', 2});
    myMap.insert({'d', 4});
    myMap.insert({'c', 3});



    // Iterate in sorted order
    cout << "Print elements in sorted order:\n";
    for(auto it = myMap.begin(); it != myMap.end(); ++it)
    {
        cout << it->first << ": " << it->second << endl;
    }
    cout << "\n\nDONE!!\n";

/**************************************Ex6 END******************MAPS************************************* */

    return 0;
}












