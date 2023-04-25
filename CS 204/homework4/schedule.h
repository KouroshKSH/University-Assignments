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

        // getter functions for time_slots and data
        int getTimeSlots() const;  
        const string& getData(int i, int j) const;

        // mutator function to set the data value at a specific index
        void setData(int row, int col, const string& value);

        // member functions for operator overloading (=, *, [], <)
        Schedule& operator=(const Schedule& other); // assignment operator
        Schedule operator*(const Schedule& other) const; // asterisk operator
        string* operator[](const Days day) const; // square bracket operator using enum
        bool operator<(const Schedule& other) const; // less than operator
};

// free functions for operator overloading (<<, +)
ostream& operator<<(ostream& os, const Schedule& rhs); // 1st leftshift operator
ostream& operator<<(ostream& os, const Days rhs); // 2nd leftshift operator

Schedule operator+(const Schedule& lhs, const Days rhs); // 1st addition operator
Schedule operator+(const Schedule& lhs, const int rhs); // 2nd addition operator
Schedule operator+(const Schedule& lhs, const Schedule& rhs); // 3rd addition operator

#endif
