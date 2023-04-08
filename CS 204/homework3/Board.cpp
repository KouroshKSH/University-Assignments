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
    07. [ ] destroySlot
    08. [x] createSlotBegin
    09. [x] createSlotEnd
    10. [x] createEmptySlotEnd
    11. [ ] clearBoard
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
    if (xCnt < oCnt)
    { return 1; }
    else if (xCnt > oCnt)
    { return 2; }
    else if (xCnt == oCnt)
    { return 3; }
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
    return targetSlot->slotStack.isFull();
}

void Board::destroySlot(int targetInd)
{
    cout << "\ndestroy slot\n";
}

void Board::createSlotBegin(char plyChr, int num) // done
{
    //cout << "\ncreate slot begin\n";
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

    // If the board is empty, set the head pointer to the new slot
   if (head == nullptr)
   {
      head = newEndSlot;
   } else { // Otherwise, set the next pointer of the tail slot to the new slot
      tail->next = newEndSlot;
   }

   // Set the tail pointer to the new slot
   tail = newEndSlot;

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

void Board::createEmptySlotEnd() // done
{
   // Create a new slot
   slot *newSlot = new slot;

   // Set the next and prev pointers of the new slot
   newSlot->next = nullptr;
   newSlot->prev = tail;

   // Set the slot stack of the new slot to be empty
   newSlot->slotStack = CharStack();

   // If the board is empty, set the head pointer to the new slot
   if (head == nullptr)
   {
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
