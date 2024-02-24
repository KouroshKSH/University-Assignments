/*
    Info: This is the header file for the Hash Table used in the main program.
*/

#ifndef HASH_H
#define HASH_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <list>
#include <string>
#include <vector>

// prototype
template <class T>
class HashTable;

bool funcIsPrime(const int& num);
int funcNextPrime(int num);

// a struct that represents the number of repetitions of the word per file
struct docItem {
    string docName;
    int count;

    // default constructor
    docItem(const string& name, int num) : docName(name), count(num) {}
};

// the Entry class that represents each Entry in the hash table
template <class T>
class Entry {
private:
    T element; // the word representing the Entry
    vector<docItem> info; // the information of each Entry (name and repetitions)

public:
    // constructor for Entry
    Entry(const T& data, const docItem& doc) : element(data) {
            info.push_back(doc);
    }

	// to get the element
	const T& getElement() const {return element;}

	// to get the document info
	const vector<docItem>& getInfo() const {return info;}

	// to get the specific count of a specific docName
    int getSpecificDocCount(const string& thisFile) const {
        for (const auto& doc : info) {
            if (doc.docName == thisFile) {
                return doc.count;
            }
        }
        return -1; // the document is not found
    }

    friend class HashTable<T>; // for accessing private data members
};

const double THRESHOLD = 0.80;

template <class T>
class HashTable {
private:
	// private members
	vector<list<Entry<T>>> buckets; // the list of items per each bucket
	const Entry<T> ITEM_NOT_FOUND; // the typical not found item of type Entry
	int numOfElements; // total number of elements stored in the table

public: 
	// constructors and destructor
	HashTable(const T& notFound, int size=17);
	HashTable(const T& rhs);
	~HashTable();
	
	// useful functions
	double getLoadFactor() const;
	void funcResizeTable();
	const Entry<T>& funcFind(const T& obj) const;
	void funcMakeEmpty();
	void funcInsert(const T& obj, const string& fileName);
	void funcRemove(const T& obj);
	int getUniqueWordCount() const;
	void funcPrintTable() const;
	int funcFetchDocCount(const string& word, const string& fileName) const;
	void funcCopyEntriesToVector(vector<Entry<T>>& unsortedVector) const;
	const HashTable& operator=(const HashTable& rhs);
};

// Fowler–Noll–Vo hashing function
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
int FowlerNollVoHashFunction(const string & key, int tableSize) {
	int hashValue = 0;
	for (char c : key) {
		hashValue = 37 * hashValue + c;
	}

	hashValue = hashValue % tableSize;

	// very important!!! otherwise, you get seg faults
	if (hashValue < 0) {hashValue += tableSize;}

	return (hashValue);
}

