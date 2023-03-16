#include <iostream>
#include <string>
#include "strutils.h"

using namespace std;

// Function prototype
void funcInputCheck(int a, int &b, bool &c);
void funcQuestionParse(string s, int &a, bool &b);
void funcCheckResult(int a, int b, int c, char d, string e, int &f, bool &g);
bool funcCheckUnacceptChar(string s);
bool funcCheckEqualSigns(string s);
bool funcEqSignPlace(string s);


int main()
{
    int userPoints = 0;
    bool jokerChance = true;
    for (int i = 1; i < 5; i++)
    {
        funcInputCheck(i, userPoints, jokerChance);
    }

    // Points can be a maximum of 100 points and a minimum of 0
    if (userPoints < 0)
    {
        cout << "End of the program. You got 0 points in total." << endl;
    } else {
        cout << "End of the program. You got " << userPoints << " points in total." << endl;
    }

    return 0;
}

void funcInputCheck(int counter, int &userPoints, bool &jokerChance)
{
    cout << "Please enter question #" << counter << " and its answer: ";
    string userInput;
    cin >> userInput;

    int wrongAns = 10;
   
    // Check for digits, plus, minus & equal signs
    if (funcCheckUnacceptChar(userInput))
    {
        // Check for exactly 1 equal sign
        if (funcCheckEqualSigns(userInput))
        {
            // Check for the placement of equal sign
            if (funcEqSignPlace(userInput))
            {
                funcQuestionParse(userInput, userPoints, jokerChance);
            } else {
                cout << "Wrong input format! You got -10 penalty points for this math expression.\n" << endl;
                userPoints -= wrongAns;
            }
        } else {
            cout << "Wrong input format! You got -10 penalty points for this math expression.\n" << endl;
            userPoints -= wrongAns;
        }
    } else {
        cout << "Wrong input format! You got -10 penalty points for this math expression.\n" << endl;
        userPoints -= wrongAns;
    } 
}


void funcQuestionParse(string userInput, int &userPoints, bool &jokerChance)
{
    int num1, num2, num3, idx, i;
    char opSign = ' ';

    if (userInput.at(0) == '-')
    {
        // If the 1st number is negative, skip the first index
        i = 1;
    } else {
        i = 0;
    }

    while (i < userInput.length() - 1)
    {
        if (!(isdigit(userInput.at(i))))
        {
            // Determines the index of the operator
            idx = i;
            break;
        }
        i++;
    }

    // Determine 1st number (substring from beginning until operator sign)
    num1 = stoi(userInput.substr(0, idx));
    
    // Determine 2nd number (substring after operator sign until equal sign)
    num2 = stoi(userInput.substr(idx + 1, userInput.find("=") - idx - 1));

    // Determine 3rd number (sbstring after equal sign until the end)
    num3 = stoi(userInput.substr(userInput.find("=") + 1, userInput.length() - 1));

    // Determine operator (addition or subtraction)
    if (userInput.at(idx) == '+')
    {
        opSign = '+';
    } else {
        opSign = '-';
    }

    funcCheckResult(num1, num2, num3, opSign, userInput, userPoints, jokerChance);
}

