#include <iostream>
#include <string>
#include <sstream>
#include "schedule.h"

using namespace std;

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
Schedule::~Schedule()
{
    // memory deallocation
    for (int i = 0; i < 7; i++)
    { delete[] data[i]; }
    delete[] data;
}

// getter function for returning the number of time slots
int Schedule::getTimeSlots() const
{ return time_slots; }

// gett function for returning the specific value in the data (busy/free)
const string& Schedule::getData(int i, int j) const
{ return data[i][j]; }

// mutator function for setting the value (busy/free) for data based on index
void Schedule::setData(int day, int time_slot, const string& value)
{ this->data[day][time_slot] = value; }

// operator overload for '=' (equal)
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
        for (int j = 0; j < time_slots; j++) // copying the values
        { data[i][j] = rhs.data[i][j]; }
    }
    return *this;
}

// operator overload for '*' (asterisk)
Schedule Schedule::operator*(const Schedule& other) const
{
    // should return a bran new Schedule object
    Schedule result(this->time_slots);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < this->time_slots; j++)
        {
            if (this->data[i][j] == "busy" && other.data[i][j] == "busy")
            { result.data[i][j] = "busy"; }
            else
            { result.data[i][j] = "free"; }
        }
    }
    return result;
}

// operator overload for '[]' (square brackets)
string* Schedule::operator[](const Days day) const
{ return data[day]; }

// operator overload for '<' (lesser than)
bool Schedule::operator<(const Schedule& other) const
{
    // find the number of busy days per schedule for comparison
    int leftBusyCounter = 0;
    int rightBusyCounter = 0;

    // find the "busy" days in left handside
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < this->time_slots; j++)
        {
            if (this->data[i][j] == "busy")
            { leftBusyCounter += 1; }
        }
    }

    // find the "busy" days in the right handside
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < other.getTimeSlots(); j++)
        {
            if (other.getData(i, j) == "busy")
            { rightBusyCounter += 1; }
        }
    }
    return (leftBusyCounter < rightBusyCounter);
}

// 1st operator overload for '<<' (leftshift)
ostream& operator<<(ostream& os, const Schedule& rhs)
{
    string dayAbriviations[] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
    for (int i = 0; i < 7; i++)
    {
        os << dayAbriviations[i] << ": "; // for styling purposes
        for (int j = 0; j < rhs.getTimeSlots(); j++)
        { os << rhs.getData(i, j) << " "; }
        os << "\n";
    }
    return os;
}

// 2nd operator overload for '<<' (leftshift)
ostream& operator<<(ostream& os, const Days rhs)
{
    string dayFullNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    os << dayFullNames[rhs]; // only required to print the full name of the day
    return os;
}

// 1st operator overload for '+' (addition) with Schedule objects and Days values
Schedule operator+(const Schedule& lhs, const Days rhs)
{
    Schedule result = lhs;
    for (int i = 0; i < result.getTimeSlots(); i++) // traverse the columns instead of rows
    { // intentionally didn't use switch case because it wasn't taught in class
        if (rhs == Monday)
        { result.setData(0, i, "busy"); }
        else if (rhs == Tuesday)
        { result.setData(1, i, "busy"); }
        else if (rhs == Wednesday)
        { result.setData(2, i, "busy"); }
        else if (rhs == Thursday)
        { result.setData(3, i, "busy"); }
        else if (rhs == Friday)
        { result.setData(4, i, "busy"); }
        else if (rhs == Saturday)
        { result.setData(5, i, "busy"); }
        else if (rhs == Sunday)
        { result.setData(6, i, "busy"); }
    }
    return result;
}

// 2nd operator overload for '+' (addition) with Schedule objects and integer number
Schedule operator+(const Schedule& lhs, const int rhs)
{
    Schedule result = lhs;
    for (int i = 0; i < 7; i++)
    { result.setData(i , rhs, "busy"); }
    return result;
}

// 3rd operator overload for '+' (addition) with 2 Schedule objects 
Schedule operator+(const Schedule& lhs, const Schedule& rhs)
{
    // create a new schedule object with the same number of elements
    int size = lhs.getTimeSlots();
    Schedule result(size);

    // iterate through all the days (rows) as i and time_slots (columns) as j
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // check if the time slot is "free" in both schedules
            string leftData = lhs.getData(i, j), rightData = rhs.getData(i, j);
            if (leftData == "free" && rightData == "free")
            { result.setData(i, j, "free"); }
            else
            { result.setData(i, j, "busy"); }
        }
    }
    return result;
}
