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
    12. [x] noMove
***/

Board::Board() // done
{
    head = NULL; tail = NULL;
    xCnt = 0; oCnt = 0;
}

bool Board::noMove(char plyChr, int die) // done?
{ // returns false even if a single possible move is found, otherwise true
    slot * currentSlot = head;
    int currentIdx = 0;

    while (currentSlot != NULL)
    {
        char tempChar = ' ';
        if (currentSlot->slotStack.isEmpty() == false)
        { // to avoid shallow copy whilst not modifying the current slot
            currentSlot->slotStack.pop(tempChar);
            currentSlot->slotStack.push(tempChar);
        }

        if (plyChr == tempChar)
        {
            // checking possibilities on the right-hand side
            slot *possibleSlot = currentSlot;
            int possibleIdx = currentIdx + die; // go forward with the steps
            for (int i = 0; possibleSlot != nullptr && i < die; i++)
            { possibleSlot = possibleSlot->next; } // next points to right columns
            if (possibleSlot != nullptr)
            {
                char topChr = ' ';
                if (possibleSlot->slotStack.isEmpty() == false)
                {
                    possibleSlot->slotStack.pop(topChr);
                    possibleSlot->slotStack.push(topChr);
                }
                if (topChr == ' ' || topChr == plyChr)
                { // at least one valid move has been found
                    return false;
                }
            }

            // checking possibilities on the left-hand side
            possibleSlot = currentSlot;
            possibleIdx = currentIdx - die; // go backwards with the steps
            for (int i = 0; possibleSlot != nullptr && i < die; i++)
            { possibleSlot = possibleSlot->prev; } // prev points to left columns
            if (possibleSlot != nullptr)
            {
                char topChr = ' ';
                if (possibleSlot->slotStack.isEmpty() == false)
                {
                    possibleSlot->slotStack.pop(topChr);
                    possibleSlot->slotStack.push(topChr);
                }
                if (topChr == ' ' || topChr == plyChr)
                { // at least one valid move has been found
                    return false;
                }
            }
        }
        currentSlot = currentSlot->next;
        currentIdx += 1;
    }
    return true; // by default, there are no possible moves
}

/***
In Homework 3, validMove function, there are some error codes mentioned.
If there are errors in both entered slot and target slot indexes, you have to prioritize entered slot index.
This means first you need to check 1 and 4, and then 2 and 3. 
***/ 
int Board::validMove(char plyChr, int choice, int die, int direction)
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

int Board::evaluateGame() const
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

bool Board::targetSlotFull(int targetInd) // done?
{
    int colIdx = 0, size = 0;
    slot * targetSlot = head;
    while (targetSlot != nullptr && colIdx < targetInd)
    { // find the intended column
        targetSlot = targetSlot->next;
        colIdx++;
    }
    CharStack tempStack = targetSlot->slotStack;
    char tempChar = ' ';
    while (tempStack.isEmpty() == false)
    { // find the size of the stack 
        tempStack.pop(tempChar);
        size++;
    }
    return (size == 4);
}

void Board::destroySlot(int targetInd) // done
{
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
{ // create a new slot with player's character at the beginning/left of the board
    slot *newEndSlot = new slot;
    newEndSlot->next = nullptr;
    newEndSlot->prev = tail;
    CharStack newEndStack;

    for (int i = 0; i < num; i++)
    { newEndStack.push(plyChr); }

    newEndSlot->slotStack = newEndStack;


    if (head == nullptr) // the head pointer is the new slot when board is empty
    { head = newEndSlot; }
    else // else, set the next pointer of the tail slot to the new slot
    { newEndSlot->next = head; }
    head = newEndSlot; // update the head as the new column

    if (plyChr == 'x')
    { xCnt += num; }
    else if (plyChr == 'o')
    { oCnt += num; }
}

void Board::createSlotEnd(char plyChr, int num) // done
{ // create a new slot with player's character at the end/right of the board
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
    tail = newEndSlot; // update the tail as the new column

    // update the number of characters
    if (plyChr == 'x')
    { xCnt += num; }
    else if (plyChr == 'o')
    { oCnt += num; }
}

void Board::createEmptySlotEnd() // done
{ // // create an empty slot at the end/right of the board
   slot *newSlot = new slot;
   newSlot->next = nullptr;
   newSlot->prev = tail;
   newSlot->slotStack = CharStack();

   // the head pointer is the new slot if board is empty
   if (head == nullptr)
   { head = newSlot; }
   else
   { tail->next = newSlot; }
   tail = newSlot; // update tail as the new column
}

void Board::clearBoard() // done
{
    slot *currentSlot = head;
    char tempChar = ' ';
    while (currentSlot != NULL)
    { // traverse all slot from left to right to delete their nodes separately
        while (currentSlot->slotStack.isEmpty() == false)
        { currentSlot->slotStack.pop(tempChar); } // deletes from top node to bottom until there is none left
        currentSlot = currentSlot->next;
    }
    delete head; delete tail;
    head = NULL; tail = NULL; // to avoid possible memory issues
    xCnt = 0; oCnt = 0; // there are no more X & O's left
}
