#include <stdio.h> 

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {JAN = 1, FEB, MAR, APR, MAY, JUN,
                        JUL, AUG, SEP, OCT, NOV, DEC } Months;

//void generateDate( int&, int&, int);
void printDate( Months, int, int );
bool validDate( Months, int, int );
int getYear();
Months getMonth();  
int getDay();


int main()
{
  Months month = JAN;
  int day = 1;
  int year = 1900;
  
  srand( time( 0 ) );
  
  printDate( month, day, year );
  year = getYear();
  month = getMonth();
  day = getDay();
  
  if ( validDate( month, day, year )  == true )
    printDate( month, day, year );
    
  return 0;
  
}

// return month
Months getMonth()
{

  int myMonth = rand() % 12 + 1;

  return Months(myMonth);
}

// return year
int getYear()
{
  return rand() % 105 + 1900;
}

// return day
int getDay()  
{
  return rand() % 31 + 1;
}

// output date
void printDate( Months month, int day, int year )
{
  switch ( month ) {
  
    case JAN:
      printf("January %d, %d \n", day,year);
      break;

    case FEB:
      printf("February %d, %d \n", day,year);
      break;
            
    case MAR:
      printf("March %d, %d \n", day,year);
      break;
            
    case APR:
      printf("April %d, %d \n", day,year);
      break;      
    case MAY:
      printf("May %d, %d \n", day,year);
      break;

    case JUN:
      printf("June %d, %d \n", day,year);
      break;
      
    case JUL:
      printf("July %d, %d \n", day,year);
      break;      
      
    case AUG:
      printf("August %d, %d \n", day,year);
      break;
      
    case SEP:
      printf("September %d, %d \n", day,year);    
      break;
    case OCT:
      printf("October %d, %d \n", day,year);
      break;
      
    case NOV:
      printf("November %d, %d \n", day,year);
      break;
      
    case DEC:
      printf("December %d, %d \n", day,year);
      break;
            
    default:
      printf("invalid month\n");
      
  }
  
}

bool validDate( Months month, int day, int year )
{
  if ( year < 1900 || year > 2001)
    return false;

  else if ( month < 1 || month > 12)
    return false;

  else if (day < 1 || day > 31 )
    return false;

  else if ( day == 31 && (month == APR || month == JUN || month == SEP || month == NOV))
    return false;

  else if (month == 2 && day > 28 )
    return false;

  return true; 
}
