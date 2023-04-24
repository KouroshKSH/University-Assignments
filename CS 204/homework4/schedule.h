#ifndef _CLOCKTIME_H
#define _CLOCKTIME_H

#include <iostream>
#include <string>
using namespace std;

enum Days {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};

class Schedule
{
    private:
        int time_slots;
        string** data;

    public:
        Schedule(); // default constructor
        Schedule(const int col); // parametric constructor
        Schedule(const Schedule& other); // deep copy constructor
        ~Schedule(); // destructor

        Schedule& operator=(const Schedule& other); // assignment operator
        Schedule operator*(const Schedule& other) const; // asterisk operator
        string* operator[](const Days day) const; // square bracket operator using enum
        bool operator<(const Schedule& other) const; // less than operator

        int getTimeSlots() const; // getter function for time_slots
        //string** getData() const; // getter function for double pointer of data
        const string& getData(int i, int j) const;
        //string* getDay(const Days day) const; // getter function for day

        //void setData(const int i, const int j, const string& value); // setter function for data
        // Mutator function to set the data value at a specific index
        void setData(int row, int col, const string& value);
};


ostream& operator<<(ostream& os, const Schedule& rhs);
ostream& operator<<(ostream& os, const Days rhs);

// Overloading + for Schedule objects and Days values
Schedule operator+(const Schedule& lhs, const Days rhs); 

// Overloading + for Schedule objects and integers
Schedule operator+(const Schedule& lhs, const int rhs);

// Overloading + for Schedule objects
Schedule operator+(const Schedule& lhs, const Schedule& rhs);

#endif
