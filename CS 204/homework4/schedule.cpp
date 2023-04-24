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
    //cout << "\n\n\tdefault constructor has been called.\n\n";
    data = new string*[7]; // memory allocation
    for (int i = 0; i < 7; i++) // initialize elements to null
    { data[i] = nullptr; }
}

// parametric constructor
Schedule::Schedule(const int col) : time_slots(col), data(nullptr)
{
    //cout << "\n\n\tparametric constructor has been called.\n";
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
    //cout << "\n\n\tdeep copy has been called.\n\n";
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
    //cout << "\n\n\tdestructor ~ has been called.\n\n";
    // memory deallocation
    for (int i = 0; i < 7; i++)
    { delete[] data[i]; }
    delete[] data;
}

Schedule& Schedule::operator=(const Schedule& rhs)
{
    //cout << "\n\n\toperator = has been called.\n\n";
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
    //cout << "\n\n\toperator * has been called.\n\n";
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
    //cout << "\n\n\toprator [] has been called.\n\n";
    return data[day];
}

bool Schedule::operator<(const Schedule& other) const
{
    //cout << "\n\n\toperator < has been called.\n\n";
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
{ return time_slots; }


const string& Schedule::getData(int i, int j) const
{ return data[i][j]; }

// 1st << overload for Schedule objects
ostream& operator<<(ostream& os, const Schedule& rhs)
{
    //cout << "\n\n\t1st << operator has been called.\n\n";
    string dayAbriviations[] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
    for (int i = 0; i < 7; i++)
    {
        os << dayAbriviations[i] << ": ";
        for (int j = 0; j < rhs.getTimeSlots(); j++)
        { os << rhs.getData(i, j) << " "; }
        os << "\n";
    }
    return os;
}

// 2nd << overload for Days values
ostream& operator<<(ostream& os, const Days rhs)
{
    //cout << "\n\n\t2nd << operator has been called.\n\n";
    string dayFullNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    os << dayFullNames[rhs];
    return os;
}

void Schedule::setData(int day, int time_slot, const string& value)
{
    // check if the day and time slot are within range
    if (day >= 0 && day < 7 && time_slot >= 0 && time_slot < this->getTimeSlots())
    {
        // set the data for the specified day and time slot
        this->data[day][time_slot] = value;
    }
}


// Overloading + for Schedule objects and Days values
Schedule operator+(const Schedule& lhs, const Days rhs)
{
    cout << "\n\n\t1st + operator has been called.\n\n";
    Schedule result = lhs;
    cout << "\nrhs is: " << rhs << endl;
    for (int i = 0; i < 7; i++) // changed form result.getTimeSlots()
    {
        if (rhs == Monday)
        {
            // result.data[i][0] = "busy";
            //cout << "\nMonday has been set to busy\n";
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

// // Overloading + for Schedule objects and integers
// Schedule operator+(const Schedule& lhs, const int rhs)
// {
//     cout << "\n\n\t2nd + operator has been called.\n\n";
//     Schedule result = lhs;
//     //cout << "\n\tget time slots returns the number: " << result.getTimeSlots() << "\n";
//     // gotta fix this, shouldn't make everything busy 
//     for (int i = 0; i < 7; i++) // was getTimeSlots() instead of 7
//     {
//         result.setData(i, rhs, "busy");
//     } // was (i, i + rhs, "busy")
//     cout << "\nassigned busy to the " << rhs+1 << "th place successfully.\n";
//     return result;
// }

// Overloading + for Schedule objects and integers
Schedule operator+(const Schedule& lhs, const int rhs)
{
    // cout << ""
    Schedule result = lhs;
    for (int j = 0; j < result.getTimeSlots(); j++) // iterate through all time slots
    {
        //result.setData(rhs - 1, j, "busy"); // set the corresponding data to "busy"
        //result.setData(j , rhs - 1, "busy");
        result.setData(rhs-4, j, "busy");
        //result.setData(j, rhs - 4, "busy");
    }
    cout << "\nassigned busy to the " << rhs << "th place successfully.\n";
    return result;
}


// Overloading + for Schedule objects
Schedule operator+(const Schedule& lhs, const Schedule& rhs)
{
    cout << "\n\n3rd + overload and let's see if it works\n\n";
    // create a new schedule object with the same number of elements
    int size = lhs.getTimeSlots();
    Schedule result(size);

    // iterate through all the days (rows) as i and time_slots (columns) as j
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // check if the time slot is "free" in both schedules
            string leftData = lhs.getData(i, j);
            string rightData = rhs.getData(i, j);
            if (leftData == "free" && rightData == "free")
            {
                // set the corresponding day to free in the result schedule
                result.setData(i, j, "free");
            } else {
                result.setData(i, j, "busy");
            }
        }
    }

    // return the final schedule
    return result;
}