void funcCheckResult(int num1, int num2, int num3, char opSign, string userInput, int &userPoints, bool &jokerChance)
{
    int correctAns = 25, wrongAns = 10;
    string userAns;

    // Adding numbers
    if (opSign == '+')
    {
        // Mathematically correct
        if (num1 + num2 == num3)
        {
            userPoints += correctAns;
            cout << "Correct answer! You got 25 points for this math expression.\n" << endl;
        } else {
            // Mathematically incorrect
            if (jokerChance)
            {
                // When user still has a joker
                cout << "Wrong!\nWould you like to use your joker option to correct the answer? ";
                cin >> userAns;
                ToLower(userAns);
                if (userAns == "no")
                {
                    // Display correct answer and move on
                    cout << "Sorry! The answer should have been: " << num1 + num2 << ". You got 0 points for this math expression.\n" << endl;
                } else if (userAns == "yes")
                {
                    // Give user a second chance
                    cout << "Please enter the expression again: ";
                    string secondChance;
                    cin >> secondChance;

                    if (secondChance.find("=") == string::npos)
                    {
                        // Show problem in case of no equal sign
                        cout << "Wrong input format! You got -10 penalty points for this joker option.\n" << endl;
                        userPoints -= wrongAns;
                        jokerChance = false;
                    } else {
                        int idx = secondChance.find("=");
                        if (userInput.substr(0, idx) != secondChance.substr(0, idx))
                        {
                            // Check if everything before equal sign is exactly the same
                            cout << "Wrong input format! You got -10 penalty points for this joker option.\n" << endl;
                            userPoints -= wrongAns;
                            jokerChance = false;
                        } else if (stoi(secondChance.substr(secondChance.find("=") + 1, secondChance.length() - 1)) != (num1 + num2))
                        {
                            // Mathematically incorrect
                            cout << "Sorry! The answer should have been: " << num1 + num2 << ". You got -10 penalty points for this joker option.\n" << endl;
                            userPoints -= wrongAns;
                            jokerChance = false;
                        } else if (stoi(secondChance.substr(secondChance.find("=") + 1, secondChance.length() - 1)) == (num1 + num2))
                        {
                            // Mathematically correct
                            cout << "Correct answer! You got 25 points for this joker option.\n" << endl;
                            userPoints += correctAns;
                            jokerChance = false;
                        }
                    } 
                }
            } else {
                // When user doesn't have joker anymore & is mathematically incorrect
                cout << "Sorry! The answer should have been: " << num1 + num2 << ". You got 0 points for this math expression.\n" << endl;
            }
        }
    }
    
    // Subtracting numbers
    if (opSign == '-')
    {
        if (num1 - num2 == num3)
        {
            userPoints += correctAns;
            cout << "Correct answer! You got 25 points for this math expression.\n" << endl;
        } else {
            if (jokerChance)
            {
                cout << "Wrong!\nWould you like to use your joker option to correct the answer? ";
                cin >> userAns;
                ToLower(userAns);
                if (userAns == "no")
                {
                    cout << "Sorry! The answer should have been: " << num1 - num2 << ". You got 0 points for this math expression.\n" << endl;
                } else if (userAns == "yes")
                {
                    cout << "Please enter the expression again: ";
                    string secondChance;
                    cin >> secondChance;

                    if (secondChance.find("=") == string::npos)
                    {
                        cout << "Wrong input format! You got -10 penalty points for this joker option.\n" << endl;
                        userPoints -= wrongAns;
                        jokerChance = false;
                    } else {
                        int idx = secondChance.find("=");
                        if (userInput.substr(0, idx) != secondChance.substr(0, idx))
                        {
                            cout << "Wrong input format! You got -10 penalty points for this joker option.\n" << endl;
                            userPoints -= wrongAns;
                            jokerChance = false;
                        } else if (stoi(secondChance.substr(secondChance.find("=") + 1, secondChance.length() - 1)) != (num1 - num2))
                        {
                            cout << "Sorry! The answer should have been: " << num1 - num2 << ". You got -10 penalty points for this joker option.\n" << endl;
                            userPoints -= wrongAns;
                            jokerChance = false;
                        } else if (stoi(secondChance.substr(secondChance.find("=") + 1, secondChance.length() - 1)) == (num1 - num2))
                        {
                            cout << "Correct answer! You got 25 points for this joker option.\n" << endl;
                            userPoints += correctAns;
                            jokerChance = false;
                        }
                    }
                }
            } else {
                cout << "Sorry! The answer should have been: " << num1 - num2 << ". You got 0 points for this math expression.\n" << endl;
            }
        }
    }
}


bool funcCheckUnacceptChar(string userInput)
{
    // Check for anything other than numbers, plus, minus, and equal signs
    string list = "0123456789+-=";
    for (int i = 0; i < userInput.length(); i++)
    {
        if (list.find(userInput.at(i)) == string::npos)
        {
            return false;
        }
    }
    
    return true;
}


bool funcCheckEqualSigns(string userInput)
{   
    // Count the number of equal signs
    int counter = 0;
    
    for (int i = 0; i < userInput.length(); i++)
    {
        if (userInput.at(i) == '=')
        {
            counter += 1;
        }
    }
    
    // Any number of instances other than one is problematic
    return (counter == 1);
}


bool funcEqSignPlace(string userInput)
{
    // Check if the equal sign is at the wrong place
    if ((userInput.find('=') < 3) || (userInput.find('=') == userInput.length() - 1))
    {
        // The equal sign can never be in the first 3 places (1+1=2)
        // It also can not be the last character
        return false;
    }

    return true;
}