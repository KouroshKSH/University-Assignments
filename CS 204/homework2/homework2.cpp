#include <iostream>
#include <string>
#include "feedback.h"

using namespace std;

struct node
{
	int digit;
	node * next;
	node () {} // default
	node (int num, node * ptr) // constructor
		: digit(num), next(ptr) {}
};
typedef node* nodePtr;

bool funcCheckGuessDigits(const string & guess);
bool funcCheckDistinct(const string & guess);
void funcPrintLinkedList(nodePtr header);
nodePtr funcInsertOrderly(nodePtr & head, int newkey);
void funcDeleteOneNode(int value, nodePtr & head);
void funcDeleteLinkedList(nodePtr & head);
bool funcSearchNode(nodePtr head, int key);
void funcDeleteRest(nodePtr & head, int key);
void funcFillListExceptNum(int key, nodePtr & head);
void funcDeleteDuplicates(nodePtr & head);
int funcGetListSize(nodePtr head);
string funcReturnSecretNum(nodePtr list1, nodePtr list2, nodePtr list3);
int funcFindMissingDigit(nodePtr head);

int main()
{
	int gameID;
	cout << "Please enter a game ID.\n"; cin >> gameID;
	// Linked lists for possibles
	nodePtr linklist1 = nullptr, linklist2 = nullptr, linklist3 = nullptr;
	// Linked lists for impossibles
	nodePtr linklist4 = nullptr, linklist5 = nullptr, linklist6 = nullptr;
	bool flagCondition = true;
	while (flagCondition == true)
	{
		string userGuess;
		cout << "Enter your guess.\n"; cin >> userGuess;
		if (userGuess.length() != 3 || funcCheckGuessDigits(userGuess) == false || funcCheckDistinct(userGuess) == false)
		{ // Guess should be a 3 digit-long integer number
			cout << "Invalid guess. ";
		} else {
			string feedback = get_the_feedback(userGuess, gameID);
			for (int i = 0; i < feedback.length(); i++)
			{ // iterate over feedback for checking each letter
				int current_num = userGuess[i] - '0'; // store the int value of user's guess
				if (feedback[i] == 'G')
				{
					if (i == 0)
					{ // first possible gets it, second and third impossible also get it
						linklist1 = funcInsertOrderly(linklist1, current_num);
						linklist5 = funcInsertOrderly(linklist5, current_num);
						linklist6 = funcInsertOrderly(linklist6, current_num);
						// Remove the specfic number from the possibles of second and third
						if (funcSearchNode(linklist2, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist2); }
						if (funcSearchNode(linklist3, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist3); }
						funcDeleteRest(linklist1, current_num); // the first slot can have only one possible digit
						// edit the impossible list of slot 1
						funcFillListExceptNum(current_num, linklist4);
						funcDeleteOneNode(current_num, linklist4);
					}
					if (i == 1)
					{ // same applies to the rest
						linklist2 = funcInsertOrderly(linklist2, current_num);
						linklist4 = funcInsertOrderly(linklist4, current_num);
						linklist6 = funcInsertOrderly(linklist6, current_num);
						if (funcSearchNode(linklist1, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist1); }
						if (funcSearchNode(linklist3, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist3); }
						funcDeleteRest(linklist2, current_num);
						funcFillListExceptNum(current_num, linklist5);
						funcDeleteOneNode(current_num, linklist5);
					}
					if (i == 2)
					{
						linklist3 = funcInsertOrderly(linklist3, current_num);
						linklist5 = funcInsertOrderly(linklist5, current_num);
						linklist4 = funcInsertOrderly(linklist4, current_num);
						if (funcSearchNode(linklist2, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist2); }
						if (funcSearchNode(linklist1, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist1); }
						funcDeleteRest(linklist3, current_num);
						funcFillListExceptNum(current_num, linklist6);
						funcDeleteOneNode(current_num, linklist6);
					}
				}
				if (feedback[i] == 'R')
				{ // that digit does not exist at all and should be added to all impossibles
					linklist4 = funcInsertOrderly(linklist4, current_num);
					linklist5 = funcInsertOrderly(linklist5, current_num);
					linklist6 = funcInsertOrderly(linklist6, current_num); 
				}
				if (feedback[i] == 'Y')
				{ // the corresponding digit exists secret number, but not in that slot
					if (i == 0)
					{
						if (funcSearchNode(linklist1, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist1); } // remove from first slot's possible list
						linklist4 = funcInsertOrderly(linklist4, current_num); // add the number to first slot's impossible list
						if (funcSearchNode(linklist5, current_num) == false)
						// add the number to the possibles of second and third slot
						{ linklist2 = funcInsertOrderly(linklist2, current_num); }
						if (funcSearchNode(linklist6, current_num) == false)
						{ linklist3 = funcInsertOrderly(linklist3, current_num); }
					}
					if (i == 1)
					{ // same for the rest
						if (funcSearchNode(linklist2, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist2); }
						linklist5 = funcInsertOrderly(linklist5, current_num);
						if (funcSearchNode(linklist4, current_num) == false)
						{ linklist1 = funcInsertOrderly(linklist1, current_num); }
						if (funcSearchNode(linklist6, current_num) == false)
						{ linklist3 = funcInsertOrderly(linklist3, current_num); }
					}
					if (i == 2)
					{
						if (funcSearchNode(linklist3, current_num) == true)
						{ funcDeleteOneNode(current_num, linklist3); }
						linklist6 = funcInsertOrderly(linklist6, current_num);
						if (funcSearchNode(linklist5, current_num) == false)
						{ linklist2 = funcInsertOrderly(linklist2, current_num); }
						if (funcSearchNode(linklist4, current_num) == false)
						{ linklist1 = funcInsertOrderly(linklist1, current_num); }
					}	
				}
			}
			// delete the duplicate numbers from all lists 
			funcDeleteDuplicates(linklist1);
			funcDeleteDuplicates(linklist2);
			funcDeleteDuplicates(linklist3);
			funcDeleteDuplicates(linklist4);
			funcDeleteDuplicates(linklist5);
			funcDeleteDuplicates(linklist6);
			// printing process for all 3 slots or 6 linked lists
			cout << "Linked lists:\n";
			cout << "Slot: 1\n";
			cout << "Impossibles: "; funcPrintLinkedList(linklist4);
			cout << "Possibles: "; funcPrintLinkedList(linklist1);
			cout << "\nSlot: 2\n";
			cout << "Impossibles: "; funcPrintLinkedList(linklist5);
			cout << "Possibles: "; funcPrintLinkedList(linklist2);
			cout << "\nSlot: 3\n";
			cout << "Impossibles: "; funcPrintLinkedList(linklist6);
			cout << "Possibles: "; funcPrintLinkedList(linklist3);
			cout << endl; // needed for correct styling			
			if (funcGetListSize(linklist4) == 9 && funcGetListSize(linklist5) == 9 && funcGetListSize(linklist6) == 9)
			{ // the game is over when all 3 impossible lists have 9 digits in them
				string secretNumResult = funcReturnSecretNum(linklist4, linklist5, linklist6);
				cout << "The secret number is: " << secretNumResult << endl;
				flagCondition = false; // end the main while loop
			}
		}
	}
	cout << "Congrats! Now, deleting the lists...\n";
	funcDeleteLinkedList(linklist1);
	funcDeleteLinkedList(linklist2);
	funcDeleteLinkedList(linklist3);
	funcDeleteLinkedList(linklist4);
	funcDeleteLinkedList(linklist5);
	funcDeleteLinkedList(linklist6);
	return 0;
}

