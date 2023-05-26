/*
To implement this program, we will need to:
- Create a queue object using the IntQueueHW6 class to represent the chairs.
- Prompt the user for the number of players and create the corresponding number of threads.
- Implement a thread function where each player thread tries to enqueue its ID to the queue using mutex locks.
- Synchronize the start time for all players using sleep_until.
- Keep track of the players who successfully capture a chair in each round and eliminate the player who failed.
- Decrement the number of chairs for each round.
- Join the threads properly after each round.
- Print the IDs of players who captured a chair in each round.
- Continue the game until only one player is left as the winner.
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include "IntQueueHW6.h"

using namespace std;
using namespace this_thread;

void func_hello(int order); // experimental
void func_handle_overflow(); // experimental
void func_testing_q(int size); // experimental
void funcPlayerThread(int playerID, IntQueueHW6& chairQ, struct tm *ptm);
void funcPrintQ(IntQueueHW6 my_q); // for debugging purposes

mutex mutexQ;

int main()
{
    // show starting prompts
    cout << "Welcome to Musical Chairs game!\n";
    cout << "Enter the number of players in the game:\n"; 
    int num_of_players;
    cin >> num_of_players;
    int num_of_chairs = num_of_players - 1;

    IntQueueHW6 chairQ(num_of_chairs);
    // Create an array to track eliminated players
    vector<bool> eliminated(num_of_players, false);
    
    cout << "Game Start!\n";

    while (num_of_chairs >= 2)
    {
        time_t current_time = chrono::system_clock::to_time_t (chrono::system_clock::now());  //gets the current time
        struct tm *ptm = localtime(&current_time);
        cout << "\nTime is now " << put_time(ptm,"%X") << endl;  //displaying the time 
        // Calculate the start time for the players, which is 2 seconds after the current time
        ptm->tm_sec += 2;

        // Handle minute overflow by incrementing the minute and resetting the seconds
        if (ptm->tm_sec >= 60)
        {
            //cout << "\tsecond was greater equal to 60.\n";
            ptm->tm_sec %= 60;
            ptm->tm_min++;
        }

        // Sleep until the calculated start time
        this_thread::sleep_until(chrono::system_clock::from_time_t(mktime(ptm)));
        
        // create the threads for players
        // IntQueueHW6 chairQ(num_of_chairs);
        vector<thread> player_threads(num_of_players); // define as vector due to dynamic size
        for (int i = 0; i < num_of_players; i++)
        {
            if (!eliminated[i]) // Skip eliminated players
            { player_threads[i] = thread(funcPlayerThread, i, ref(chairQ), ptm); }
            // player_threads[i] = (thread(funcPlayerThread, i, ref(chairQ), ptm));
        }
        for (int i = 0; i < num_of_players; i++)
        {
            if (player_threads[i].joinable())
            { player_threads[i].join(); }
        }

        // print the queue
        cout << "Remaining players are as follows: ";
        funcPrintQ(chairQ);

        // Identify the player who couldn't capture a chair and mark them as eliminated
        int eliminated_player_id;
        chairQ.dequeue(eliminated_player_id);
        eliminated[eliminated_player_id] = true;

        num_of_players--;
        num_of_chairs--; // reduce these every round
        chairQ.clear(); // reset the chairs for the next round
    }

    cout << "\nGame over!\n";
    // int winnerID;
    // chairQ.dequeue(winnerID);
    // cout << "Winner is Player " << winnerID << "!\n"; 
        // Find the winner among the remaining players
    int winner_id = -1;
    for (int i = 0; i < num_of_players; i++)
    {
        if (!eliminated[i])
        {
            winner_id = i;
            break;
        }
    }

    cout << "Winner is Player " << winner_id << "!\n";
    return 0;


    /*experimental*/
    // create a thread
    // thread my_thread(func_hello);
    // if (my_thread.joinable())
    // { my_thread.join(); }
    /*experimental*/

    /*experimental*/
    // create an array of threads
    // thread threads[num_of_players];
    // for (int i = 0; i < num_of_players; i++)
    // { threads[i] = (thread(&func_hello, i)); }

    // for (int i = 0; i < num_of_players; i++)
    // { threads[i].join(); }
    /*experimental*/

    /*experimental*/
    // func_handle_overflow();
    /*experimental*/

    /*experimental*/
    // func_testing_q(num_of_chairs);
    /*experimental*/
}

void funcPlayerThread(int playerID, IntQueueHW6& chairQ, struct tm *ptm)
{
    // cout << "\tstarting player thread and sleep...\n";
    // cout << "player " << get_id() << " slept for 2 seconds.\n";


    mutexQ.lock();
    // cout << "\t\t=== mutex is locked ===\n";
    // print the id of the player
    // ostringstream os;
    // os << "for player " << playerID << ": " << get_id() << endl;
	// cout << os.str();

    if (!chairQ.isFull())
    {
        chairQ.enqueue(playerID);
        time_t player_time = chrono::system_clock::to_time_t (chrono::system_clock::now());  //gets the current time
        struct tm *player_ptm = localtime(&player_time);
        cout << "Player " << playerID << " captured a chair at " << put_time(player_ptm,"%X") << ".\n";  //displaying the time 
    } else {
        cout << "Player " << playerID << " couldn't capture a chair.\n";
    }
    // cout << "enqueued the player ID.\n";

    // cout << "\t\t=== unlocking mutex... ===\n";
    mutexQ.unlock();

    
}

void funcPrintQ(IntQueueHW6 my_q)
{
    while (!my_q.isEmpty()) 
    {
        int currNum;
        my_q.dequeue(currNum);
        cout << currNum << " ";
    }
    cout << endl;
}

void func_testing_q(int size)
{
    IntQueueHW6 my_q(size);
    if (my_q.isEmpty())
    {
        cout << "q is empty\n";
    } else {
        cout << "q is not empty\n";
    }
    cout << "adding new elements to q\n";
    while (!my_q.isFull())
    { my_q.enqueue(10); }
    if (my_q.isEmpty())
    {
        cout << "q is empty\n";
    } else {
        cout << "content of q:\n";
        while (!my_q.isEmpty()) 
        {
            int currNum;
            my_q.dequeue(currNum);
            cout << currNum << " ";
        }
        cout << endl;
    }
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