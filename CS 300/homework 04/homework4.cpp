/*
    Info: This is the main file for the main program.
	Steps:
		1) Get the number of files.
		2) Read each file.
		3) Make all the words of the given file lowercase.
		4) Separate the words and store them in both BST and Hash Table.
		5) Store the elements of the table into a vector (happens in an unsroted manner).
			-> will be used later for the sorting algorithms
		6) Perform the sorting algorithms on th different instances of the vector of unsorted elements.
		7) Based on the query, display a proper message.
		8) Report the compairson of the performances.
*/

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "BinarySearchTree.h"
#include "Hash.h"

using namespace std;

const string ITEM_NOT_FOUND = "---";

void funcLowerString(string& word);
void funcExtractWords(const string& sentence, vector<string>& words);
int getNumberOfWords(const vector<string>& fileNames);

template <class T>
vector<string> funcFindCommonFiles(const BinarySearchTree<T>& wordTree, const vector<string>& words);

template <class T>
vector<string> funcFindCommonFiles(const HashTable<T>& wordTable, const vector<string>& words);

template <class T>
vector<string> funcFindCommonFiles(const vector<T>& array, const vector<string>& words);

template <class T>
string funcQueryBinarySearchTree(const BinarySearchTree<T>& wordTree, const vector<string>& words);

template <class T>
string funcQueryHashTable(const HashTable<T>& wordTable, const vector<string>& words);

template <class T>
string funcQueryBinarySearchAlgorithm(const vector<T>& entryVector7, const vector<string>& words);

template <class T>
int binarySearch(const vector<T>& array, const string& key);

template <class T>
void funcPrintQueryResultBinarySearch(const vector<T>& array, const vector<string>& words);

// the class that holds all the sorting algorithms and their inner functions
template <class T>
class SortingAlgorithms {
private:
	// a function for swapping 2 items
	void swap(T& a, T& b) {
		T temp = a; a = b; b = temp;
	}

	// this merge function performs the merge without using extra memory space (in-place)
	void merge(vector<T>& array, int start, int mid, int end) {
		int p1 = start; // stores the index of first unprocessed element of left sublist
		int p2 = mid + 1; // stores the index of first unprocessed element of right sublist

		// while both sublists are not empty
		while (p1 <= mid && p2 <= end) {
			if (array[p1].getElement() <= array[p2].getElement()) {
				// array[p1] is at the correct position, so proceed to the next one
				p1 += 1;
			} else {
				// array[p1] is NOT at the correct position
				T temp = array[p2];
				int idx = p2;

				// shift all elements from array[p1, p2-1] to the right by one
				while (idx > p1) {
					array[idx] = array[idx - 1];
					idx -= 1;
				}
				array[p1] = temp; // move array[p2] to its correct place
				p1 += 1; p2 += 1; mid += 1; // incrase all paramters by one
			}
		}
	}

	// this partition function is for quick sort where changes the pivot depending on the provided argument
	int partition(vector<T>& array, int low, int high, string pivotType) {
		int pi; // the pivot index `pi` will change depending on the `pivotType`
		int result;
		if (pivotType == "first") {
			T pivot = array[low];
			int i = high + 1;

			for (int j = high; j > low; j--) {
				if (array[j].getElement() > pivot.getElement()) {
					i -= 1;
					swap(array[i], array[j]);
				}
			}
			swap(array[i-1], array[low]);			
			result = i - 1; // return i-1 as result
        } else if (pivotType == "random") {
            // set the pivot index as a random number between low and high
			srand(time(0));
            pi = low + rand() % (high - low + 1);
			swap(array[pi], array[high]);
			T pivot = array[high];
			int i = low - 1;

			for (int j = low; j < high; j++) {
				if (array[j].getElement() > pivot.getElement()) {
					i += 1;
					swap(array[i], array[j]);
				}
			}
			
			swap(array[i+1], array[high]);
			result = i + 1;
        } else if (pivotType == "median") {
            // median-of-three pivot selection
            int pi = low + (high - low) / 2;
			if (array[low].getElement() > array[pi].getElement()) {
				swap(array[low], array[pi]);
			}
			if (array[pi].getElement() > array[high].getElement()) {
				swap(array[pi], array[high]);
				if (array[low].getElement() > array[pi].getElement()) {
					swap(array[low], array[pi]);
				}
			}

			swap(array[pi], array[high]);
			T pivot = array[high];
			int i = low - 1;

			for (int j = low; j < high; j++) {
				if (array[j].getElement() > pivot.getElement()) {
					i += 1;
					swap(array[i], array[j]);
				}
			}

			swap(array[i + 1], array[high]);
			result = i + 1;
        }

		return result;
	}

