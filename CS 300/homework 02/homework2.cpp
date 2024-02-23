/*
    Info: This is the main file for the main program.
	Steps:
		1) Get the number of files.
		2) Read each file.
		3) Make all the words of the given file lowercase.
		4) Separate the words and store in them the AVL tree.
		5) Get the queries from user.
		6) Based on the query, display a proper message.
*/

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "AVLTree.h"

using namespace std;

const string ITEM_NOT_FOUND = "---"; // globally defined

void funcLowerString(string& word);
void funcExtractWords(const string& sentence, vector<string>& words);
template <class T>
vector<string> funcFindCommonFiles(const AVLTree<T>& wordTree, const vector<string>& words);
void beginSearchEngine(); // Search Engine program

int main() {
	beginSearchEngine();
	return 0;
}

// function for making a string lowercase
void funcLowerString(string& word) {
	for (int i = 0; i < word.size(); i++) {
    	word[i] = tolower(word[i]);

	}
}

// function for extrating words from a string
void funcExtractWords(const string& sentence, vector<string>& words) {
	string word = "";
	for (char c : sentence) {
		if (isalpha(c) || c == '\'') {
			// only alphabetical characters and apostrophe-having words are allowed
			word += c;
		} else if (word != "") {
			words.push_back(word); // `word` can be added to the list
			word = ""; // reset `word`
		}
	}
	if (word != "") {
		words.push_back(word);
	}
}

// function to return the names of the common files among all queried words
template<class T>
vector<string> funcFindCommonFiles(const AVLTree<T>& wordTree, const vector<string>& words) {
	// the `words` vector has more than one element when this function is being called
	vector<string> commonFiles = {};
	for (const string& word : words) {
		// check if the word is even in the tree to begin with
		if (wordTree.funcFindItem(word) != ITEM_NOT_FOUND) {
			// get the info of current word
			vector<documentItem> docInfo = wordTree.funcGetDocInfo(word); 

			// extract the names of the files for each doc
			vector<string> docNamesList;
			for (const documentItem& doc : docInfo) {
				docNamesList.push_back(doc.docName);
			}

			// `commonFiles` is initialized as `docNamesList` at first
			if (commonFiles.empty()) {
				commonFiles = docNamesList;
			} else {
				// moving onto the rest of the words and their list of file names
				vector<string> intersectedNames; // the intersection of file names
				
				// use the algorithm library to find the intersection of the 2 vectors
				// and store it in `intersectedNames` 
				set_intersection(commonFiles.begin(), commonFiles.end(), docNamesList.begin(), docNamesList.end(), back_inserter(intersectedNames));
				
				// update the common files
				commonFiles = intersectedNames;
			}
		}
	}
	return commonFiles;
}

void beginSearchEngine() {
	// Stage1: Processing the files
	// get the files
	cout << "Enter number of input files: ";
	int numInputFiles; cin >> numInputFiles;
	vector<string> fileNames;

	for (int i = 0; i < numInputFiles; i++) {
		cout << "Enter " << i+1 << ". file name: ";
		string fileName; cin >> fileName;
		fileNames.push_back(fileName);
	}

	// Stage 2: Creating the tree
	// a vector of strings that will hold all the words
	vector<string> words; 

	// the tree of words that will hold all the words and their information
	AVLTree<string> wordTree(ITEM_NOT_FOUND); 

	for (int i = 0; i < fileNames.size(); i++) {
		string text;
		ifstream readFile(fileNames[i]);
		while (getline(readFile, text)) {
			string sentence = text;
			funcLowerString(sentence); // lowercase all the content of the current sentence
			funcExtractWords(sentence, words); // extract the words of sentence and store it in a vector
		}
		readFile.close(); // close the file to avoid errors
		for (int j = 0; j < words.size(); j++) {
			wordTree.funcInsertItem(words[j], fileNames[i]);
		}
		words.clear(); // reset the vector to have a fresh list for the next file
	}

	// Stage 3: Handling the queries
	string line; // each line of input
	words.clear(); // we want to use this again for separating the words in the query
	bool endOfQueries = false; // to terminate the program
    
	// clear input buffer before starting
	cin.clear(); 
	cin.ignore();

	while (endOfQueries == false) {
		cout << "\nEnter queried words in one line: ";

		// in case some error happens
		if (!getline(cin, line)) {
			break;
		}

		words.clear(); // reset each cycle for fresh queries
		
		if (line == "ENDOFINPUT") {
			// case1: finish the program
			endOfQueries = true;
		} else if (line.substr(0, 6) == "REMOVE") {
			// case2: remove the following words from the tree
			istringstream wordStream(line);
			string word;
			wordStream >> word; // skip "REMOVE"
			while (wordStream >> word) {
				funcLowerString(word);
				words.push_back(word); // 'words' vector contains the individual words
			}
			
			// as discussed in class and recitation, only 1 word will be removed at a time
			if (wordTree.funcFindItem(words[0]) != ITEM_NOT_FOUND) {
				wordTree.funcRemoveItem(words[0]);
				cout << words[0] << " has been REMOVED\n";
			} else {
				cout << "Can't REMOVE a word that doesn't exist\n";
			}
			
		} else {
			// case3: multiple words to be found and displayed
			istringstream wordStream(line);
            string word;
            while (wordStream >> word) {
				funcLowerString(word);
                words.push_back(word);
            }
			
			// when the user only queries one word
			if (words.size() == 1) {
				if (wordTree.funcFindItem(words[0]) == ITEM_NOT_FOUND) {
					// the word is not in the tree
					cout << "No document contains the given query\n";
				} else {
					// the word exists in the tree
					// get the info of the current word
					vector<documentItem> docInfo = wordTree.funcGetDocInfo(words[0]);
					for (const documentItem& doc : docInfo) {
						// print all the document info of the word
						cout << "in Document " << doc.docName << ", " << words[0] << " found " << doc.count << " times.\n";
					}
				}
			} else {
				// check if all queried words even exist in the tree
				bool valid = true;
				for (const string& word : words) {
					if (wordTree.funcFindItem(word) == ITEM_NOT_FOUND) {
						// a single non-existing word has been found
						valid = false;
						break;
					}
				}
				if (valid == false) {
					// automatically means that no document will contain the queried words
					cout << "No document contains the given query\n";
				} else {
					// there are multiple words, which all exist in the tree
					// so we need to find their intersections/common files
					vector<string> commonFiles = funcFindCommonFiles(wordTree, words);
					if (commonFiles.empty()) {
						// when there are no intersections 
						cout << "No document contains the given query\n";
					} else {
						// at least 1 file is common between them
						for (const string& singleFile : commonFiles) {
							// to print the final result
							string result;
							result += "in Document " + singleFile + ", ";
							for (int k = 0; k < words.size(); k++) {
								int count = wordTree.funcGetDocCount(words[k], singleFile);
								result += words[k] + " found " + to_string(count) + " times, ";
							}
							result = result.substr(0, result.size()-2) + ".\n"; // to handle the last word
							cout << result;
						}
					}
				} 
			}
		}
		cout << endl; // for printing purposes...
	}
}
