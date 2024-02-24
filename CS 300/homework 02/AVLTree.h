/*
    Info: This is the header file for the AVL Tree used in the main program.
*/

#ifndef AVLTree_H
#define AVLTree_H

#include <iostream>
#include <vector>

using namespace std;

template <class T>
class AVLTree;

// a struct that represents the number of repetitions of the word per file
struct documentItem {
    string docName;
    int count;

    // default constructor
    documentItem(const string& name, int num) : docName(name), count(num) {}
};

// the Node class that represents each node in the tree
template <class T>
class Node {
protected:
    T element; // the word representing the node
    vector<documentItem> info; // the information of each node (name and repetitions)
    Node* left; // pointer to the left subtree
    Node* right; // pointer to the right subtree
    int height; // height of the node

    // constructor for Node
    Node(const T& data,
        const documentItem& doc,
        Node* leftTree,
        Node* rightTree,
        int h = 0) :
        element(data), left(leftTree), right(rightTree), height(h) {
            info.push_back(doc);
        }

    friend class AVLTree<T>; // for accessing private data members
};

template <class T>
class AVLTree {
private:
    // private members
    Node<T>* root;
    T ITEM_NOT_FOUND;

    // private methods
    const T& elementAt(Node<T>* node) const;
    void insert(const T& item, Node<T>*& node, const string& fileName);
    void remove(const T& item, Node<T>*& node);
    Node<T>* findMin(Node<T>* node) const;
    Node<T>* findMax(Node<T>* node) const;
    Node<T>* findItem(const T& item, Node<T>* node) const;
    void makeEmpty(Node<T>*& node) const;
    Node<T>* clone(Node<T>* node) const;
    void funcPrintTree(Node<T>* node) const;
    int getHeight(Node<T>* node) const;
    int max(int num1, int num2) const;
    void funcRotateLeftOnce(Node<T>*& k2) const;
    void funcRotateRightOnce(Node<T>*& k1) const;
    void funcRotateLeftTwice(Node<T>*& k3) const;
    void funcRotateRightTwice(Node<T>*& k1) const;

public:
    // constructors and destructor
    explicit AVLTree(const T& notFound)  : root(NULL), ITEM_NOT_FOUND(notFound) {}
    AVLTree(const AVLTree& rhs);
    ~AVLTree();

    // public methods
    const T& funcFindMin() const;
    const T& funcFindMax() const;
    const T& funcFindItem(const T& item) const;
    bool funcIsEmpty() const;
    void funcPrintTree() const;
    void funcMakeEmpty();
    void funcInsertItem(const T& item, const string& fileName);
    void funcRemoveItem(const T& item);
    vector<documentItem> funcGetDocInfo(const T& item) const;
    int funcGetDocCount(const string& word, const string& file) const;
    const AVLTree& operator=(const AVLTree& rhs); // operator overloading
};


// Private Methods
// internal function to return the maximum element between two
template <class T>
int AVLTree<T>::max(int num1, int num2) const {
    return (num1 > num2) ? num1 : num2;
}

// internal function to return the element of the node (returns ITEM_NOT_FOUND if NULL)
template <class T>
const T& AVLTree<T>::elementAt(Node<T>* node) const {
    return (node != NULL) ? node->element : ITEM_NOT_FOUND;
}

// internal function to get the height of node (-1 if NULL)
template <class T>
int AVLTree<T>::getHeight(Node<T>* node) const {
    return (node != NULL) ? node->height : -1; // a height of -1 means the node doesn't exist
}

// internal function to find the mininum element of the tree
template <class T>
Node<T>* AVLTree<T>::findMin(Node<T>* node) const {
    // the left-most node is the minimum element in the tree
    if (node != NULL) {
        while (node->left != NULL) {
            node = node->left;
        }
    } // ITEM_NOT_FOUND otherwise
    return node;
}

// internal function to find the maximum element of the tree
template <class T>
Node<T>* AVLTree<T>::findMax(Node<T>* node) const {
    // the right-most node is the maximum element in the tree
    if (node != NULL) {
        while (node->right != NULL) {
            node = node->right;
        }
    } // ITEM_NOT_FOUND otherwise
    return node;
}

// internal function to find a specific item in the tree
template <class T>
Node<T>* AVLTree<T>::findItem(const T& item, Node<T>* node) const {
    // try to find `item` in the tree with the given root recursively
    if (node == NULL) {
        return NULL; // an empty tree won't have the item
    }

    if (item < node->element) {
        return findItem(item, node->left); // need to return the value from the recursive call
    } else if (item > node->element) {
        return findItem(item, node->right); // need to return the value from the recursive call
    } else {
        return node; // there's a match
    }
}

// rotations are copied from lecture slides (all 4 cases)
template <class T>
void AVLTree<T>::funcRotateLeftOnce(Node<T>*& k2) const {
    // case 1
    Node<T>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(getHeight(k2->left), getHeight(k2->right)) + 1;
    k1->height = max(getHeight(k1->left), k2->height) + 1;
    k2 = k1;
}