	// this heapify function is for heap sort (rooted at node `i`)
	void heapify(vector<T>& array, int len, int i) {
		int largest = i; // set the largest as root
		int left = (2 * i) + 1; // left child of root
		int right = (2 * i) + 2; // right child of root

		if ((left < len) && (array[left].getElement() > array[largest].getElement())) {
			largest = left; // left child is larger than root
		}

		if ((right < len) && (array[right].getElement() > array[largest].getElement())) {
			largest = right; // right child is larger than root
		}

		if (largest != i) {
			// largest is not root
			swap(array[i], array[largest]); 
			heapify(array, len, largest); // recursively heapify the subtree
		}
	}

public:
	// the quick sort algorithm with 3 possible pivot types (must be provided)
	void quickSort(vector<T>& array, int low, int high, string pivotType) {
		if (low < high) {
			// the pivot index `pi` will change depending on the `pivotType` provided during `partition()`
			int pi = partition(array, low, high, pivotType);
            quickSort(array, low, pi - 1, pivotType);
            quickSort(array, pi + 1, high, pivotType);
		}
	}

   	// the in-place implementation of merge sort
	void mergeSort(vector<T>& array, int left, int right) {
		if (left == right) {
			return; // array of size 1 does not need sorting
		} else if (left < right) {
			int middle = left + (right - left) / 2; // same as (left+right)/2 but avoids overflow
			mergeSort(array, left, middle);
			mergeSort(array, middle + 1, right);
			merge(array, left, middle, right);
		}
	}

	// heap sort implementation (uses heapify)
	void heapSort(vector<T>& array, int len) {
		// build the heap by rearranging the array
		for (int i = len / 2 - 1; i >= 0; i--) {
			heapify(array, len, i);
		}

		// extract one element (max) from the heap at a time
		for (int i = len - 1; i >= 0; i--) {
			swap(array[0], array[i]); // move current root to the end
			heapify(array, i, 0); // max-heapify the reduced heap
		}
	}

	// insertion sort (`key` is of type of the elements of the table)
    void insertionSort(vector<T>& array, int len) {
		int i, j;
		T key = array[0]; // initialize key as something (doesn't matter which element)
		for (i = 1; i < len; i++) {
			key = array[i];
			j = i - 1;
			// move elements of arr[0..i-1] that are greater than key,
			// to one position ahead of their current position
			while ((j >= 0) && (array[j].getElement() > key.getElement())) {
				array[j+1] = array[j];
				j -= 1;
			}
			array[j+1] = key;
		}
	}
};


void SearchEngineV3();

int main() {
	SearchEngineV3();
	return 0;
}