int funcFindMissingDigit(nodePtr head)
{ // find the only missing number from a linked lis
    for (int i = 0; i < 10; i++)
	{
        bool found = false;
        // Check if the current digit is in the linked list
        nodePtr current = head;
        while (current != nullptr)
		{
            if (current->digit == i)
			{
                found = true;
                break;
            }
            current = current->next;
        }

        // return the current number if it isn't found
        if (!found)
		{ return i; }
    }
	// this will never return -1 because it will always return something between 0-9
    return -1;
}

string funcReturnSecretNum(nodePtr list1, nodePtr list2, nodePtr list3)
{ // create the secret number out of the impossible lists
	// Find the missing digit in each linked list
    int miss1 = funcFindMissingDigit(list1);
    int miss2 = funcFindMissingDigit(list2);
    int miss3 = funcFindMissingDigit(list3);
	// the secret number is the missing digits of all impossible lists combined together
	string result = to_string(miss1) + to_string(miss2) + to_string(miss3);
	return result;
}

int funcGetListSize(nodePtr head)
{ // calculate the size of a linked list
    int count = 0;
    nodePtr current = head;
    while (current != NULL)
	{
        count += 1;
        current = current->next;
    }
    return count;
}

void funcDeleteDuplicates(nodePtr & head)
{ // Delete the duplicate nodes in the linked list
    nodePtr current = head;
    while (current != NULL)
	{
        nodePtr temp = current;
        while (temp != NULL && temp->digit == current->digit)
		{ temp = temp->next; } // node has been seen before
        current->next = temp;
        current = current->next; // remove node
    }
}