// Murmur hashing function
// https://en.wikipedia.org/wiki/MurmurHash
uint32_t MurmurHashFunction(const void* key, int len, uint32_t seed = 0) {
	// This hashing function, known as the FNV-1a hash, iterates through each character of the input string, 
	// multiplying the hash by a prime and XORing it with the character's value, 
	// aiming to produce a well-distributed hash value by incorporating both multiplication and XOR operations.
    
	// the chosen values for 'm' and 'r' just happen to work well together
	const uint32_t m = 0x5bd1e995; // constant 'm' used in the MurmurHash3 algorithm
    const int r = 24; // number of bits to rotate the final hash result
    const uint8_t* data = reinterpret_cast<const uint8_t*>(key); // cast the 'key' to a byte array for processing
    uint32_t h = seed ^ len; // initialize the hash 'h' with the seed XORed with the length of the key
    const uint8_t* end = data + len; // point 'end' to the end of the data array

    while (data + 4 <= end) {
        uint32_t k;
        memcpy(&k, data, sizeof(uint32_t));
        data += 4;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;
    }

    switch (end - data) {
        case 3:
            h ^= data[2] << 16;
            [[fallthrough]];
        case 2:
            h ^= data[1] << 8;
            [[fallthrough]];
        case 1:
            h ^= data[0];
            h *= m;
            break;
        default:
            break;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

// a hash function when the string element of the entry is passed
int hashFunction(const string& key, int tableSize) {
    uint32_t hashValue = MurmurHashFunction(key.c_str(), key.length());
    return static_cast<int>(hashValue % tableSize);
}

// a hash function for when an Entry is being passed and its `element` needs to be hashed
template <class T>
int hashFunction(const Entry<T>& entry, int tableSize) {
    return hashFunction(entry.element, tableSize);
}

// constructor for hash table
template <class T>
HashTable<T>::HashTable(const T& notFound, int size) : ITEM_NOT_FOUND(notFound, docItem("", 0)), numOfElements(0) {
	int newSize = funcNextPrime(size);
	// https://cplusplus.com/reference/vector/vector/resize/
	buckets.resize(newSize); // `.resize()` adjusts the size of the vector (introduced with C++98)
}

// copy constructor for hash table
template <class T>
HashTable<T>::HashTable(const T& rhs) :
			ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
			buckets(rhs.buckets),
			numOfElements(rhs.numOfElements) {}

// destructor
template <class T>
HashTable<T>::~HashTable() {
	funcMakeEmpty();
}

// this function gives the load factor, which should be precise
template <class T>
double HashTable<T>::getLoadFactor() const {
	return double(numOfElements / double(buckets.size())); // to make sure it returns double
}

// this function resizes the hash table and prints appropriate messages
template <class T>
void HashTable<T>::funcResizeTable() {
	// no need for printing messages for this homework
	// cout << "rehashed...\n";
    // cout << "previous table size:" << buckets.size();

    int newSize = funcNextPrime(buckets.size() * 2);
    // cout << ", new table size:" << newSize;

    int uniqueWords = 0; // the sum of all items in all buckets gives the number of unique words
    for (auto& bucket : buckets) {
		// https://en.cppreference.com/w/cpp/language/range-for
        uniqueWords += bucket.size();
    }

    double currentLoad = double(uniqueWords) / double(newSize);
    // cout << ", current unique word count:" << uniqueWords;
    // cout << ", current load factor:" << currentLoad << "\n";

    // temporarily store the current buckets to avoid memory issues during resizing
    vector<list<Entry<T>>> tempBuckets = move(buckets); // https://en.cppreference.com/w/cpp/utility/move

    buckets.resize(newSize);

    // rehash and reassign items to the new table
    for (auto& bucket : tempBuckets) {
        for (auto& item : bucket) {
            int index = hashFunction(item.element, newSize);
            buckets[index].emplace_back(item); // https://en.cppreference.com/w/cpp/container/vector/emplace_back
        }
    }
}

// this function locates the desired object and returns the Entry
template <class T>
const Entry<T>& HashTable<T>::funcFind(const T& obj) const {
	// locate the index of the object given the hash table
	int index = hashFunction(obj, buckets.size());
	for (auto& item : buckets[index]) {
		// traverse the given list of items
		if (item.element == obj) {
			return item; // found the desired item
		}
	}

	return ITEM_NOT_FOUND;
}

// this function clears all the lists in the hash table
template <class T>
void HashTable<T>::funcMakeEmpty() {
	for (auto& b : buckets) {
		// clear each list in the buckets vector
		// https://cplusplus.com/reference/list/list/clear/
		b.clear(); // the `.clear()` method removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
	}

	numOfElements = 0; // the updated number of elements is zero since all lists have been cleared
}


// this function inserts an object into the hash table
template <class T>
void HashTable<T>::funcInsert(const T& obj, const string& fileName) {
	// there are 3 possible scenarios for insertion:
	// 1. object does not exist, then it should create a new instance for it
	// 2. object does exist, however, this is the first instance of the given file
	// 3. object does exist, and this is the n-th time of encountering it in the same file

	int index = hashFunction(obj, buckets.size()); // find which bucket the object belongs to
	
	bool found = false; // used for cases 2 and 3

	// traverse all the items in the appropriate list
	for (auto iterator = buckets[index].begin(); iterator != buckets[index].end(); ++iterator) {
		auto& item = *iterator;

		if (item.element == obj) {
			for (auto& doc : item.info) {
				if (doc.docName == fileName) {
					// case 3
					doc.count += 1;
					found = true;
					break;
				}
			}
			if (!found) {
				// case 2
				docItem newDocItem(fileName, 1);
				item.info.push_back(newDocItem);
			}

			return; // we're done in the pre-existing cases
		}
	}

	// case 1
	docItem newDocItem(fileName, 1);
	Entry<T> newEntry(obj, newDocItem);
	buckets[index].emplace_back(newEntry); 
	numOfElements++; // because a new item has been added to the table

	// check if resize is needed
	if (getLoadFactor() > THRESHOLD) {
		funcResizeTable();
	}
}

// this function removes the object from the hash table
template <class T>
void HashTable<T>::funcRemove(const T& obj) {
	int index = hashFunction(obj, buckets.size()); 	// find which index the object belongs to
	
	auto& bucket = buckets[index]; // get the content of that list

	// iterate through the items in that bucket
	for (auto iterator = bucket.begin(); iterator != bucket.end(); ++iterator) {
		// https://cplusplus.com/reference/iterator/begin/
		// https://cplusplus.com/reference/iterator/end/
		if (iterator->element == obj) {
			// object has been found
			// https://cplusplus.com/reference/list/list/erase/
			bucket.erase(iterator); // `.erase()` removes one item from the list (introduced in C++98)
			numOfElements -= 1; // deduce by one since an object has been removed from the table
			return;
		}
	}

	// item hasn't been found
	cout << "Can't remove an object that doesn't exist in the table\n";
}

// for printing purposes in the main program
template <class T>
int HashTable<T>::getUniqueWordCount() const {
	return numOfElements; // they're the same
}

// this function prints the content of the table (for internal testing)
template <class T>
void HashTable<T>::funcPrintTable() const {
	for (int i = 0; i < buckets.size(); i++) {
		cout << "- Bucket [" << i << "] -> ";
		for (const auto& item : buckets[i]) {
			cout << "`" << item.element << "`: ";
			for (const auto& doc : item.info) {
				cout << "in `" << doc.docName << "` " << doc.count << " rep(s), ";
			}
			cout << "\n             -> ";
		}
		cout << "\n";
	}
}

// get the number of reps of said `fileName` for that specific `word`
template <class T>
int HashTable<T>::funcFetchDocCount(const string& word, const string& fileName) const {
	int result = 0; // the count we return at the end
	const Entry<T>& foundEntry = funcFind(word); // find the word in the table
	if (&foundEntry != &ITEM_NOT_FOUND) {
		// the word does exist in the table
		vector<docItem> allInfo = foundEntry.getInfo(); // get all the doc info of current entry
		for (const auto& i : allInfo) {
			// traverse all the doc info to find the desired file
			if (i.docName == fileName) {
				result = i.count; // we want the reps of this file
			}
		}
	}

	return result;
}

// operator overload for assignment
template <class T>
const HashTable<T>& HashTable<T>::operator=(const HashTable& rhs) {
	if (this != &rhs) {
		buckets = rhs.buckets;
		numOfElements = rhs.numOfElements;
	}

	return *this;
}

// function for copying the entries of the table to a vector
template <class T>
void HashTable<T>::funcCopyEntriesToVector(vector<Entry<T>>& unsortedVector) const {
    for (const auto& bucket : buckets) {
        for (const auto& entry : bucket) {
            unsortedVector.push_back(entry);
        }
    }
}

// this function determines if a number is prime or not
bool funcIsPrime(const int& num) {
	if (num == 2 || num == 3) {
		return true;
	} else if (num == 1 || num % 2 == 0) {
		return false;
	}

	for (int i = 3; i*i <= num; i += 2) {
		if (num % i == 0) {
			return false;
		}
	}

	return true;
}

// this function returns a prime number that is either the given number,
// or the smallest prime number greater than `num`
int funcNextPrime(int num) {
	// even numbers are clearly not prime
	if (num % 2 == 0) {num++;}
	while (!funcIsPrime(num)) {num += 2;}
	return num;
}

#endif