template <class T>
void AVLTree<T>::funcRotateRightOnce(Node<T>*& k1) const {
    // case 2
    Node<T>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(getHeight(k1->left), getHeight(k1->right)) + 1;
    k2->height = max(getHeight(k2->right), k1->height) + 1;
    k1 = k2;
}

template <class T>
void AVLTree<T>::funcRotateLeftTwice(Node<T>*& k3) const {
    // case 3
    funcRotateRightOnce(k3->left);
    funcRotateLeftOnce(k3);
    k3->height = max(getHeight(k3->left), getHeight(k3->right)) + 1; // update the height
}

template <class T>
void AVLTree<T>::funcRotateRightTwice(Node<T>*& k1) const {
    // case 4
    funcRotateLeftOnce(k1->right);
    funcRotateRightOnce(k1);
    k1->height = max(getHeight(k1->left), getHeight(k1->right)) + 1; // update the height
}

// internal function to insert item into a node (depends on node's status)
template <class T>
void AVLTree<T>::insert(const T& item, Node<T>*& node, const string& fileName) {
    // when the tree is empty...
    if (node == NULL) {
        // a new leaf node (no children)
        documentItem newDocItem(fileName, 1);
        node = new Node<T>(item, newDocItem, NULL, NULL);
        return; // a tree with a single node has been made
    }

    // check the content of node with the item
    if (item < node->element) {
        // check the left subtree to insert
        insert(item, node->left, fileName);
    } else if (item > node->element) {
        // check the right subtree to insert
        insert(item, node->right, fileName);
    } else {
        // so item already exists
        // increase the repition of the item for the given file or make a new record
        bool found = false;
        for (int i = 0; i < node->info.size(); i++) {
            documentItem currentDoc = node->info[i];
            if (currentDoc.docName == fileName) {
                // an already-existing file has the word
                // so increase the repetitions of the word for that file
                currentDoc.count += 1;
                node->info[i] = currentDoc;
                found = true;
                break;
            }
        }
        if (found == false) {
            // a new file has the same word 
            // we have to create a new instance for it then
            documentItem newDocItem(fileName, 1);
            node->info.push_back(newDocItem);
        }
        return; // exit after creating a new doc entry
    }

    // update the height of node because of new depths in the tree
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    // perform the necessary rotation based on the balance factor to balance the AVL tree
    int balance_factor = getHeight(node->left) - getHeight(node->right);
    if (balance_factor > 1) { // left-heavy tree
        if (item < node->left->element) {
            funcRotateLeftOnce(node); // case 1
        } else {
            funcRotateLeftTwice(node); // case 3
        }
    } else if (balance_factor < -1) { // right-heavy tree
        if (item > node->right->element) {
            funcRotateRightOnce(node); // case 2
        } else {
            funcRotateRightTwice(node); // case 4
        }
    }
}

// internal function to remove said item from the tree
template <class T>
void AVLTree<T>::remove(const T& item, Node<T>*& node) {
    // Steps:
    // 1. handle an empty tree
    // 2. compare `item` with the `element` of `node`
    // > 2.1. recursively call `remove()` on `left` or `right` of node if not equal
    // > 2.2. if equal:
    // >> 2.2.1. no child case
    // >> 2.2.2. one child case
    // >> 2.2.3. two children case
    // 3. update height and balance factor
    // 4. perform rotations based on balance factor and heights of subtrees (LL, LR, RR, RL)

    if (node == nullptr) {
        // do nothing for an empty tree
        return;
    }

    if (item < node->element) {
        // item to be removed is in the left subtree
        remove(item, node->left);
    } else if (item > node->element) {
        // item to be removed is in the right subtree
        remove(item, node->right);
    } else {
        // found the node that should be removed
        if (node->left == nullptr || node->right == nullptr) {
            // at most one child
            // grab the non-Null child
            Node<T>* temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                // it's a leaf node (no children)
                temp = node;
                node = nullptr;
            } else {
                // one child case
                *node = *temp; // copy the contents of the child
            }
            delete temp;
        } else {
            // two children case
            // find the in-order successor
            Node<T>* successor = findMin(node->right);
            // copy the content of the successor in current node
            node->element = successor->element;
            node->info = successor->info;
            // remove the successor from the right subtree
            remove(successor->element, node->right);
        }
    }

    if (node == nullptr) {
        return; // no need to rebalance if it had one node
    }

    // update height and balance factor
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getHeight(node->left) - getHeight(node->right);

    // rebalance the tree based on the 2 parameters
    if (balance > 1) {
        if (getHeight(node->left->left) >= getHeight(node->left->right)) {
            funcRotateLeftOnce(node);
        } else {
            funcRotateLeftTwice(node);
        }
    } else if (balance < -1) {
        if (getHeight(node->right->right) >= getHeight(node->right->left)) {
            funcRotateRightOnce(node);
        } else {
            funcRotateRightTwice(node);
        }
    }
}


