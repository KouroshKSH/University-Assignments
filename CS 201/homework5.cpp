#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "strutils.h"
#include "movie.h"

using namespace std;

void funcPrintVector(vector < MyClassMovie > givenVec);
void funcInsertionSort(vector < MyClassMovie > &givenVec);
void funcRandomMovie(vector < MyClassMovie > givenVec);
int funcChoiceValidity(string inputString);
bool funcCompareMovies(const MyClassMovie& movie1, const MyClassMovie& movie2);
void funcSelectedGenre(const vector < MyClassMovie > givenVec, string selectedGenre);
void funcSelectedYear(const vector < MyClassMovie > givenVec, string selectedYear);
string funcMyLowerString(const string s);

int main()
{
    cout << "Welcome to the movie recommender program!\n\n";
    cout << "Please enter the movie list filename: ";
    string nameOfFile;
    ifstream input;
    input.open(nameOfFile.c_str());
    int count = 0;
    vector <MyClassMovie> vecMovie;

    while (true)
    {
        cin >> nameOfFile;
        input.open(nameOfFile.c_str());

        if (!input.fail()) {
            break;
        }
        cout << "Please check filename! Enter a correct movie list filename: ";
        cin.clear();
    }
    string currentLine, currentName, currentGenre;
    int currentYear;
    while (getline(input, currentLine))
    {
        int positionFirstDigit = 0, positionLastDigit = 0;

        for (int i = 0; i < currentLine.length(); i++)
        {
            if (isdigit(currentLine[i]))
            {
                positionFirstDigit = i;
                positionLastDigit = i + 4;
                break;
            }
        }
        currentName = currentLine.substr(0, positionFirstDigit - 1);
        currentYear = atoi(currentLine.substr(positionFirstDigit, 4));
        currentGenre = currentLine.substr(positionLastDigit + 1);
        
        // Handle carriage
        if (currentGenre.find("\r") != string::npos)
        {
            currentGenre = currentGenre.substr(0, currentGenre.length()-1);
        }

        MyClassMovie currentMovie;
        currentMovie.setName(currentName);
        currentMovie.setGenre(currentGenre);
        currentMovie.setYear(currentYear);
        vecMovie.push_back(currentMovie);

    }
    input.close();

    funcInsertionSort(vecMovie);

    cout << "Please select your action:\n";
    cout << "1. Select the genre of the movie that you want to watch\n";
    cout << "2. Select the year of the movie that you want to watch\n";
    cout << "3. Exit program\n";

    bool validChoiceBool = true;
    while (validChoiceBool)
    {
        cout << "\nEnter your choice: ";
        string actionInputString;
        cin >> actionInputString;
        int actionInputNumber = funcChoiceValidity(actionInputString)
        if (actionInputNumber == 1)
        {
            cout << "Please enter the genre of the movie you want to watch: ";
            string desiredGenre;
            cin >> desiredGenre;
            funcSelectedGenre(vecMovie, desiredGenre);
        } else if (actionInputNumber == 2)
        {
            cout << "Please enter the year of the movie you want to watch: ";
            string desiredYear;
            cin >> desiredYear;
            funcSelectedYear(vecMovie, desiredYear);
        } else if (actionInputNumber == 3)
        {
            cout << "Thank you...\n";
            validChoiceBool = false;
        } else if (actionInputNumber <= 0 || actionInputNumber >= 4){
            cout << "Invalid action!\n";
        }
    }

    return 0;
}


void funcInsertionSort(vector <MyClassMovie> &givenVec)
{
    for (int i = 1; i < givenVec.size(); i++)
    {
        MyClassMovie current = givenVec[i];
        int j = i - 1;
        while (j >= 0 && funcCompareMovies(givenVec[j], current))
        {
            givenVec[j + 1] = givenVec[j];
            j--;
        }
        givenVec[j + 1] = current;
    }
}

int funcChoiceValidity(string actionInputString)
{
    // Return the integer equivalent of the choice of the user (if valid)
    int actionInputNumber = 0;
    if (actionInputString[0] == '-')
    {
        // Return zero for negative numbers
        return actionInputNumber;
    } else {
        for (char ch : actionInputString)
        {
            if (ch >= '0' && ch <= '9')
            {
                actionInputNumber = (actionInputNumber * 10) + (ch - '0');
            } else {
                // If there is anything other than digits
                actionInputNumber = 0;
                break;
            } 
        }
        return actionInputNumber;
    }
}

