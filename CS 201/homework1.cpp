#include <iostream>
#include <string>

using namespace std;

// Call all the functions used
void getNameFunc();
int calculateGradeFunc(string user_name);

int main()
{
    getNameFunc();
}

void getNameFunc()
{
    // Prompt the user to enter his name
    cout << "The purpose of this program is to calculate your CS201 grade. What is your name? ";
    string user_name;
    cin >> user_name;

    // Proceed to find the numerical grade of the user
    calculateGradeFunc(user_name);
}

int calculateGradeFunc(string user_name)
{
    // Get exam grades
    cout << "Hello " << user_name 
    << ", what are your midterm and final exam grades? ";
    double mid_grade, fin_grade;
    cin >> mid_grade >> fin_grade;

    // Calculate the weighted average of the 2 exams
    double weight_exam_avg = ((0.3 * mid_grade) + (0.35 * fin_grade)) / 0.65;
    if (weight_exam_avg < 30.0)
    {
        cout << user_name 
        << ", your weighted exam average is less than 30, you will fail the course!"
        << endl;

        // Finish in case it is less than 30
        return 0;
    }
    else
    {
        // Continue the program since the exam average was NOT less than 30
        // Get homework grades
        cout << "What are the grades of your 5 homework? ";
        double hw1, hw2, hw3, hw4, hw5;
        cin >> hw1 >> hw2 >> hw3 >> hw4 >> hw5;

        // Assign the proper value to the homework grade based on calculations
        double hw_grade, hw_avg = (hw1 + hw2 + hw3 + hw4 + hw5) / 5, ratio = hw_avg / weight_exam_avg;
        if (ratio <= 2)
        {
            hw_grade = hw_avg;
            cout << "You'll get all the points earned from homework!" << endl;
        }
        else if ((2 < ratio) && (ratio < 3))
        {
            hw_grade = hw_avg * (3 - ratio);
            cout << "You'll get " << hw_grade << " from the points earned from homework!" << endl;
        }
        else
        {
            hw_grade = 0;
            cout << "You'll get NONE of the points earned from homework!" << endl;
        }

        // Assume that quizzes are out of 10 and the lowest will be dropped
        cout << "What are the grades of your 4 quizzes? If any, enter your grade as 0 for missed quizzes. ";
        double qz1, qz2, qz3, qz4, min_quiz = 11.0; // 11 will be used for min-swapping
        cin >> qz1 >> qz2 >> qz3 >> qz4;

        // Find the minimum quiz grade
        if (qz1 < min_quiz)
        {
            min_quiz = qz1;
        }
        if (qz2 < min_quiz)
        {
            min_quiz = qz2;
        }
        if (qz3 < min_quiz)
        {
            min_quiz = qz3;
        }
        if (qz4 < min_quiz)
        {
            min_quiz = qz4;
        }

        // The sum of all quizzes minus min is equivalent to dropping the lowest
        double quiz_grade = (qz1 + qz2 + qz3 + qz4 - min_quiz) / 30.0 * 100.0;

        // Find the final numeric grade given the formula
        double course_numeric_grade = (quiz_grade * 0.10) + (hw_grade * 0.25) + (mid_grade * 0.30) + (fin_grade * 0.35);
        cout << user_name << ", your grade for CS201 is: " << course_numeric_grade << endl;

        return 0;
    }
}