// internal method for clearning the tree
template <class T>
void AVLTree<T>::makeEmpty(Node<T>*& node) const {
    // recursively delete the node
    if (node != NULL) {
        makeEmpty(node->left); // delete the left subtree
        makeEmpty(node->right); // delete the right subtree
        delete node;
    }
    node = NULL; // nullify to make sure
}

// internal function to clone a subtree
template <class T>
Node<T>* AVLTree<T>::clone(Node<T>* node) const {
    if (node == NULL) {
        return NULL;
    } else {
        // recursively clone the left and right subtree of the current node
        Node<T>* newNode = new Node<T>(node->element, {}, clone(node->left), clone(node->right));

        // deep copy the info of the node
        for (const documentItem& item : node->info) {
            documentItem newDocItem(item.docName, item.count);
            newNode->info.push_back(newDocItem);
        }

        // update the height of the new node
        newNode->height = max(getHeight(newNode->left), getHeight(newNode->right)) + 1;
        return newNode;
    }
}

// internal function to print the tree recursively
template <class T>
void AVLTree<T>::funcPrintTree(Node<T>* node) const {
    // this will print from the left-most node all the way to the right-most node (in-order traversal)
    if (node != NULL) {
        funcPrintTree(node->left);
        cout << "\n\t> word:\t" << node->element << endl;
        cout << "\t> info is of size " << node->info.size() << "\n";
        // for internal testing (not used in the main program)
        for (int i = 0; i < node->info.size(); i++) {
            documentItem currentDoc = node->info[i];
            cout << "\t>> file name: " << currentDoc.docName << ", reps: " << currentDoc.count << endl;
        }
        cout << endl;
        funcPrintTree(node->right);
    }
}


// Public Methods
// copy constructor
template<class T>
AVLTree<T>::AVLTree(const AVLTree& rhs) : root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND) {
    root = clone(rhs.root); // deep copy the rhs tree via cloning
}

// operator overloading for `=`
template <class T>
const AVLTree<T>& AVLTree<T>::operator=(const AVLTree& rhs) {
    if (this != &rhs) { // don't change anything if it's a self-assignment
        funcMakeEmpty(); // empty the current tree
        ITEM_NOT_FOUND = rhs.ITEM_NOT_FOUND; // get the rhs's INF value
        root = clone(rhs.root); // clone the root of right hand side into root
    }
    return *this;
}

// destructor for AVLTree
template <class T>
AVLTree<T>::~AVLTree() {
    makeEmpty(root);
    root = NULL;
}

// function for finding the minimum element
template <class T>
const T& AVLTree<T>::funcFindMin() const {
    return elementAt(findMin(root));
}

// function for finding the maximum element
template <class T>
const T& AVLTree<T>::funcFindMax() const {
    return elementAt(findMax(root));
}

// function for finding a specific item
template <class T>
const T& AVLTree<T>::funcFindItem(const T& item) const {
    return elementAt(findItem(item, root));
}

// function for checking if the tree is empty or not
template <class T>
bool AVLTree<T>::funcIsEmpty() const {
    // the tree is empty if the root is NULL
    return (root == NULL);
}

// function for printing the tree
template <class T>
void AVLTree<T>::funcPrintTree() const {
    // use the private print method to recursively print the nodes starting from root
    funcPrintTree(root);
}

// function for clearning a tree
template <class T>
void AVLTree<T>::funcMakeEmpty() {
    makeEmpty(root); // empty the tree starting from `root`
}

// functoin to insert a specific item into the tree
template <class T>
void AVLTree<T>::funcInsertItem(const T& item, const string& fileName) {
    // private insert method to insert `item` in the tree starting from `root` node given the name of file
    insert(item, root, fileName);
}

// function to remove a specific item from the tree
template <class T>
void AVLTree<T>::funcRemoveItem(const T& item) {
    remove(item, root);
}

// function to return the document information of a node given an item as the element
template <class T>
vector<documentItem> AVLTree<T>::funcGetDocInfo(const T& item) const {
    vector<documentItem> docInfo;
    Node<T>* currentNode = findItem(item, root);
    if (currentNode) {
        docInfo = currentNode->info;
    }
    return docInfo;
}


// function to return the total number of times a given word has been seen in a given document
template <class T>
int AVLTree<T>::funcGetDocCount(const string& word, const string& file) const {
    // get the info for the specific word
    int result;
    Node<T>* currentNode = findItem(word, root);
    // check if the word actually exists or not
    if (currentNode) {
        // if so, grab its information
        vector<documentItem> docInfo = this->funcGetDocInfo(word);
        for (const documentItem& doc : docInfo) {
            // find the document that has the same name
            if (doc.docName == file) {
                // the number we're looking for is the count for this document
                result = doc.count;
                break;
            }
        }
    }
    return result;
}


#endif
