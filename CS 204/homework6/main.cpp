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

// Global variables
IntQueueHW6 chairs(0);  // Queue representing the chairs
mutex mtx;  // Mutex for synchronization
vector<int> eliminated; // For checking elimination status of each player
vector<int> IDofPlayer;

// Thread function for each player
void playerThread(int playerID, struct tm *ptm)
{
    // this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(2));
    this_thread::sleep_until(chrono::system_clock::from_time_t(mktime(ptm)));
    mtx.lock();
    ostringstream os;
    if (!chairs.isFull())
    {
        // eliminated[playerID] = 0;
        eliminated.push_back(playerID);
        time_t player_time = chrono::system_clock::to_time_t (chrono::system_clock::now());  //gets the current time
        struct tm *player_ptm = localtime(&player_time);
        chairs.enqueue(playerID);
        os << "Player " << playerID << " captured a chair at " << put_time(player_ptm,"%X") << ".\n";
        cout << os.str();
    }
    else
    {
        // eliminated[playerID] = 1;
        os << "Player " << playerID << " couldn't capture a chair.\n";
        cout << os.str();
    }
    mtx.unlock();
}

int main()
{
    // Take input for the number of players";
    cout << "Welcome to Musical Chairs game!\n";
    cout << "Enter the number of players in the game:\n"; 
    int numPlayers;
    cin >> numPlayers;

    // Initialize the queue with the number of players - 1 chairs
    chairs = IntQueueHW6(numPlayers - 1);
    
    vector<thread> threads(numPlayers);
    // vector<bool> eliminated(numPlayers, false);
    // eliminated(numPlayers, false);

    for (int i = 0; i < numPlayers; i++)
    {
        IDofPlayer.push_back(i);
    }

    // static vector<thread> threads(numPlayers);
    cout << "Game Start!\n";
    int copyNumPlayers = numPlayers;
    // while (copyNumPlayers > 1)
    while (numPlayers > 1)
    {
        // // Display current time
        time_t current_time = chrono::system_clock::to_time_t (chrono::system_clock::now());  //gets the current time
        struct tm *ptm = localtime(&current_time);
        cout << "\nTime is now " << put_time(ptm,"%X") << endl;  //displaying the time
        ptm->tm_sec += 2;

        // Handle minute overflow by incrementing the minute and resetting the seconds
        if (ptm->tm_sec >= 60)
        {
            //cout << "\tsecond was greater equal to 60.\n";
            ptm->tm_sec %= 60;
            ptm->tm_min++;
        }
         
        // Start player threads
        for (int i = 0; i < numPlayers; i++)
        {
            // if (eliminated[i] == 0) // Skip eliminated players
            // { 
            //     threads[i] = thread(playerThread, i, ptm);
            // }
            threads[i] = thread(playerThread, IDofPlayer[i], ptm);
        }

        // Join player threads
        for (int i = 0; i < numPlayers; i++)
        {
            if (threads[i].joinable())
            { threads[i].join(); }
        }

        // IntQueueHW6 tempChairs = chairs;
        // Display the IDs of players who captured a chair
        cout << "Remaining players are as follows: ";
        // while (!chairs.isEmpty())
        // {
        //     int playerID;
        //     chairs.dequeue(playerID);
        //     cout << playerID << " ";
        // }
        // cout << endl;
        for (int id : eliminated)
        {
            cout << id << " ";
        }
        cout << endl;

        IDofPlayer = eliminated;
        eliminated.clear();

        // Eliminate a player
        // copyNumPlayers -= 1;
        numPlayers -= 1;

        // // Identify the player who couldn't capture a chair and mark them as eliminated
        // int eliminated_player_id;
        // tempChairs.dequeue(eliminated_player_id);
        // eliminated[eliminated_player_id] = true;

        // Decrease the number of chairs by one
        chairs = IntQueueHW6(numPlayers - 1);
    }
    cout << "\nGame over!\n";
    // cout << "Winner is Player " << currentPlayerID << "!\n";

    // try to dequeue te chairs and see if you can get the winner
    int winnerID = -1;
    winnerID = IDofPlayer[0];
    cout << "Winner is Player " << winnerID << "!\n";

    return 0;
}
