#include <iostream>
#include <string>
#include <vector>
#include "strutils.h"
#include "randgen.h"

using namespace std;

struct MovieStruct
{
    string nameOfMovie;
    unsigned int yearOfMovie;
    string genreOfMovie;
};

void funcPrintVector(vector < MovieStruct > givenVec);
void funcInsertionSort(vector < MovieStruct > &givenVec);
void funcRandomMovie(vector < MovieStruct > givenVec);
int funcChoiceValidity(string inputString);
bool funcCompareMovies(const MovieStruct& movie1, const MovieStruct& movie2);
void funcSelectedGenre(const vector < MovieStruct > givenVec, string selectedGenre);
void funcSelectedYear(const vector < MovieStruct > givenVec, string selectedYear);

int main()
{
    cout << "Welcome to the movie recommender program!\n\n";
    cout << "Please enter the movie, year and genre list:\n";
    string originalInput;
    cin >> originalInput;

    vector < MovieStruct > listOfMoviesVec;

    // For the edge-case where the list of movies ends with a '|' character
    if (originalInput[originalInput.size() - 1] == '|')
    {
        originalInput = originalInput.substr(0, originalInput.size() - 1);
    }

    // Differentiate the movies by the vertical line character
    while (originalInput.find("|") != string::npos)
    {
        MovieStruct movie1;
        int verticalLineLoc = originalInput.find('|');
        string temp = originalInput.substr(0, verticalLineLoc);
        int semicolonLoc = temp.find(';');
        movie1.nameOfMovie = temp.substr(0, semicolonLoc);
        temp = temp.substr(semicolonLoc + 1);
        semicolonLoc = temp.find(';');
        movie1.yearOfMovie = stoi(temp.substr(0, 4));
        temp = temp.substr(semicolonLoc + 1);
        movie1.genreOfMovie = temp;

        // Add the current movie the vector of list of movies
        listOfMoviesVec.push_back(movie1);

        // Update the input of movies by getting rid of the first movie's string
        originalInput = originalInput.substr(verticalLineLoc + 1);
    }

    // To store the last movie manually
    MovieStruct lastMovie;
    int verticalLineLoc = originalInput.find('|');
    string temp = originalInput.substr(0, verticalLineLoc);
    int semicolonLoc = temp.find(';');
    lastMovie.nameOfMovie = temp.substr(0, semicolonLoc);
    temp = temp.substr(semicolonLoc + 1);
    semicolonLoc = temp.find(';');
    lastMovie.yearOfMovie = stoi(temp.substr(0, 4));
    temp = temp.substr(semicolonLoc + 1);
    lastMovie.genreOfMovie = temp;
    listOfMoviesVec.push_back(lastMovie);

    // Show prompt for choosing action
    cout << "\nPlease select your action:\n";
    cout << "1. Select the genre of the movie that you want to watch\n";
    cout << "2. Select the year of the movie that you want to watch\n";
    cout << "3. Choose a random movie\n";
    cout << "4. Exit program\n";
    
    funcInsertionSort(listOfMoviesVec);
    
    bool validChoiceBool = true;
    while (validChoiceBool)
    {
        cout << "\nEnter your choice: ";
        string actionInputString;
        cin >> actionInputString;
        int actionInputNumber = funcChoiceValidity(actionInputString);
        if (actionInputNumber == 1)
        {
            cout << "Please enter the genre of the movie you want to watch: ";
            string desiredGenre;
            cin >> desiredGenre;
            funcSelectedGenre(listOfMoviesVec, desiredGenre);
        } else if (actionInputNumber == 2)
        {
            cout << "Please enter the year of the movie you want to watch: ";
            string desiredYear;
            cin >> desiredYear;
            funcSelectedYear(listOfMoviesVec, desiredYear);
        } else if (actionInputNumber == 3)
        {
            funcRandomMovie(listOfMoviesVec);

        } else if (actionInputNumber == 4)
        {
            cout << "Thank you...\n";
            validChoiceBool = false;
        } else if (actionInputNumber <= 0 || actionInputNumber >= 5){
            cout << "Invalid action!\n";
        }
    }
}

void funcPrintVector(vector < MovieStruct > givenVec)
{
    for (MovieStruct i : givenVec)
    {
        cout << "\nName of movie: " << i.nameOfMovie 
            << "\nYear of movie: " << i.yearOfMovie
            << "\nGenre of movie: " << i.genreOfMovie << endl;
    }
}