void SearchEngineV3() {
	// process the files
	cout << "Enter number of input files: ";
	int numInputFiles;
	cin >> numInputFiles;
	vector<string> fileNames; // will be needed for updating the document info of elements of the BST and table

	for (int i = 0; i < numInputFiles; i++) {
		cout << "Enter " << i+1 << ". file name: ";
		string fileName;
		cin >> fileName;
		fileNames.push_back(fileName);
	}

	// pass the input files' vector to the below functions
	vector<string> words; // a vector of string that will hold all the words from the file
	int tempSize = (int)(getNumberOfWords(fileNames) / 2); // for the initial size of the hash table
	HashTable<string> wordTable(ITEM_NOT_FOUND, tempSize); // the hash table that will all the words and their information
	BinarySearchTree<string> wordTree(ITEM_NOT_FOUND); // the tree of words that will hold all the words and their information

	// store the words in both BST and Hash Table
	for (int i = 0; i < fileNames.size(); i++) {
		string text;
		ifstream readFile(fileNames[i]);
		while (getline(readFile, text)) {
			string sentence = text;
			funcLowerString(sentence); // lowercase all the content of the current sentence
			funcExtractWords(sentence, words); // extract the words of sentence and store it in a vector
		}
		readFile.close();
		for (int j = 0; j < words.size(); j++) {
			wordTable.funcInsert(words[j], fileNames[i]);
			wordTree.funcInsertItem(words[j], fileNames[i]);
		}
		words.clear(); // reset the vector to have a fresh list for the next file
	}

	// display the info for the hash table before getting the queries
	int uniqueWords = wordTable.getUniqueWordCount();
	double loadRatio = wordTable.getLoadFactor();
	cout << "\nAfter preprocessing, the unique word count is " << uniqueWords << ". Current load ratio is " << loadRatio << "\n";

	// for printing the binary search tree and hash table for presentation purposes
	// wordTree.funcPrettyPrintTree(); wordTable.funcPrintTable();

	// handle the queries
	string line; // each line of input
	words.clear(); // we want to use this again for separating the words in the query
    
	// clear input buffer before starting
	cin.clear(); 
	cin.ignore();

	cout << "Enter queried words in one line: ";
	string userQuery;
	getline(cin, userQuery);

	// clean up the user's query
	funcLowerString(userQuery); // we only want the lower-case of the queries
	funcExtractWords(userQuery, words); // we only want the words (not punctuations, nor numbers)

	// the results of user's query	
	string outputBinarySearchTree = "";
	string outputHashTable = "";
	string outputBinarySearchAlgo = "";

	// move to the timing section for the queries
	int k = 1; // number of iterations for time testing (has been set to 1 in case the PC is slow)

	// query docs with BST (using input files) and time it
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		outputBinarySearchTree = funcQueryBinarySearchTree(wordTree, words);
	}
	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	string reportBinarySearchTree = "Binary Search Tree Time: " + to_string(BSTTime.count() / k) + "\n";

	// query docs with hash table (using input files) and time it
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		outputHashTable = funcQueryHashTable(wordTable, words);
	}
	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	string reportHashTable = "Hash Table Time: " + to_string(HTTime.count() / k) + "\n";

	SortingAlgorithms<Entry<string>> sortInstance; // to call each sorting algorithm separately
	
	vector<Entry<string>> entryVector1; // will be used by quick sort, pivot = median
	wordTable.funcCopyEntriesToVector(entryVector1); // copy the entries of the hash table to the vector (will be unsorted)
	// keep multiples copies of this vector to use in the sorting algorithms
	vector<Entry<string>> entryVector2 = entryVector1; // will be used by quick sort, pivot = random
	vector<Entry<string>> entryVector3 = entryVector1; // will be used by quick sort, pivot = first
	vector<Entry<string>> entryVector4 = entryVector1; // will be used by merge sort
	vector<Entry<string>> entryVector5 = entryVector1; // will be used by heap sort
	vector<Entry<string>> entryVector6 = entryVector1; // will be used by insertion sort
	vector<Entry<string>> entryVector7 = entryVector1; // will be used for binary search
	vector<Entry<string>> unsortedVector = entryVector1; // this vector will never be sorted and is for reseting/unsorting the sorted vectors during time testing

	sortInstance.heapSort(entryVector7, entryVector7.size()); // for binary search
	
	// query with binary search algorithm (using a sorted vector) and time it
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		outputBinarySearchAlgo = funcQueryBinarySearchAlgorithm(entryVector7, words);
	}
	auto BinarySearchTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	string reportBinarySearchAlgo = "Binary Search Time: " + to_string(BinarySearchTime.count() / k) + "\n";

	// display the results of the queries
	cout << outputBinarySearchTree;
	cout << outputHashTable;
	cout << outputBinarySearchAlgo;
	cout << endl;

	// report the times
	cout << reportBinarySearchTree;
	cout << reportHashTable;
	cout << reportBinarySearchAlgo;
	cout << endl;

	// quick sort (median)
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		sortInstance.quickSort(entryVector1, 0, entryVector1.size()-1, "median");
		entryVector1 = unsortedVector; // unsort the vector to reset it
	}
	auto QuickSortMedianTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	cout << "Quick Sort(median) Time: " << QuickSortMedianTime.count() / k << "\n";

	// quick sort (random)
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		sortInstance.quickSort(entryVector2, 0, entryVector2.size()-1, "random");
		entryVector2 = unsortedVector;
	}
	auto QuickSortRandomTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	cout << "Quick Sort(random) Time: " << QuickSortRandomTime.count() / k << "\n";
	
	// quick sort (first)
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		sortInstance.quickSort(entryVector3, 0, entryVector3.size()-1, "first");
		entryVector3 = unsortedVector;
	}
	auto QuickSortFirstTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	cout << "Quick Sort(first) Time: " << QuickSortFirstTime.count() / k << "\n";
	
	// merge sort (in-place)	
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		sortInstance.mergeSort(entryVector4, 0, entryVector4.size()-1);
		entryVector4 = unsortedVector;
	}
	auto MergeSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	cout << "Merge Sort Time: " << MergeSortTime.count() / k << "\n";
	
	// heap sort
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		sortInstance.heapSort(entryVector5, entryVector5.size());
		entryVector5 = unsortedVector;
	}
	auto HeapSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	cout << "Heap Sort Time: " << HeapSortTime.count() / k << "\n";

	// insertion sort
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++) {
		sortInstance.insertionSort(entryVector6, entryVector6.size());
		entryVector6 = unsortedVector;
	}
	auto InsertionSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);

	cout << "Insertion Sort Time: " << InsertionSortTime.count() / k << "\n";

	// report the ratios
	cout << endl;
	cout << "Speed Up BST/HST: " << double(double(BSTTime.count()) / double(HTTime.count())) << "\n";
	cout << "Speed Up Merge Sort/Quick Sort(Median): " << double(double(MergeSortTime.count()) / double(QuickSortMedianTime.count())) << "\n";
	cout << "Speed Up Heap Sort/Quick Sort(Median): " << double(double(HeapSortTime.count()) / double(QuickSortMedianTime.count())) << "\n";
	cout << "Speed Up Insertion Sort/Quick Sort(Median): " << double(double(InsertionSortTime.count()) / double(QuickSortMedianTime.count())) << "\n";
	cout << endl;
	cout << "Speed Up Binary Search Tree/Binary Search Time: " << double(double(BSTTime.count()) / double(BinarySearchTime.count())) << "\n";
	cout << "Speed Up Hash Table/Binary Search Time: " << double(double(HTTime.count()) / double(BinarySearchTime.count())) << "\n";
}

