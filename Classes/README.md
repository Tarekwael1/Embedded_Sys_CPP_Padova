## Classes ##

In this directory you will find the sequence of versions of `Date` introduced in lectures about Classes.

For each version the `main()` function:
1. defines a `Date` called `today` initialized to June 25, 1978;
2. defines a `Date` called `tomorrow` and gives it a value by copying `today` into it and incresing its day by one using `add_day()`;
3. outputs `today` and `tomorrow` using a `<<` operator;
4. finally, test `Date` with one invalid date (e.g. 2004, 13, -5). 
 
The check for a valid date is very simple. It ignores leap years. However, it doesn't accept a month that is not in the [1,12] range or day of the month that is not in the [1,31] range.

### `Date` versions ###

1. `Date01` struct and functions
2. `Date02` member functions and constructors
3. `Date03` keep details private, defining member functions
4. `Date04` enumerations, class interface, argument types
5. `Date05` default constructor, const member functions