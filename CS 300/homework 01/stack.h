/*
    Info: This is the header file for the stack data structure used in the main program.
*/

#ifndef STACK_H
#define STACK_H

#include <iostream>

template <typename T>
struct Node {
    T data;
    Node* next;
    // constructor for Node
    Node(const T& givenData, Node* givenNext = nullptr) :
        data(givenData), next(givenNext) {}
};

template <typename T>
class Stack {
private:
    Node<T>* top; // points to the top of the stack of type T

public:
    // default constructor
    Stack() : top(nullptr) {}

    // copy constructor
    Stack(const Stack& rhs) {
        top = NULL;
        *this = rhs; // deep copy
    }

    // destructor
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // stack operations
    // push the value to the top of the stack
    void push(const T& value) {
        // no need to check for size since there is no limitation
        Node<T> *newNode = new Node<T>(value, top);
        top = newNode;
    }

    // pop the top element without returning anything
    void pop() {
        if (isEmpty()) { // can't pop from an empty stack
            throw std::out_of_range("stack is empty");
        }
        Node<T> *newNode = top;
        top = top->next; // update top
        delete newNode;
    }

    // get the value of the top element without popping
    T& peek() const {
        if (isEmpty()) { // can't peek the top value of an empty stack
            throw std::out_of_range("stack is empty");
        }
        return (top->data);
    }

    // check if the stack is empty or not
    bool isEmpty() const {
        return (top == nullptr);
    }
    
    // make the stack empty (similar to destructor)
    void emptyStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // operator overloading for deep copying
    const Stack& operator=(const Stack& rhs) {
        if (this != &rhs) {  // skip if it is a self-assignment
            emptyStack(); // empty the current stack to have a fresh slate
            if (rhs.isEmpty()) { // the rhs stack itself is empty
                return *this; // an empty stack will be returned
            }

            // assign each element of the right hand side to the current stack
            Node<T>* rightPtr = rhs.top; // get the top element of rhs
            Node<T>* currPtr = new Node<T>(rightPtr->data); // create a current pointer for current stack
            top = currPtr; // assign the current pointer to the original top element
            
            for (rightPtr = rightPtr->next; rightPtr != nullptr; rightPtr = rightPtr->next) {
                // traverse until the end of the rhs stack
                currPtr = currPtr->next = new Node<T>(rightPtr->data);
            }
        }
        return *this;
    }

    // print the content of the stack
    void printStack() const {
        Node<T> *newNode = top;
        while (newNode != nullptr) {
            std::cout << newNode->data << "\n";
            newNode = newNode->next;
        }
    }
};

#endif