// function for making a string lowercase
void funcLowerString(string& word) {
	for (int i = 0; i < word.size(); i++)
	{ word[i] = tolower(word[i]); }
}

// function for extrating words from a string
void funcExtractWords(const string& sentence, vector<string>& words) {
	string word = "";
	for (char c : sentence) {
		if (isalpha(c)) {
			// only alphabetical characters are allowed
			word += c;
		} else {
			// the character might be a number or a punctuation
			if (word != "") {
				words.push_back(word);
			}
			word = ""; // reset the word
		}
	}
	if (!word.empty()) {
		words.push_back(word);
	}
}

// the implementation of binary search algorithm
template <class T>
int binarySearch(const vector<T>& array, const string& key) {
	int low = 0;
	int high = array.size() - 1;

	while (low <= high) {
		int mid = low + (high - low) / 2; // avoids overflow
		if (array[mid].getElement() == key) {
			return mid; // return the index of the found element
		} else if (array[mid].getElement() < key) {
			low = mid + 1;
		} else if (array[mid].getElement() > key) {
			high = mid - 1;
		}
	}

	return -1; // element is not in the array
}

// function to return the names of the common files among all queried words (for Binary Search Tree)
template<class T>
vector<string> funcFindCommonFiles(const BinarySearchTree<T>& wordTree, const vector<string>& words) {
	// the `words` vector has more than one element when this function is being called
	vector<string> commonFiles = {};
	for (const string& word : words) {
		// check if the word is even in the tree to begin with
		if (wordTree.funcFindItem(word) != ITEM_NOT_FOUND) {
			vector<documentItem> docInfo = wordTree.funcGetDocInfo(word); // get the info of current word
			vector<string> docNamesList; // extract the names of the files for each doc
			for (const auto& doc : docInfo) { // it's of type `documentItem`
				docNamesList.push_back(doc.docName);
			}

			// `commonFiles` is initialized as `docNamesList` at first
			if (commonFiles.empty()) {
				commonFiles = docNamesList;
			} else {
				// moving onto the rest of the words and their list of file names
				vector<string> intersectedNames; // the intersection of file names
				// use the algorithm library to find the intersection of the 2 vectors and store it in `intersectedNames` 
				set_intersection(commonFiles.begin(), commonFiles.end(), docNamesList.begin(), docNamesList.end(), back_inserter(intersectedNames));
				commonFiles = intersectedNames; // update the common files
			}
		}
	}
	return commonFiles;
}