bool funcCompareMovies(const MyClassMovie &movie1, const MyClassMovie &movie2)
{
    // Check years first, then alphabetical order
    int movie1year = movie1.getYear();
    int movie2year = movie2.getYear();
    if (movie1year != movie2year)
    {
        return movie1year < movie2year;
    } else {
        return movie1.getName() > movie2.getName();
    }
}

void funcSelectedGenre(const vector < MyClassMovie > givenVec, string selectedGenre)
{
    int vecSize = givenVec.size();
    string copySelectedGenre = selectedGenre;
    copySelectedGenre = funcMyLowerString(copySelectedGenre);
    // Check for the presence of the desired genre
    int flag = 0;
    for (int i = 0; i < vecSize; i++)
    {
        // Create a copy of each movie struct to avoid modification
        MyClassMovie currentMovie = givenVec[i];

        // Lowercase for absolute comparison
        string tempGenre = currentMovie.getGenre();
        tempGenre = funcMyLowerString(tempGenre);
        if (tempGenre == copySelectedGenre)
        {
            flag += 1;
            break;
        }
    }

    // When there are no movies with the desired genre
    if (flag == 0)
    {
        cout << "\nThere are no " << selectedGenre << " movies!\n";
    } else {
        cout << "\n" << selectedGenre << " movies from newest to oldest:\n";
        for (int i = 0; i < vecSize; i++)
        {
            MyClassMovie currentMovie = givenVec[i];
            string tempGenre = currentMovie.getGenre();
            tempGenre = funcMyLowerString(tempGenre);
            if (tempGenre == copySelectedGenre)
            {
                cout << "Movie name: " << currentMovie.getName() << " Release Year: " << currentMovie.getYear() << endl;
            }
        }
    }
}

void funcSelectedYear(const vector < MyClassMovie > givenVec, string selectedYear)
{
    if (selectedYear.find("-") != string::npos)
    {
        // Input contains a range of years to check
        int dashLocation = selectedYear.find("-");
        int startingYear = atoi(selectedYear.substr(0, dashLocation));
        int endingYear = atoi(selectedYear.substr(dashLocation + 1));
        if (startingYear > endingYear)
        {
            startingYear = startingYear + endingYear;
            endingYear = startingYear - endingYear;
            startingYear = startingYear - endingYear;
        }
        int flag = 0;
        for (int i = 0; i < givenVec.size(); i++)
        {
            MyClassMovie currentMovie = givenVec[i];
            if (currentMovie.getYear() >= startingYear && currentMovie.getYear() <= endingYear)
            {
                flag += 1;
                break;
            }
        }

        if (flag == 0)
        {
            cout << "There are no movies released between " << selectedYear << "!\n";
        } else {
            cout << "Movies released between the years " << selectedYear << " from A to Z with decreasing year ordering:\n";
            for (int i = 0; i < givenVec.size(); i++)
            {
                MyClassMovie currentMovie = givenVec[i];
                if (currentMovie.getYear() >= startingYear && currentMovie.getYear() <= endingYear)
                {
                    cout << "Movie name: " << currentMovie.getName() << " Release Year: " << currentMovie.getYear() << " Genre: " << currentMovie.getGenre() << endl;
                }
            }
        }
    } else {
        // Input contains a single year to check
        int currentYear = atoi(selectedYear);

        // Check for the presence of the desired year
        int flag = 0;
        for (int i = 0; i < givenVec.size(); i++)
        {
            MyClassMovie currentMovie = givenVec[i];
            if (currentMovie.getYear() == currentYear)
            {
                flag += 1;
                break;
            }
        }

        if (flag == 0)
        {
            cout << "There are no movies released in " << currentYear << "!\n";
        } else {
            cout << "Movies released in " << currentYear << " from A to Z:\n";
            for (int i = 0; i < givenVec.size(); i++)
            {
                MyClassMovie currentMovie = givenVec[i];
                int tempYear = currentMovie.getYear();
                if (tempYear == currentYear)
                {
                    cout << "Movie name: " << currentMovie.getName() << " Genre: " << currentMovie.getGenre() << endl;
                }
            }
        }
    }
}

string funcMyLowerString(const string s)
{
    string word = "";
    int len = s.length();
    for(int k=0; k < len; k++)
    {
        if ('A' <= s[k] && s[k] <= 'Z')
        {
            char temp = s[k] + ('a' - 'A');
            word += temp;
        } else {
            word += s[k];
        }
    }
    return word;
}