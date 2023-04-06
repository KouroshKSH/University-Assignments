#include <iostream>
#include "Board.h"
#include "CharStack.h"

using namespace std;

Board::Board() // done
{
    head = NULL; 
    tail = NULL;
    xCnt = 0;
    oCnt = 0;
}

bool Board::noMove(char plyChr, int die)
{
    return false;
}

int Board::validMove(char plyChr, int choice, int die, int right)
{
    return 0;
}

void Board::movePiece(int choice, int targetInd)
{
    cout << "\nmove piece\n";
}

void Board::printBoard()
{
    cout << "\nprint board\n";
}

int Board::evaluateGame()
{
    return 0;
}

bool Board::targetSlotFull(int targetInd)
{
    return false;
}

void Board::destroySlot(int targetInd)
{
    cout << "\ndestroy slot\n";
}

void Board::createSlotBegin(char plyChr, int num)
{
    cout << "\ncreate slot begin\n";
}

void Board::createSlotEnd(char plyChr, int num) // current
{
    cout << "\ncreate slot end\n";
    slot *newEndSlot = new slot;

    newEndSlot->next = nullptr;
    newEndSlot->prev = tail;

    CharStack newEndStack;
    for (int i = 0; i < num; i++)
    { newEndStack.push(plyChr); }

    newEndSlot->slotStack = newEndStack;

    // If the board is empty, set the head pointer to the new slot
   if (head == nullptr)
   {
      head = newEndSlot;
   } else { // Otherwise, set the next pointer of the tail slot to the new slot
      tail->next = newEndSlot;
   }

   // Set the tail pointer to the new slot
   tail = newEndSlot;
}

void Board::createEmptySlotEnd() // done?
{
   // Create a new slot
   slot *newSlot = new slot;

   // Set the next and prev pointers of the new slot
   newSlot->next = nullptr;
   newSlot->prev = tail;

   // Set the slot stack of the new slot to be empty
   newSlot->slotStack = CharStack();

   // If the board is empty, set the head pointer to the new slot
   if (head == nullptr) {
      head = newSlot;
   } else { // Otherwise, set the next pointer of the tail slot to the new slot
      tail->next = newSlot;
   }

   // Set the tail pointer to the new slot
   tail = newSlot;
}

void Board::clearBoard()
{
    cout << "\nclear board\n";   
}