// function to return the names of the common files among all queried words (for Hash Table)
template <class T>
vector<string> funcFindCommonFiles(const HashTable<T>& wordTable, const vector<string>& words) {
	vector<string> commonFiles = {};
	for (const string& word : words) {
		// check if the word is even in the table to begin with
		if (wordTable.funcFind(word).getElement() != ITEM_NOT_FOUND) {
			vector<docItem> docInfo = wordTable.funcFind(word).getInfo(); // get the info of current word
			vector<string> docNamesList; // to store the names of the files for each doc
			for (const auto& doc : docInfo) { // it's of type `docItem`
				docNamesList.push_back(doc.docName);
			}

			// `commonFiles` is initialized as `docNamesList` at first
			if (commonFiles.empty()) {
				commonFiles = docNamesList;
			} else {
				// moving onto the rest of the words and their list of file names
				vector<string> intersectedNames; // the intersection of file names
				// use the algorithm library to find the intersection of the 2 vectors and store it in `intersectedNames` 
				set_intersection(commonFiles.begin(), commonFiles.end(), docNamesList.begin(), docNamesList.end(), back_inserter(intersectedNames));
				commonFiles = intersectedNames; // update the common files
			}
		}
	}
	return commonFiles;
}

// function to return the names of the common files among all queried words (for Binary Search Algorithm)
template <class T>
vector<string> funcFindCommonFiles(const vector<T>& array, const vector<string>& words) {
	vector<string> commonFiles = {};
	for (const string& word : words) {
		// check if the word is even in the array to begin with
		bool isInArray = false;
		int index = binarySearch(array, word); // find the index of the query in the array
		if (index != -1) {
			isInArray = true;
		}

		if (isInArray) {
			vector<docItem> docInfo = array[index].getInfo(); // get the info of current word
			vector<string> docNamesList; // to store the names of the files for each doc
			for (const auto& doc : docInfo) { // it's of type `docItem`
				docNamesList.push_back(doc.docName);
			}

			// `commonFiles` is initialized as `docNamesList` at first
			if (commonFiles.empty()) {
				commonFiles = docNamesList;
			} else {
				// moving onto the rest of the words and their list of file names
				vector<string> intersectedNames; // the intersection of file names
				// use the algorithm library to find the intersection of the 2 vectors and store it in `intersectedNames` 
				set_intersection(commonFiles.begin(), commonFiles.end(), docNamesList.begin(), docNamesList.end(), back_inserter(intersectedNames));
				commonFiles = intersectedNames; // update the common files
			}
		}
	}
	return commonFiles;
}


// this function returns the number of words in all files provided
int getNumberOfWords(const vector<string>& fileNames) {
	int result = 0;
	// a vector of string that will hold all the words from the file
	vector<string> words;
	for (int i = 0; i < fileNames.size(); i++) {
		string text;
		ifstream readFile(fileNames[i]);
		while (getline(readFile, text)) {
			string sentence = text;
			funcLowerString(sentence); // lowercase all the content of the current sentence
			funcExtractWords(sentence, words); // extract the words of sentence and store it in a vector
		}
		readFile.close();
		result += words.size();
		words.clear(); // reset the vector to have a fresh list for the next file
	}
	return result;
}


// this function returns the result of the querying via the binary search tree
template <class T>
string funcQueryBinarySearchTree(const BinarySearchTree<T>& wordTree, const vector<string>& words) {
	string result = "";
	// when the user only queries one word
	if (words.size() == 1) {
		if (wordTree.funcFindItem(words[0]) != ITEM_NOT_FOUND) {
			// if the word exists in the tree
			vector<documentItem> docInfo = wordTree.funcGetDocInfo(words[0]);
			for (const documentItem& doc : docInfo) {
				result += "in Document " + doc.docName + ", " + words[0] + " found " + to_string(doc.count) + " times.\n"; 
			}
		} else {
			result += "No document contains the given query\n";
			return result;
		}
	} else {
		// check if all queried words even exist in the tree
		bool valid = true;
		for (const string& word : words) {
			if (wordTree.funcFindItem(word) == ITEM_NOT_FOUND) {
				// a non-existing word has been found
				// automatically means that no document will contain the queried words
				valid = false;
			}
		}
		if (valid) {
			// there are multiple existing words
			// so we need to find their intersections/common files
			vector<string> commonFiles = funcFindCommonFiles(wordTree, words);
			if (commonFiles.empty()) {
				// when there are no intersections 
				result += "No document contains the given query\n";
				return result;
			} else {
				// at least 1 file is common between them
				for (const string& singleFile : commonFiles) {
					result += "in Document " + singleFile + ", ";
					// cout << "in Document " << singleFile << ", ";
					for (int k = 0; k < words.size() - 1; k++) {
						// all words except the last one
						int count = wordTree.funcGetDocCount(words[k], singleFile);
						result += words[k] + " found " + to_string(count) + " times, ";
					}
					// for the last word
					int count = wordTree.funcGetDocCount(words[words.size()-1], singleFile);
					result += words[words.size()-1] + " found " + to_string(count) + " times.\n";
				}
			}
		} else {
			result += "No document contains the given query\n";
			return result;
		}
	}
	return result;
}

