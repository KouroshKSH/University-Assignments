#include <iostream>
#include "Board.h"
#include "CharStack.h"

using namespace std;

Board::Board()
{
    head = NULL; tail = NULL;
    xCnt = 0; oCnt = 0;
}

bool Board::noMove(const char plyChr, const int steps)
{ // returns false even if a single possible move is found, otherwise true
    slot* currentSlot = head;
    int currentIdx = 0;

    while (currentSlot != nullptr)
    {
        char tempChar = '\0';
        if (currentSlot->slotStack.isEmpty() == false)
        { // to avoid shallow copy whilst not modifying the current slot
            currentSlot->slotStack.pop(tempChar);
            currentSlot->slotStack.push(tempChar);
        }
        
        // if (plyChr == tempChar && currentIdx > 0)
        if (plyChr == tempChar)
        {
            // checking possibilities on the left-hand side
            slot* possibleSlot = currentSlot;
            //if (possibleSlot->prev != NULL)
            if (possibleSlot != head)
            {
                // check left side
                for (int i = 0; possibleSlot != nullptr && i < steps; i++)
                {
                    possibleSlot = possibleSlot->prev;
                } // prev points to left columns
                if (possibleSlot != nullptr)
                {
                    char topChr = '\0';
                    if (possibleSlot->slotStack.isEmpty() == false)
                    {
                        possibleSlot->slotStack.pop(topChr);
                        possibleSlot->slotStack.push(topChr);
                    }
                    if (topChr == '\0' || topChr == plyChr)
                    { // at least one valid move has been found
                        return false;
                    }
                }
            }
            // checking possibilities on the right-hand side
            possibleSlot = currentSlot;
            for (int i = 0; possibleSlot != nullptr && i < steps; i++)
            {
                possibleSlot = possibleSlot->next; // next points to right columns
            } 
            if (possibleSlot != nullptr)
            {
                char topChr = '\0';
                if (possibleSlot->slotStack.isEmpty() == false)
                {
                    possibleSlot->slotStack.pop(topChr);
                    possibleSlot->slotStack.push(topChr);
                }
                if (topChr == '\0' || topChr == plyChr)
                { // at least one valid move has been found
                    if (possibleSlot->slotStack.isFull() == false)
                    { return false; } // the piece can be placed since there's space left
                }
            } 
        }
        currentSlot = currentSlot->next; // move on to the rest of the columns
        currentIdx += 1;
    }
    return true; // by default, there are no possible moves
}

int Board::validMove(const char plyChr, const int startIdx, const int steps, const int direction)
{
    // Find the size of the board
    int boardSize = 0;
    slot* currSlot = head;
    while (currSlot != nullptr)
    {
        boardSize += 1;
        currSlot = currSlot->next;
    }
    
    if (startIdx < 0 || startIdx >= boardSize) 
    {
        return 1; // start index out of bounds error code
    } 
    
    // Find the slot at the start index
    slot* startSlot = head;
    for (int i = 0; i < startIdx; i++)
    { startSlot = startSlot->next; }
    
    // does the start slot belongs to the player?
    char topChar = ' ';
    startSlot->slotStack.pop(topChar);
    startSlot->slotStack.push(topChar);
    if (topChar != plyChr) 
    {
        return 4; // start slot does not belong to player
    } 

    // based on the direction, target can be calculated
    int targetIdx;
    if (direction == 0) // going left
    {
        targetIdx = startIdx - steps;
    }
    else if (direction == 1) // going right
    {
        targetIdx = startIdx + steps;
    }
    
    if (targetIdx < 0 || targetIdx >= boardSize) 
    {
        return 2;  // target index is out of bounds
    }

    slot* targetSlot = head;
    for (int i = 0; i < targetIdx; i++)
    {
        targetSlot = targetSlot->next; // the column of our target based on index
    }
    
    // Check if target slot is empty or belongs to the player
    char checkChar = ' ';
    
    if (targetSlot->slotStack.isEmpty() == true)
    {
        return 0;  // the column is empty and is available for placement
    }
    else if (targetSlot->slotStack.isEmpty() == false)
    {
        targetSlot->slotStack.pop(checkChar);
        targetSlot->slotStack.push(checkChar);
        if (checkChar != plyChr)
        {
            return 3; // target index is not empty and does not belong to the player
        }
    }
    return 0; // it's a valid move by default if it has gone through all the checks
}

void Board::movePiece(int originIdx, int targetIdx)
{
    slot *originSlot = head, *targetSlot = head;
    int currentIdx = 0;

    while (originSlot != NULL && currentIdx < originIdx)
    { // locate the column that is the source of the move
        originSlot = originSlot->next;
        currentIdx += 1;
    }

    currentIdx = 0; // reset the index for traversal from beginning

    while (targetSlot != NULL && currentIdx < targetIdx)
    {
        targetSlot = targetSlot->next;
        currentIdx += 1;
    }

    if (originSlot != nullptr && targetSlot != nullptr)
    { // both columns exist on the board
        char currentChar = ' ';
        if (originSlot->slotStack.pop(currentChar))
        { // move the player character from the original to the target column via the modified character
            targetSlot->slotStack.push(currentChar);
        }
    }
}

void Board::printBoard()
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
    int result = -1;
    if (xCnt < oCnt)
    { result = 1; }
    if (xCnt > oCnt)
    { result = 2; }
    if (xCnt == oCnt)
    { result = 3; }
    return result;
}

bool Board::targetSlotFull(int targetInd)
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

void Board::destroySlot(int targetInd)
{
    slot *targetSlot = head;
    int colIdx = 0;

    // find the target slot
    while (targetSlot != NULL && colIdx < targetInd)
    {
        targetSlot = targetSlot->next;
        colIdx += 1;
    }

    // update the piece counts accordingly
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

void Board::createSlotBegin(char plyChr, int num)
{ // create a new slot with player's character at the beginning/left of the board
    slot *newEndSlot = new slot;
    newEndSlot->next = nullptr;
    newEndSlot->prev = tail;
    CharStack newEndStack;

    for (int i = 0; i < num; i++)
    { newEndStack.push(plyChr); }
    newEndSlot->slotStack = newEndStack; // update the stack of the new final column

    if (head == NULL) // the head pointer is the new slot when board is empty
    { head = newEndSlot; }
    else
    { newEndSlot->next = head; } // set the next pointer of the tail slot to the new slot
    head = newEndSlot; // update the head as the new column

    if (plyChr == 'x')
    { xCnt += num; }
    else if (plyChr == 'o')
    { oCnt += num; }
}

void Board::createSlotEnd(char plyChr, int num)
{ // create a new slot with player's character at the end/right of the board
    slot *newEndSlot = new slot;

    newEndSlot->next = nullptr;
    newEndSlot->prev = tail;

    CharStack newEndStack;
    for (int i = 0; i < num; i++)
    { newEndStack.push(plyChr); }

    newEndSlot->slotStack = newEndStack;

    if (head == NULL) // the head pointer to the new slot when board is empty
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

void Board::createEmptySlotEnd()
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

void Board::clearBoard()
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