void funcInsertionSort(vector <MovieStruct> &givenVec)
{
    // Sort using Insertion Sort's algorithm
    for (int i = 1; i < givenVec.size(); i++)
    {
        MovieStruct current = givenVec[i];
        int j = i - 1;
        while (j >= 0 && funcCompareMovies(givenVec[j], current)) {
            givenVec[j + 1] = givenVec[j];
            j--;
        }
        givenVec[j + 1] = current;
    }
}

void funcRandomMovie(vector < MovieStruct > givenVec)
{
    // Generate the random number given the already known seed 
    RandGen rand;
    rand.SetSeed(1907);
    int i = rand.RandInt(0, givenVec.size() - 1);

    // Store the parameters respectively
    string randomMovieName = givenVec[i].nameOfMovie;
    int randomMovieYear = givenVec[i].yearOfMovie;
    string randomMovieGenre = givenVec[i].genreOfMovie;

    cout << "Movie name: " << randomMovieName << " Release Year: " << randomMovieYear << " Genre: " << randomMovieGenre << endl;
}

int funcChoiceValidity(string actionInputString)
{
    // Return the integer equivalent of the choice of the user (if valid)
    int actionInputNumber = 0;
    if (actionInputString[0] == '-')
    {
        // When the number is negative,
        // it's automatically invalid
        return actionInputNumber;
    } else { // Input is positive
        // Convert each character of the string to a digit
        for (char ch : actionInputString)
        {
            if (ch >= '0' && ch <= '9')
            {
                // Store the characters as digits by...
                /// ... multiplying the previous value of number by 10 and...
                // ... adding the difference of character with the character zero
                actionInputNumber = (actionInputNumber * 10) + (ch - '0');
            } else {
                // Store 0 when there is anything other than digits in the string
                actionInputNumber = 0;
                break;
            } 
        }
        return actionInputNumber;
    }
}

bool funcCompareMovies(const MovieStruct& movie1, const MovieStruct& movie2)
{
    // Check years first
    if (movie1.yearOfMovie != movie2.yearOfMovie)
    {
        return movie1.yearOfMovie < movie2.yearOfMovie;
    } else {
        // If both movies share the same year,
        // ... compare based on name
        return movie1.nameOfMovie > movie2.nameOfMovie;
    }
}

void funcSelectedGenre(const vector < MovieStruct > givenVec, string selectedGenre)
{
    // Check for the presence of the desired genre
    int flag = 0;
    string copySelectedGenre = selectedGenre;
    ToLower(copySelectedGenre);
    for (int i = 0; i < givenVec.size(); i++)
    {
        // Create a copy of each movie struct to avoid modification
        MovieStruct currentMovie = givenVec[i];

        // Lowercase for absolute comparison
        ToLower(currentMovie.genreOfMovie);
        if (currentMovie.genreOfMovie == copySelectedGenre)
        {
            // Get out of the loop as soon as possible to avoid extra computation
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

        // Iterate again, but for printing this time
        for (int i = 0; i < givenVec.size(); i++)
        {
            MovieStruct currentMovie = givenVec[i];
            ToLower(currentMovie.genreOfMovie);
            if (currentMovie.genreOfMovie == copySelectedGenre)
            {
                cout << "Movie name: " << currentMovie.nameOfMovie << " Release Year: " << currentMovie.yearOfMovie << endl;
            }
        }
    }
}

void funcSelectedYear(const vector < MovieStruct > givenVec, string selectedYear)
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
            MovieStruct currentMovie = givenVec[i];
            if (currentMovie.yearOfMovie >= startingYear && currentMovie.yearOfMovie <= endingYear)
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
                MovieStruct currentMovie = givenVec[i];
                if (currentMovie.yearOfMovie >= startingYear && currentMovie.yearOfMovie <= endingYear)
                {
                    cout << "Movie name: " << currentMovie.nameOfMovie << " Release Year: " << currentMovie.yearOfMovie << " Genre: " << currentMovie.genreOfMovie << endl;
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
            MovieStruct currentMovie = givenVec[i];
            if (currentMovie.yearOfMovie == currentYear)
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
                MovieStruct currentMovie = givenVec[i];
                if (currentMovie.yearOfMovie == currentYear)
                {
                    cout << "Movie name: " << currentMovie.nameOfMovie << " Genre: " << currentMovie.genreOfMovie << endl;
                }
            }
        }
    }
}