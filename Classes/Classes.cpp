#include <iostream>
#include <string>
using namespace std;



///////////////////////////////////////////////////////////////////////////
// Create a class called Book with the following attributes:

// title (string)
// author (string)
// year (int)
// Then create two objects of the class and print their attribute values.

// Expected Output
// Matilda, Roald Dahl, 1988
// The Giving Tree, Shel Silverstein, 1964

                // class Book {
                //   public:
                //     string title;
                //     string author;
                //     int year;
                // };
///////////////////////////////////////////////////////////////////////////

/********************************************************************* */
// class MyClass {       // The class
//   public:             // Access specifier
//     int myNum;        // Attribute (int variable)
//     string myString;  // Attribute (string variable)
// };

// // Create a Car class with some attributes
// class Car {
//   public:
//     string brand;   
//     string model;
//     int year;
// };
/********************************************************************* */


/*......................................................................*/
// class MyClass {        // The class
//   public:              // Access specifier
//     int myMethod() {  // Method/function defined inside the class
//       cout << "Hello World!";
//       return 0;
//     }
// };





// class MyClass {        // The class
//   public:              // Access specifier
//     int myNum;        // Attribute (int variable)
//     string myString;  // Attribute (string variable)
//     void myMethod();  // Method/function declaration
// };



// // Method/function definition outside the class
// void MyClass::myMethod() {
//   cout << "Hello World!";
// }
/*......................................................................*/

class constructorClass {        // The class
  public:              // Access specifier
    int myNum;        // Attribute (int variable)
    string myString;  // Attribute (string variable)
    constructorClass() // Constructor declaration
    {// This is the constructor definition
        myNum = 15;        // Attribute (int variable)
        myString = "This is a string";  // Attribute (string variable)
        cout << myNum << "\n";
        cout << myString << "\n";
    }  
};


int main() 
{

    ////////////////////////////////////////////////////////////////////////////
//     Book book1;  // Create an object of Book
//     Book book2;  // Create another object of Book
// // Access attributes and set values
//     book1.title = "Matilda";
//     book1.author = "Roald Dahl";
//     book1.year = 1988;

// // Access attributes and set values
//     book2.title = "The Giving Tree";
//     book2.author = "Shel Silverstein";
//     book2.year = 1964;

// // Print attribute values
//     cout << book1.title << ", " << book1.author << ", " << book1.year << "\n";
//     cout << book2.title << ", " << book2.author << ", " << book2.year << "\n";

    ////////////////////////////////////////////////////////////////////////////


/*********************************************************************** */
//     MyClass myObj;  // Create an object of MyClass
//     Car carObj1;    // Create an object of Car
//     Car carObj2;   // Create another object of Car

// // Access attributes and set values
//     myObj.myNum = 15; 
//     myObj.myString = "This is a string";

// // Print attribute values
//     cout << myObj.myNum << "\n";
//     cout << myObj.myString << "\n";


// // Access attributes and set values
//     carObj1.brand = "BMW";
//     carObj1.model = "X5";
//     carObj1.year = 1999;

// // Access attributes and set values

//     carObj2.brand = "Ford";
//     carObj2.model = "Mustang";
//     carObj2.year = 1969;

// // Print attribute values
//     cout << carObj1.brand << " " << carObj1.model << " " << carObj1.year << "\n";
//     cout << carObj2.brand << " " << carObj2.model << " " << carObj2.year << "\n";

/*********************************************************************** */
/*......................................................................*/
    // MyClass myObj;     // Create an object of MyClass
    // int ret = myObj.myMethod(); 
    // cout << "\n"; // Call the method
    // cout << ret << "\n";  // Print the returned value
/*......................................................................*/





constructorClass obj; // Create an object of constructorClass which will invoke the constructor








    return 0;
}

















