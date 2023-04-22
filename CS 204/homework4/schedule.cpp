#include <iostream>
#include <string>
#include "schedule.h"

using namespace std;

/*
TODO:
[ ] 01. default constructor
[ ] 02. parametric constructor
[ ] 03. deep copy constructor
[ ] 04. destructor
[ ] 05. = overload
[ ] 06. + overload
    [ ] 6.1. addition of Days to Schedule
    [ ] 6.2. addition of integer to Schedule
    [ ] 6.3. addition of Schedule to Schedule
[ ] 07. * overload
[ ] 08. [] overload
[ ] 09. < overload
[ ] 10. << overload
    [ ] 10.1. for Schedule
    [ ] 10.2. for Days
*/

// default constructor
Schedule::Schedule ()  : time_slots(0), data(nullptr) 
{
    data = new string*[7]; // memory allocation
    for (int i = 0; i < 7; i++) // initialize elements to null
    { data[i] = nullptr; }
}

// parametric constructor
Schedule::Schedule(const int col) : time_slots(col), data(nullptr)
{
    data = new string*[7];
    for (int i = 0; i < 7; i++)
    {
        data[i] = new string[col];
        for (int j = 0; j < col; j++) // initialize elements to "free"
        { data[i][j] = "free"; }
    }
}

// deep copy constructor
Schedule::Schedule(const Schedule& other) : time_slots(other.time_slots), data(nullptr)
{
    data = new string*[7];
    for (int i = 0; i < 7; i++) {
        data[i] = new string[time_slots];
        for (int j = 0; j < time_slots; j++)
        { data[i][j] = other.data[i][j]; } // copy elements from "other"
    }
}

// destructor
Schedule::~Schedule() {
    // memory deallocation
    for (int i = 0; i < 7; i++)
    { delete[] data[i]; }
    delete[] data;
}

Schedule& operator=(const Schedule& rhs)
{
    // do nothing if it's assigning to itself
    if (this == &rhs)
    { return *this; }
    
    // deallocation of old memory
    for (int i = 0; i < 7; i++)
    { delete[] data[i]; }
    delete[] data;

    // allocation of new memory
    time_slots = rhs.time_slots;
    data = new string*[7];
    for (int i = 0; i < 7; i++)
    {
        data[i] = new string[time_slots];
        // copying the values
        for (int j = 0; j < time_slots; j++)
        { data[i][j] = rhs.data[i][j]; }
    }
    return *this;
}

Schedule Schedule::operator*(const Schedule& other) const
{
    // initialize bran new Schedule object with correct number of time slots
    Schedule result(this->time_slots);

    for (int i = 0; i < this->time_slots; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (this->data[j][i] == "busy" && other.data[j][i] == "busy")
            { result.data[j][i] = "busy"; }
            else
            { result.data[j][i] = "free"; }
        }
    }

    return result;
}

string* Schedule::operator[](const Days day) const
{
    return data[day];
}

bool Schedule::operator<(const Schedule& other) const
{
    int leftBusyCounter = 0;
    int rightBusyCounter = 0;

    // find the "busy" days in left and right handside
    for (int i = 0; i < this->time_slots; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (this->data[i][j] == "busy")
            { leftBusyCounter += 1; }
            if (other.data[i][j] == "busy")
            { rightBusyCounter += 1; }
        }
    }

    return (leftBusyCounter < rightBusyCounter); // is true when 'this' has less busy days than 'other'
}
