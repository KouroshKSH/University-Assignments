#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
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
    // time_slots can be shallow copied since it's just an integer
    data = new string*[7];
    for (int i = 0; i < 7; i++)
    {   // perform deep copy
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

Schedule& Schedule::operator=(const Schedule& rhs)
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

int Schedule::getTimeSlots() const
{
  return time_slots;
}

string** Schedule::getData() const
{
    return data;
}

// string* getDay(const Days day) const
// {
//     return data[day];
// }

// Overloading << for Schedule objects
ostream& operator<<(ostream& os, const Schedule& rhs)
{
    for (int i = 0; i < rhs.getTimeSlots(); i++)
    {
        for (int j = 0; j < 7; j++)
        {
            string** temp = rhs.getData();
            os << temp[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

// Overloading << for Days values
ostream& operator<<(ostream& os, const Days rhs)
{ // I intetionally didn't use switch-case since it was not taught in class
    if (rhs == Monday)
    { os << "Monday";}
    else if (rhs == Tuesday)
    { os << "Tuesday"; }
    else if (rhs == Wednesday)
    { os << "Wednesday"; }
    else if (rhs == Thursday)
    { os << "Thursday"; }
    else if (rhs == Friday)
    { os << "Friday"; }
    else if (rhs == Saturday)
    { os << "Saturday"; }
    else if (rhs == Sunday)
    { os << "Sunday"; }
    return os;
}

void Schedule::setData(const int i, const int j, const string& value)
{ // setter (mutator) function for assigning a value to the right index of the 2D matrix
  data[i][j] = value;
}

// Overloading + for Schedule objects and Days values
Schedule operator+(const Schedule& lhs, const Days rhs)
{
    // Schedule result = lhs;
    // for (int i = 0; i < result.getTimeSlots(); i++)
    // { result.data[i][rhs] = "busy"; }
    // return result;

    Schedule result = lhs;
    for (int i = 0; i < result.getTimeSlots(); i++)
    {
        if (rhs == Monday)
        {
            // result.data[i][0] = "busy";
            result.setData(i, 0, "busy");
        }
        else if (rhs == Tuesday)
        {
            // result.data[i][1] = "busy";
            result.setData(i, 1, "busy");
        }
        else if (rhs == Wednesday)
        {
            // result.data[i][2] = "busy";
            result.setData(i, 2, "busy");
        }
        else if (rhs == Thursday)
        {
            // result.data[i][3] = "busy";
            result.setData(i, 3, "busy");
        } else if (rhs == Friday)
        {
            // result.data[i][4] = "busy";
            result.setData(i, 4, "busy");
        } else if (rhs == Saturday)
        {
            // result.data[i][5] = "busy";
            result.setData(i, 5, "busy");
        } else if (rhs == Sunday)
        {
            // result.data[i][6] = "busy";
            result.setData(i, 6, "busy");
        }
    }
    return result;
}

// Overloading + for Schedule objects and integers
Schedule operator+(const Schedule& lhs, const int rhs)
{
    Schedule result = lhs;
    for (int i = 0; i < result.getTimeSlots(); i++)
    { result.setData(i, i + rhs, "busy"); } // was (i, rhs, "busy")
    return result;
}

// Overloading + for Schedule objects
Schedule operator+(const Schedule& lhs, const Schedule& rhs)
{
    //Schedule result(lhs.getTimeSlots(), lhs.getData());
    Schedule result(lhs.getTimeSlots());
    for (int i = 0; i < result.getTimeSlots(); i++)
    {
        for (int j = 0; j < 7; j++)
        {
            string** tempL = lhs.getData();
            string ** tempR = rhs.getData();
            if (tempL[i][j] == "free" && tempR[i][j] == "free")
            { result.setData(i, j, "free"); }
            else
            { result.setData(i, j, "busy"); }
        }
    }
    return result;
}
