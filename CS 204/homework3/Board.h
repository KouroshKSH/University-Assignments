#ifndef BOARD_H
#define BOARD_H

#include "CharStack.h"

struct slot
{
    slot * next; slot * prev;
    CharStack slotStack;
    slot () {} // defeault
};

class Board
{
private:
    slot * head;  slot * tail;
    int xCnt; int oCnt;

public:
    Board();
    bool noMove(const char, const int);
    int validMove(const char, const int, const int, const int);
    void movePiece(const int, const int);
    void printBoard();
    int evaluateGame();
    bool targetSlotFull(const int);
    void destroySlot(const int);
    void createSlotBegin(const char, const int);
    void createSlotEnd(const char, const int);
    void createEmptySlotEnd();
    void clearBoard();
};

#endif
