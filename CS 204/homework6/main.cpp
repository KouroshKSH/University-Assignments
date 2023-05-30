/*
The purpose of the program:
- Create a queue object using the IntQueueHW6 class to represent the chairs.
- Prompt the user for the number of players and create the corresponding number of threads.
- Implement a thread function where each player thread tries to enqueue its ID to the queue using mutex locks.
- Synchronize the start time for all players using `sleep_until`.
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
vector<int> notEliminated; // For checking elimination status of each player
vector<int> IDofPlayer; // For having ID's of players in check

// Thread function for each player
void playerThread(int playerID, struct tm *ptm); 

int main()
{
    // Prompt the beginning of the game
    cout << "Welcome to Musical Chairs game!\n";
    cout << "Enter the number of players in the game:\n"; 
    int numPlayers; cin >> numPlayers;

    chairs = IntQueueHW6(numPlayers - 1); // Initialize the queue for chairs
    vector<thread> threads(numPlayers); // Initialize the thread for players

    for (int i = 0; i < numPlayers; i++)
    { IDofPlayer.push_back(i); } // Add all ID's in the beginning

    cout << "Game Start!\n";
    while (numPlayers > 1) // The game shall be played until 2 players remaining
    {
        // Display current time
        time_t current_time = chrono::system_clock::to_time_t (chrono::system_clock::now());
        struct tm *ptm = localtime(&current_time);
        cout << "\nTime is now " << put_time(ptm,"%X") << endl; 
        ptm->tm_sec += 2;

        // Handle minute overflow
        if (ptm->tm_sec >= 60)
        {
            ptm->tm_sec %= 60;
            ptm->tm_min++;
        }
         
        // Start player threads
        for (int i = 0; i < numPlayers; i++)
        { threads[i] = thread(playerThread, IDofPlayer[i], ptm); }

        // Join player threads
        for (int i = 0; i < numPlayers; i++)
        {
            if (threads[i].joinable())
            { threads[i].join(); }
        }

        // Display the IDs of players who captured a chair
        cout << "Remaining players are as follows: ";
        for (int id : notEliminated)
        { cout << id << " "; }
        cout << endl;

        // Reduce the player count for next round
        IDofPlayer = notEliminated;
        notEliminated.clear();
        numPlayers -= 1;

        chairs = IntQueueHW6(numPlayers - 1); // Decrease the number of chairs by one
    }

    // The winner is the first player in the list of ID's
    int winnerID = IDofPlayer[0];
    cout << "\nGame over!\nWinner is Player " << winnerID << "!\n";

    return 0;
}

void playerThread(int playerID, struct tm *ptm)
{
    this_thread::sleep_until(chrono::system_clock::from_time_t(mktime(ptm))); // Sleep before mutex for competition
    mtx.lock();
    ostringstream os; // For more secure printing
    if (!chairs.isFull())
    {
        notEliminated.push_back(playerID); // Update the list of players still in game
        time_t player_time = chrono::system_clock::to_time_t (chrono::system_clock::now());
        struct tm *player_ptm = localtime(&player_time);
        chairs.enqueue(playerID); // Assign the player to a chair
        os << "Player " << playerID << " captured a chair at " << put_time(player_ptm,"%X") << ".\n";
        cout << os.str();
    }
    else
    {
        os << "Player " << playerID << " couldn't capture a chair.\n";
        cout << os.str();
    }
    mtx.unlock();
}
