#ifndef _CLOCKTIME_H
#define _CLOCKTIME_H

#include <iostream>
#include <string>
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
        string* operator[](const Days day) const; // square bracket operator
        bool operator<(const Schedule& other) const; // less than operator
};

#endif
