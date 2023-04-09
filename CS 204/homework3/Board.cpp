#include <iostream>
#include "Board.h"
#include "CharStack.h"

using namespace std;

/***
    TODO:
    01. [x] Board
    02. [ ] validMove
    03. [ ] movePiece
    04. [x] printBoard
    05. [x] evaluateGame
    06. [x] targetSlotFull
    07. [x] destroySlot
    08. [x] createSlotBegin
    09. [x] createSlotEnd
    10. [x] createEmptySlotEnd
    11. [x] clearBoard
    12. [ ] noMove
***/

Board::Board() // done
{
    head = NULL; 
    tail = NULL;
    xCnt = 0;
    oCnt = 0;
}

bool Board::noMove(char plyChr, int die)
{
    return true;
}

/***
In Homework 3, validMove function, there are some error codes mentioned.
If there are errors in both entered slot and target slot indexes, you have to prioritize entered slot index.
This means first you need to check 1 and 4, and then 2 and 3. 
***/ 
int Board::validMove(char plyChr, int choice, int die, int right)
{
    return 0;
}

void Board::movePiece(int choice, int targetInd)
{
    cout << "\nmove piece\n";
}

void Board::printBoard() // done
{
    //cout << "\nprint board\n";
    cout << endl; // for styling purposes
    string topRow = "", midRow = "", endRow = "", baseCaret = "";
    // develop each row by poping the items of the stacks
    int colNum = 0;
    slot * tempSlot = head;
    while (tempSlot != NULL)
    {
        CharStack tempStack = tempSlot->slotStack;
        colNum += 1;
        char tempChar = ' ';
        if (tempStack.isEmpty() == false)
        { tempStack.pop(tempChar); }
        endRow += tempChar;
        tempChar = ' '; // reset the temp char
        if (tempStack.isEmpty() == false)
        { tempStack.pop(tempChar); }
        midRow += tempChar;
        tempChar = ' ';
        if (tempStack.isEmpty() == false)
        { tempStack.pop(tempChar); }
        topRow += tempChar;
        baseCaret += '^';
        tempSlot = tempSlot->next;
    }
    // used 3 rows since max height is 4
    cout << topRow << endl << midRow << endl << endRow << endl << baseCaret << endl;
}

int Board::evaluateGame()
{
    int result = -1;
    if (xCnt < oCnt)
    { result = 1; }
    if (xCnt > oCnt)
    { result = 2; }
    if (xCnt == oCnt)
    { result = 3; }
    return result;
}

bool Board::targetSlotFull(int targetInd) // done
{
    int colIdx = 0;
    slot * targetSlot = head;
    while (targetSlot != NULL && colIdx < targetInd)
    {
        targetSlot = targetSlot->next;
        colIdx += 1;
    }
    bool result = targetSlot->slotStack.isFull();
    return result;
}

void Board::destroySlot(int targetInd) // done?
{
    // cout << "\ndestroy slot\n";
    slot *targetSlot = head;
    int colIdx = 0;

    // find the target slot
    while (targetSlot != NULL && colIdx < targetInd)
    {
        targetSlot = targetSlot->next;
        colIdx += 1;
    }

    // update the piece counts
    CharStack targetStack = targetSlot->slotStack;
    while (targetStack.isEmpty() == false)
    {
        char piece;
        targetStack.pop(piece);
        // update the counter
        if (piece == 'x')
        { xCnt -= 1; }
        else if (piece == 'o')
        { oCnt -= 1; }
    }

    if (targetSlot == head)
    { // the board gets a shift towards left by removing the head
        head = targetSlot->next;
        if (head != NULL)
        { head->prev = NULL; }
    }
    else if (targetSlot == tail)
    { // the board gets a shift towards right by removing the tail
        tail = targetSlot->prev;
        tail->next = NULL;
    }
    else
    { // the target column in the middle disappears by not having anything pointed to it
        targetSlot->prev->next = targetSlot->next;
        targetSlot->next->prev = targetSlot->prev;
    }

    // free memory
    delete targetSlot;
}

void Board::createSlotBegin(char plyChr, int num) // done
{
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
      newEndSlot->next = head;
   }

   // Set the tail pointer to the new slot
   head = newEndSlot;

   if (plyChr == 'x')
   {
       xCnt += num;
       //cout << "\nx count is: " << xCnt << endl;
   } else if (plyChr == 'o')
   {
       oCnt += num;
       //cout << "\no count is: " << oCnt << endl;
   }
}

void Board::createSlotEnd(char plyChr, int num) // done
{
    //cout << "\ncreate slot end\n";
    slot *newEndSlot = new slot;

    newEndSlot->next = nullptr;
    newEndSlot->prev = tail;

    CharStack newEndStack;
    for (int i = 0; i < num; i++)
    { newEndStack.push(plyChr); }

    newEndSlot->slotStack = newEndStack;

    if (head == nullptr) // the head pointer to the new slot when board is empty
    { head = newEndSlot; }
    else
    { tail->next = newEndSlot; }
    tail = newEndSlot;

    if (plyChr == 'x')
    { xCnt += num; }
    else if (plyChr == 'o')
    { oCnt += num; }
}

void Board::createEmptySlotEnd() // done
{
   slot *newSlot = new slot;
   newSlot->next = nullptr;
   newSlot->prev = tail;
   newSlot->slotStack = new CharStack();

   // the head pointer is the new slot if board is empty
   if (head == nullptr)
   {
      head = newSlot;
   } else {
      tail->next = newSlot;
   }

   tail = newSlot;
}

void Board::clearBoard() // done
{
    //cout << "\nclear board\n"; 
    slot *currentSlot = head;
    char tempChar = ' ';
    while (currentSlot != NULL)
    { // traverse all slot from left to right to delete their nodes separately
        while (currentSlot->slotStack.isEmpty() == false)
        { currentSlot->slotStack.pop(tempChar); } // deletes from top node to bottom until there is none left
        currentSlot = currentSlot->next;
    }
    xCnt = 0; oCnt = 0; // there are no more X & O's left
}