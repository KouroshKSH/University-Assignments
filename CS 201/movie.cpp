#include <iostream>
#include <string>
#include "movie.h"

using namespace std;

MyClassMovie::MyClassMovie()
{
    name = "";
    year = 0;
    genre = "";
}

MyClassMovie::MyClassMovie(string n, int y, string g)
{
    name = n;
    year = y;
    genre = g;
}

void MyClassMovie::setName(string n)
{
    name = n;
}

string MyClassMovie::getName() const
{
    return name;
}

void MyClassMovie::setGenre(string g)
{
    genre = g;
}

string MyClassMovie::getGenre() const
{
    return genre;
}

void MyClassMovie::setYear(int y)
{
    year = y;
}

int MyClassMovie::getYear() const
{
    return year;
}

void MyClassMovie::print()
{
    // Can only be used for option 2
    // ... since option 1 does not print genre
    cout << "Movie name: " << name << " Release Year: " << year << " Genre: " << genre << endl;
}