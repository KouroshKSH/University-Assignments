#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include "IntQueueHW6.h"

using namespace std;
using namespace this_thread;

void func_hello(int order); // experimental
void func_handle_overflow(); // experimental

int main()
{
    // show starting prompts
    cout << "Welcome to Musical Chairs game!\n";
    cout << "Enter the number of players in the game:\n"; 
    int num_of_players;
    cin >> num_of_players;
    int num_of_chairs = num_of_players - 1;

    cout << "Game Start!\n\n";
    time_t tt = chrono::system_clock::to_time_t (chrono::system_clock::now());  //gets the current time
    struct tm *ptm = localtime(&tt);
    cout << "Time is now " << put_time(ptm,"%X") << endl;  //displaying the time 

    /*experimental*/
    // create a thread
    // thread my_thread(func_hello);
    // if (my_thread.joinable())
    // { my_thread.join(); }
    /*experimental*/

    /*experimental*/
    // create an array of threads
    thread threads[num_of_players];
    for (int i = 0; i < num_of_players; i++)
    { threads[i] = (thread(&func_hello, i)); }

    for (int i = 0; i < num_of_players; i++)
    { threads[i].join(); }
    /*experimental*/

    /*experimental*/
    func_handle_overflow();
    /*experimental*/

    cout << "Game over!\n";
    cout << "Winner is Player 3!\n"; // change this
    return 0;
}

void func_hello(int order)
{
    ostringstream os;
    os << order << ": " << get_id() << endl;
	cout << os.str();
    // cout << "this thread: " << get_id() << endl;
}

void func_handle_overflow()
{
    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* ptm = std::localtime(&tt);

    std::cout << "Time is now " << std::put_time(ptm, "%X") << endl;

    // Checking if the current minute is 59
    if (ptm->tm_min == 59) {
        // If the current minute is 59, set the minute to 0 and increment the hour
        cout << "min was 59\n";
        ptm->tm_min = 0;
        ptm->tm_hour++;
    }
    else {
        // If the current minute is not 59, simply increment the minute
        cout << "min was not 59\n";
        ptm->tm_min++;
    }
    ptm->tm_sec = 0;

    cout << "Updated time is " << std::put_time(ptm, "%X") << endl;
}