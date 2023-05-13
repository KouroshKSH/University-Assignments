#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Player
{
    private:
        Board & my_board; // reference variable
        string player_name;
        int player_balance;
        Node * player_position;

    public:
    // [ ] fix the arguments for each function
        // [ ] write constructors such as `Player();`
        int move();
        Node * where_am_I();
        void pay_to_player();
        int get_balance();
        void deposit_money();
        void buy_slot();
        bool is_bankrupt();
        void display();
};

#endif