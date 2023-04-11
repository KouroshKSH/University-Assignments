#ifndef BOARD_H
#define BOARD_H

#include "CharStack.h"

struct slot
{
    slot * next;
    slot * prev;
    CharStack slotStack;
    slot () {} // defeault
};

class Board
{
private:
    slot * head; 
    slot * tail;
    int xCnt;
    int oCnt;

public:
    Board();
    bool noMove(const char, const int);
    int validMove(const char, const int, const int, const int);
    void movePiece(int, int);
    void printBoard();
    int evaluateGame();
    bool targetSlotFull(int);
    void destroySlot(int);
    void createSlotBegin(char, int);
    void createSlotEnd(char, int);
    void createEmptySlotEnd();
    void clearBoard();
};

#endif
