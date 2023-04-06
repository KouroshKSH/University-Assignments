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
    bool noMove(char, int);
    int validMove(char, int, int, int);
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