void funcFillListExceptNum(int key, nodePtr & head)
{ // fill the given linked lists with digits 0-9 except one number
	// Create a new node with the first digit and make it the head
    head = new node(0, nullptr);
    nodePtr current = head;
    for (int i = 1; i < 10; i++) {
        // skip the specific digit as the exception
        if (i == key)
		{ continue; }
        nodePtr newNode = new node(i, nullptr); // create a new node with the current digit
        // insert the new node
        while (current->next != nullptr && current->next->digit < i)
		{ current = current->next; }
        newNode->next = current->next;
        current->next = newNode;
        // Reset the current node to the head for the next iteration
        current = head;
    }
}

void funcDeleteRest(nodePtr & head, int key)
{ // delete the content of a linked list other than a specific number
	nodePtr current = head, prev = NULL;
    while (current != NULL)
	{ // iterate until reaching the tail
        if (current->digit != key)
		{
            if (prev == NULL)
			{ // if the node to be deleted is the first node
                head = current->next;
                delete current;
                current = head;
            } else { // if the node to be deleted is not the first node
                prev->next = current->next;
                delete current;
                current = prev->next;
            }
        } else { // if the current node's digit is equal to the key
            prev = current;
            current = current->next;
        }
    }
}

bool funcSearchNode(nodePtr head, int key)
{ // search for a specific number in the linked list
	nodePtr current = head;
    while (current != NULL)
	{
        if (current->digit == key)
		{ return true; }
        current = current->next;
    }
    return false;
}

void funcDeleteLinkedList(nodePtr & head)
{ // to delete all nodes of the linked list
    nodePtr current = head, next;
    while (current != NULL)
	{
        next = current->next; delete current; current = next;
    }
}

void funcDeleteOneNode(int value, nodePtr & head)
{ // to delete a single node from the linked list
	nodePtr needDelete = nullptr, prev = nullptr;
    // search for the node with the given value
    for (nodePtr current = head; current != nullptr; current = current->next)
	{
        if (current->digit == value)
		{
            needDelete = current;
            break;
        }
        prev = current;
    }

    // delete node if found
    if (needDelete != nullptr)
	{
        if (needDelete == head)
		{ head = head->next; }
        else { prev->next = needDelete->next; }
        delete needDelete;
    }
}

nodePtr funcInsertOrderly(nodePtr & head, int newkey)
{ // insert the specific number in a linked list while maintaining ascending order
    nodePtr  ptr = head, prev = NULL;
    if (head == NULL || newkey <= head->digit) // it's the head
    { return new node(newkey, head); }

    while (ptr != NULL && ptr->digit < newkey)
    { // the number can't be placed before something lesser
        prev = ptr;     
        ptr = ptr->next;
    } 
    if (ptr != NULL && ptr->digit == newkey)
	{ // check if newkey is already present in the list
        return head; // return the current head when key is present
    }
    // insert node with newkey
    nodePtr newNode = new node(newkey, ptr);
    prev->next = newNode;
    return head;
}


void funcPrintLinkedList(nodePtr header)
{ // to print the numbers of a linked list with spaces inbetween
    nodePtr current = header;
    while (current != NULL)
	{
        cout << current->digit << " ";
        current = current->next;
    }
    cout << endl;
}

bool funcCheckDistinct(const string & guess)
{ // to check whether all digits are distinct or not
	if (funcCheckGuessDigits(guess) == true)
	{ // after passing the check for all characters being digits...
		if (guess[0] != guess[1] && guess[1] != guess[2] && guess[2] != guess[0]) // all digits are different
		{ return true; }
	}
	return false;
}

bool funcCheckGuessDigits(const string & guess)
{
	for (int i = 0; i < guess.length(); i++)
	{
		if (guess[i] < '0' || guess[i] > '9') // all characters must be digits 0-9
		{ return false; }
	}
	return true;
}
