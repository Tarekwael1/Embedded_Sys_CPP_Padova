#include <iostream>
#include <cstdio>
#include <string>
#include <math.h>
#include <time.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <windows.h>
#include <map>
#include <set>
#include <vector>


using namespace std;

struct MyDate {
    int day;
    int month;
    int year;
};



void Welcome_Message();
bool Input_Choices();
bool Cutomer_Service();
bool Safe_Service();
bool Get_an_appointment(int max_per_slot);



const char customers_number = 'B'; //B
const char safes_number = 'S';  //S
unsigned int max_customers_CS = 80;
unsigned int max_customers_SS = 500;

unsigned int* customers_CS_ptr;
unsigned int* customers_SS_ptr;

unsigned int customer_id{0};
unsigned int safe_id{0};

map<int, vector<int>> booked_appointments;

int main()
{
    bool check = true;

    customers_CS_ptr = &max_customers_CS;
    customers_SS_ptr = &max_customers_SS;

    Welcome_Message();
    system("cls");
    check =Input_Choices();



    return 0;
}


void Welcome_Message()
{
    string Welcome{"Welcome to our Bank system...."};
    for(char out : Welcome)
    {
        cout << out;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "\n";
}

bool Input_Choices()
{
    bool check{true};
    auto choice{0};
    
    
    while(choice != 3)
        {
            cout << "Please choose from the following services : \n";
            cout << "1- Customer Service\n";
            cout << "2- Safe Service\n";
            cout << "3- Exit\n";
            cout << "Your choice : ";
            cin >> choice;
            if(choice == 1)
            {
                system("cls");
                check = Cutomer_Service();
            }
            else if(choice == 2)
            {
                system("cls");
                check = Safe_Service();  
            }
            else if(choice == 3)
            {
                system("cls");
                cout << "Thank you for using our bank system\n";
                this_thread::sleep_for(chrono::milliseconds(1000));
            break;
            }
            else 
            {
                cout << "Invalid choice, please try again\n";
                this_thread::sleep_for(chrono::milliseconds(1000));
                system("cls");
            }
    }
    return check;
}

bool Cutomer_Service()
{
    auto issue{0};

    cout << "Welcome to Customer Service\n";
    cout << "What is your issue ?\n";
    cout << "1- I have a problem with my account\n";
    cout << "2- I want to open a new account\n";
    cout << "3- I want to close my account\n";
    cout << "4- Other issues\n";
    cin >> issue;

    if(issue >= 1 && issue <= 4)
    {
        customer_id += 1;
        if(issue == 1)
        {
            cout << "Your issue is: 'Problem with account'\n";
        }
        else if(issue == 2)
        {
            cout << "Your issue is 'Open a new account'\n";
        }
        else if(issue == 3)
        {
            cout << "Your issue is 'Close the account'\n";
        }
        else if(issue == 4)
        {
            cout << "Your issue is 'Other issues'\n";
        }
        cout << "your ID is : " << customers_number << customer_id << "\n";
        if(!Get_an_appointment(*customers_CS_ptr / 20)){
            customer_id--;
            cout << "Appointment booking failed. ID revoked.\n";
        }else{
            cout << "please do not be late for your appointment\n";
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        cout << "Note: If you was not there when your number is called, you will lose your turn\n";
        SetConsoleTextAttribute(hConsole, 7);
    }else
    {
        cout << "Invalid issue, please try again\n";
    }
    this_thread::sleep_for(chrono::milliseconds(5000));
    system("cls");
    return true;
}


bool Safe_Service()
{
    unsigned int Order{0};



    cout << "Welcome to Safe Service\n";
    cout << "What do you want to do ?\n";
    cout << "1- Rent a safe\n";
    cout << "2- Return a safe\n";
    cout << "3- Insert money in the safe\n";
    cout << "4- Withdraw money from the safe\n";
    cout << "5- Other issues\n";
    cin >> Order;
    if(Order >= 1 && Order <= 5)
    {
        safe_id += 1;
        if(Order == 1)
        {
            cout << "Your issue is: 'Rent a safe'\n";
        }
        else if(Order == 2)
        {
            cout << "Your issue is 'Return a safe'\n";
        }
        else if(Order == 3)
        {
            cout << "Your issue is 'Insert money in the safe'\n";
        }
        else if(Order == 4)
        {
            cout << "Your issue is 'Withdraw money from the safe'\n";
        }
        else if(Order == 5)
        {
            cout << "Your issue is 'Other issues'\n";
        }
        cout << "your ID is : " << safes_number << safe_id << "\n";
        if(!Get_an_appointment(*customers_SS_ptr / 20)){
            safe_id--;
            cout << "Appointment booking failed. ID revoked.\n";
        }else{
            cout << "please do not be late for your appointment\n";
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        cout << "Note: If you was not there when your number is called, you will lose your turn\n";
        SetConsoleTextAttribute(hConsole, 7);
    this_thread::sleep_for(chrono::milliseconds(1000));
    system("cls");
    return true;
    }else
    {
        cout << "Invalid issue, please try again\n";
    }
}

bool is_weekday(MyDate d) 
{
    struct tm t = {0};
    t.tm_year = d.year - 1900;
    t.tm_mon = d.month - 1;
    t.tm_mday = d.day;
    t.tm_hour = 12;
    t.tm_isdst = -1;
    mktime(&t);
    return t.tm_wday <= 4;
}

string get_slot_time(int slot) {
    int hour = 9 + slot / 4;
    int min = (slot % 4) * 15;
    char buf[6];
    sprintf(buf, "%02d:%02d", hour, min);
    return string(buf);
}

bool Get_an_appointment(int max_per_slot)
{
    bool check = true;
    MyDate date;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    cout << "Available dates for appointment:\n";
    vector<MyDate> available_dates;
    for(int i=1; i<=7; i++){
        tm t = *ltm;
        t.tm_mday += i;
        mktime(&t);
        if(t.tm_wday <=4){
            MyDate d = {t.tm_mday, t.tm_mon+1, t.tm_year+1900};
            available_dates.push_back(d);
            cout << available_dates.size() << ". " << d.day << "/" << d.month << "/" << d.year << "\n";
        }
    }
    cout << "Choose a date number: ";
    int choice;
    cin >> choice;
    if(choice <1 || choice > available_dates.size()){
        cout << "Invalid choice\n";
        check = false;
    }else{
        date = available_dates[choice-1];
        int key = date.month * 100 + date.day;
        if(booked_appointments.find(key) == booked_appointments.end()){
            booked_appointments[key] = vector<int>(20, 0);
        }
        vector<int> counts = booked_appointments[key];
        cout << "Available time slots for " << date.day << "/" << date.month << "/" << date.year << ":\n";
        for(int i = 0; i < 20; i++){
            if(counts[i] < max_per_slot){
                cout << (i+1) << ". " << get_slot_time(i) << "\n";
            }
        }
        cout << "Choose a slot number (1-20): ";
        int slot_choice;
        cin >> slot_choice;
        if(slot_choice < 1 || slot_choice > 20 || counts[slot_choice-1] >= max_per_slot){
            cout << "Invalid choice or slot fully booked.\n";
            check = false;
        }else{
            counts[slot_choice-1]++;
            booked_appointments[key] = counts;
            cout << "Appointment booked for " << date.day << "/" << date.month << "/" << date.year << " at " << get_slot_time(slot_choice-1) << "\n";
            check = true;
        }
    }
    return check;
}