// this function returns the result of the querying via the hash table
template <class T>
string funcQueryHashTable(const HashTable<T>& wordTable, const vector<string>& words) {
	string result = "";
	// user has queried only 1 word
	if (words.size() == 1) {
		// check if that single word exists in the table or not
		if (wordTable.funcFind(words[0]).getElement() != ITEM_NOT_FOUND) {
			// the word does exist in the table
			vector<docItem> queryInfo = wordTable.funcFind(words[0]).getInfo();
			for (const auto& i : queryInfo) {
				result += "in Document " + i.docName + ", " + words[0] + " found " + to_string(i.count) + " times.\n"; 
			}
		} else {
			result += "No document contains the given query\n";
			return result;
		}
	} else { // there are multiple words that the user wants
		// check if all the queried words even exist in the table
		bool valid = true;
		for (const auto& word : words) {
			if (wordTable.funcFind(word).getElement() == ITEM_NOT_FOUND) {
				// a non-existing word has been found, so no document will contain the query
				valid = false;
			}
		}
		if (!valid) {
			result += "No document contains the given query\n";
			return result;
		} else {
			// all words are at least in the table but not necessarily in the same file(s)
			// let's find the common file(s) of the queried words
			vector<string> commonFiles = funcFindCommonFiles(wordTable, words);
			if (commonFiles.empty()) {
				// when there are no common files for the queried words 
				result += "No document contains the given query\n";
				return result;
			} else {
				// at least 1 file is common among them
				for (const string& singleFile : commonFiles) {
					result += "in Document " + singleFile + ", ";
					for (int k = 0; k < words.size() - 1; k++) {
						// all words except the last one
						int count = wordTable.funcFetchDocCount(words[k], singleFile);
						result += words[k] + " found " + to_string(count) + " times, ";
					}
					// for the last word
					int count = wordTable.funcFetchDocCount(words[words.size()-1], singleFile);
					result += words[words.size()-1] + " found " + to_string(count) + " times.\n";
				}
			}
		}
	}
	return result;
}

// this function returns the result of the querying via the binary search algorithm (given a sorted array of items)
template <class T>
string funcQueryBinarySearchAlgorithm(const vector<T>& array, const vector<string>& words) {
	string result = "";
	int index;
	// when only 1 word is being queried
	if (words.size() == 1) {
		// check the existance of that one word in the vector
		const string queryWord = words[0]; // the first query word is the only one to be searched
		index = binarySearch(array, queryWord);

		if (index == -1) {
			result += "No document contains the given query\n";
			return result;
		} else {
			vector<docItem> docInfo = array[index].getInfo();
			for (const auto& doc : docInfo) {
				result += "in Document " + doc.docName + ", " + words[0] + " found " + to_string(doc.count) + " times.\n"; 
			}		 
		}
	} else {
		// user wants to query multiple words
		// check the existance of all words of the query in the vector
		bool allWordsExist = true; // assume they all exist by default

		// each item in the `wordPositions` vector corresponds to the index of any item in the `words` vector
		// when called by binary search
		vector<int> wordPositions;
		for (const auto& word : words) {
            index = binarySearch(array, word);
            if (index == -1) {
                allWordsExist = false; // even a single non-existent word will falsify the condition
                break;
            } else {
				wordPositions.push_back(index); // will be as the same as `words` and be used later
			}
        }

		if (!allWordsExist) {
			result += "No document contains the given query\n";
			return result;
		} else {
			// all query words do exist in the array
			vector<string> commonFiles = funcFindCommonFiles(array, words); // check if there is at least a single common file between them

			if (commonFiles.empty()) {
				// when there are no common files for the queried words 
				result += "No document contains the given query\n";
				return result;
			} else {
				// at least 1 file is common among them
				for (const string& singleFile : commonFiles) {
					result += "in Document " + singleFile + ", ";
					for (int k = 0; k < words.size() - 1; k++) {
						// all words except the last one
						int count = array[wordPositions[k]].getSpecificDocCount(singleFile); // `wordsPositions` is used here
						result += words[k] + " found " + to_string(count) + " times, ";
					}
					// for the last word
					int count = array[wordPositions[wordPositions.size()-1]].getSpecificDocCount(singleFile);
					result += words[words.size()-1] + " found " + to_string(count) + " times.\n";
				}
			}
		}
	}
	return result;
}
