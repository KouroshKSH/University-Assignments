#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void funcPrintCharMatrix(const vector<vector<char>> &matrix);
vector<vector<char>> funcQueryToMatrix(const string &stringQuery);
void funcCompareMatrices(const vector<vector<char>> &matBig, const vector<vector<char>> &matSmall);

struct xyPosition
{ // Store the x-y position of each item for comparison & printing
    int x; // row
    int y; // column
};

int main()
{
    cout << "Please enter the file name:\n";
    string fileName;
    ifstream input;
    input.open(fileName.c_str());
    bool fileOpened = false;
	while (!fileOpened)
	{ // Ask for the file name until user enters a valid file
		cin >> fileName;
		input.open(fileName.c_str());
		if (!input.fail())
		{ // User gave a correct name as input
		    fileOpened = true;
		} else {
		    cout << "Could not open the file. Please enter a valid file name:\n";
		    cin.clear();
		}
	}

    vector<vector<char>> matrix;
    if (input.is_open())
    { // When file has been opened successfully
        string line;
        int lineLength = -1; // For checking the line length
        while (getline(input, line))
        {
            for (int i = 0; i < line.length(); i++)
            { // Only '#' and '-' are acceptable characters
                if (line[i] != '#' && line[i] != '-')
                { // End the program if file has other characters
                    cout << "Erroneous file content. Program terminates.\n";
                    return 0;
                }
            }
            // Check if all lines share the same length
            if (lineLength == -1)
            { // First line being checked, hence being -1 by default
                lineLength = line.length();
            } else if (lineLength != line.length()) {
                cout << "Erroneous file content. Program terminates.\n";
                return 0;
            }
            // Store the file content in the vector based on each character
            vector<char> matLine;
            for (char c:line)
            {
                matLine.push_back(c);
            }
            matrix.push_back(matLine);
            matLine.clear();  // Empty the line to avoid duplication
        }
    }
    cout << "The matrix file contains:\n"; // Display the file as a matrix
    funcPrintCharMatrix(matrix);
    // Get query from user as string until they quit
    string stringQuery;
    bool queryFlag = true;
    while (queryFlag)
    {
        cout << "Enter the string query for a shape, or \"Quit\" to terminate the program:\n";
        cin >> stringQuery;
        if (stringQuery == "Quit")
        { // "Quit" is case-sensitive
            queryFlag = false;
        } else {
            // No placement is possible If no. of '/' in the query is >=  no. of rows.
            int countSlash = 0;
            for (char c:stringQuery)
            {
                if (c == '/') { countSlash += 1; }
            }
            if (matrix.size() <= countSlash)
            {
                cout << "No placements found.\n";
            } else {
                // Convert the query from string to a 2D char matrix
                vector<vector<char>> matrixQuery = funcQueryToMatrix(stringQuery);
                funcCompareMatrices(matrix, matrixQuery);
            } 
        }
    } 
    return 0;
}

void funcCompareMatrices(const vector<vector<char>> &matBig, const vector<vector<char>> &matSmall)
{
    // Store the stars from user's query in the x-y struct
    vector<xyPosition> starPositions;
    for (int i = 0; i < matSmall.size(); i++)
    {
        for (int j = 0; j < matSmall[i].size(); j++)
        {
            if (matSmall[i][j] == '*')
            {
                xyPosition currentStar;
                currentStar.x = i;
                currentStar.y = j;
                starPositions.push_back(currentStar);
            }
        }
    }
	// Start iterating over bigger matrix and the starPosition struct if dimensions are valid
    int rowBig = matBig.size(), colBig = matBig[0].size();
    int rowSmall = matSmall.size(), colSmall = matSmall[0].size();
    if (colSmall > colBig)
    { // The query can't have more columns than the file
        cout << "No placements found.\n";
    } else {
        vector<xyPosition> candidates;
        int len = starPositions.size();
        for (int rb = 0; rb < rowBig - rowSmall + 1; rb++) // rb: row big
        {
            for (int cb = 0; cb < colBig - colSmall + 1; cb++) // cb: col big
            {
                bool flagNoOverlap = true;
                for (int k = 0; k < len; k++) // k: index of star
                { // Compare the positions of the dashes with stars using the starPositions struct
                    int rs = starPositions[k].x, cs = starPositions[k].y;
                    if (matBig[rb + rs][cb + cs] != '-')
                    { // '*' can only be on top of '-'
                        flagNoOverlap = false;
                        k = len + 1; // break out of the loop
                    }
                }
                if (flagNoOverlap)
                { // A valid candidate has been found
                    xyPosition candid;
                    candid.x = rb; candid.y = cb;
                    candidates.push_back(candid);
                }
            }
        }
		// Proceed with printing the possible candidates if available
		if (candidates.size() == 0)
        { // Meaning no candidate was found, therefore no placements possible
            cout << "No placements found.\n";
        } else {
            for (int i = 0; i < candidates.size(); i++)
            { // Go over all candidates
                cout << "Placement number " << i + 1 << ":\n";
                for (int j = 0; j < len; j++)
                { // Print all the placements with respect to the first star
                    int rs = starPositions[j].x, cs = starPositions[j].y;
                    cout << "(" << candidates[i].x + rs << "," << candidates[i].y + cs << ") ";
                }
                cout << endl; // Each placement should be in a different line
            }
        }
    }
}

vector<vector<char>> funcQueryToMatrix(const string &stringQuery)
{
    // Create a 2D matrix out of the query
    vector<vector<char>> matrixQuery;
    vector<char> row;
    int num = 0;
    for (char c : stringQuery)
    {
        if (isdigit(c))
        { // Number of times 's' or 'b' should be inserted
            num = (num * 10) + (c - '0');
        } else if (c == 's') {
            for (int i = 0; i < num; i++) { row.push_back('s'); }
            num = 0; // Reset num to avoid extra insertions
        } else if (c == 'b') {
            for (int i = 0; i < num; i++) { row.push_back('b'); }
            num = 0;
        } else if (c == '/') {
            matrixQuery.push_back(row);
            row.clear(); // Reset row to avoid duplication
        }
    }
    matrixQuery.push_back(row);
    // Find maximum column size
    int maxCol = 0;
    for (int i = 0; i < matrixQuery.size(); i++)
    {
        if (matrixQuery[i].size() > maxCol) { maxCol = matrixQuery[i].size(); }
    }
    // Create a matrix of size row * max_column
    vector<vector<char>> sqaureMatrix(matrixQuery.size(), vector<char>(maxCol, ' ')); // All are spaces by default
    for (int i = 0; i < matrixQuery.size(); i++)
    {
        for (int j = 0; j < matrixQuery[i].size(); j++)
        { // Fill appropriate spots with '*' 
            if (matrixQuery[i][j] == 's') { sqaureMatrix[i][j] = '*'; }
        }
    }
    return sqaureMatrix;
}


void funcPrintCharMatrix(const vector<vector<char>>& matrix)
{
    for (vector<char> row : matrix)
    {
        for (char element : row)
        {
            cout << element;
        }
        cout << endl;
    }
}