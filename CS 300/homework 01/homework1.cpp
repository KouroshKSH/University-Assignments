/*
- This C++ code generates mazes using a modified version of the Depth-First Search (DFS) algorithm. It first generates mazes by breaking down walls between cells randomly, creating a connected maze. After generating mazes, the code performs a path-finding algorithm to find a path from an entry point to an exit point in a user-selected maze.
 - The maze generation process involves initializing a grid of cells with walls between them. The algorithm starts at a random cell, removes walls to neighboring cells, and continues until all cells have been visited. This creates a maze with connected paths and dead-ends.
- The generated mazes are stored in a vector, and each maze is written to a separate text file in a specified format. The format includes the dimensions of the maze and the state of walls for each cell.
 - The path-finding phase prompts the user to select a generated maze by ID, specify entry and exit points, and finds a path using a modified DFS algorithm. The path is then printed to a text file, and the final result is displayed.
- The code structure includes functions for maze generation, path finding, and utility functions for handling cells, walls, and stack operations. The stack is implemented using a custom stack class.
- Note: The code requires a "stack.h" header for stack implementation, and it uses the "Stack" class to manage the stack operations.
- Info: This is the main program where the majority of the code is written (maze generation and path finding).
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "stack.h"

using namespace std;

struct Cell {
	// x,y coordinates for the cell
	int x_pos;
	int y_pos;
	// walls of the cell
	bool up;
	bool down;
	bool left;
	bool right;

	Cell(int x, int y) : x_pos(x), y_pos(y) {
		// all the walls are set as true (existing) by default
		up = true;
		down = true;
		left = true;
		right = true;
	}
};

/*
The coordinate system of the maze:
    ^
	|
	| 2,0 | 2,1 | 2,2 | ...
y	|--------------------------
	| 1,0 | 1,1 | 1,2 | ...
	|--------------------------
	| 0,0 | 0,1 | 0,2 | ...
	--------------------------->
           x

y: represents rows
x: represents columns
*/

void funcPathFindingPhase(int numberOfMazes, const vector<vector<vector<Cell>>>& allMazes);
void funcMazeGenerationPhase();

bool funcCellHasUnvisitedNeighbor(const vector<vector<bool>>& unvisitedMaze, const int x, const int y, const int rows, const int cols);
vector<string> funcGetUnbrokenWalls(const Cell& currentCell, const vector<vector<bool>>& unvisitedMaze, int rows, int cols);
Cell funcChooseRandomNeighbor(vector<vector<Cell>>& myMaze, Cell& currentCell, vector<string>& unbrokenWalls, vector<vector<bool>>& unvisitedMaze);
vector<Cell> funcGetUnvisitedNeighbors(const Cell& currentCell, const vector<vector<bool>>& unvisitedMaze);
void funcCleanMazeUp(vector<vector<Cell>>& myMaze);
void funcMoveToAnotherCell(Cell& currentCell, const vector<vector<Cell>>& myMaze, vector<vector<bool>>& unvisitedMaze, const vector<string>& unbrokenWalls);
Stack<Cell> funcPrintPathStack(Stack<Cell>& pathStack, int currentID, int entryX, int entryY, int exitX, int exitY);
vector<string> funcBrokenWallsForPath(const Cell& currentCell, const vector<vector<Cell>>& currentMaze, const vector<vector<bool>>& unvisitedMaze);
bool funcNeighborExistenceForPath(const Cell& currentCell, const vector<vector<bool>>& unvisitedMaze);

int main(){
	funcMazeGenerationPhase();
	return 0;
}

// this function performs the maze generation
void funcMazeGenerationPhase() {
	srand(time(0)); // set the seed for the random number generator

	// getting inputs
	cout << "Enter the number of mazes: ";
	int numberOfMazes;
	cin >> numberOfMazes;
	cout << "Enter the number of rows and columns (M and N): ";
	int rows; int cols;
	cin >> rows >> cols;
	
	// this vector keeps all the mazes generation in it
	vector<vector<vector<Cell>>> allMazes;
	
    // currentID will be used for naming the txt files of generated mazes
	for (int currentID = 1; currentID <= numberOfMazes; currentID++) {
		vector<vector<Cell>> myMaze; // the maze containing the cells of type Cell struct
		vector<vector<bool>> unvisitedMaze; // the maze corresponding to unvisited cells

		// fill the maze with default cells
		for (int i = 0; i < cols; i++) {
			vector<Cell> currentCol;
			vector<bool> boolCol;
			for (int j = 0; j < rows; j++) {
				// x: columns, y: rows
				Cell currentCell(j,i); // (x,y) = (col,row)
				bool currentBool = true;
				currentCol.push_back(currentCell);
				boolCol.push_back(currentBool);
			}
			myMaze.push_back(currentCol);
			unvisitedMaze.push_back(boolCol);
		}

		Stack<Cell> cellStack; // stack for keeping track of cells
		Cell initialCell(0,0); // initial cells of position `(0,0)` to be pushed to stack
		cellStack.push(initialCell);
		int knockedWallsCount = 0; // number of knocked down walls should be `rows*columns - 1`

		while (!cellStack.isEmpty()) {
			// check if the top element of stack (use `peek()`) has unvisited neighbors
			// then check if it has walls to be broken (use `||` for its sides)
			// create a list of unbroken walls of that cell
			// -> pass it to the choose random neighbor function to get a new cell to visit

			Cell currentCell = cellStack.peek();
			int currentX = currentCell.x_pos; int currentY = currentCell.y_pos;

			// the top element of the stack has definitely been visited, so falsify its status in the unvisitedMaze
			unvisitedMaze[currentX][currentY] = false;
			
			if (funcCellHasUnvisitedNeighbor(unvisitedMaze, currentX, currentY, rows, cols)) {
				// to see the unvisited neighbors of the current cell (for internal testing)
				// vector<Cell> unvisitedNeighbors = funcGetUnvisitedNeighbors(currentCell, unvisitedMaze);

				// get a list of the unbroken walls of the current cell for choosing a random neighbor
				vector<string> unbrokenWalls = funcGetUnbrokenWalls(currentCell, unvisitedMaze, rows, cols);
				Cell nextCell = funcChooseRandomNeighbor(myMaze, currentCell, unbrokenWalls, unvisitedMaze);
			
				// push the new cell (which is a neighbor to currentCell) to the stack
				cellStack.push(nextCell);

				// update the number of knocked down walls (always < `rows*cols - 1`)
				knockedWallsCount += 1;		
			} else {
				// traverse back until you find an unvisited cell
				cellStack.pop();
			}
		}

		funcCleanMazeUp(myMaze);

		// add the current generated maze to the list of all mazes
		allMazes.push_back(myMaze);

        // write the generated maze to its txt file
		string fileName = "maze_" + to_string(currentID) + ".txt";
		ofstream outputFile(fileName);
		if (outputFile.is_open()) {
			outputFile << rows << " " << cols << endl;	
			for (int i = 0; i < myMaze.size(); i++) {
				for (int j = 0; j < myMaze[i].size(); j++) {
					//format of example: x=0 y=0 l=1 r=1 u=0 d=1
					outputFile << "x=" << i << " y=" << j <<
						" l=" << myMaze[i][j].left << 
						" r=" << myMaze[i][j].right <<
						" u=" << myMaze[i][j].up << 
						" d=" << myMaze[i][j].down << endl;
				}
			}
		} else {
			cout << "failed to create file.\n";
		}

        outputFile.close();
	}

    cout << "All mazes are generated.\n\n";

    // move onto path finding
	funcPathFindingPhase(numberOfMazes, allMazes);
}

// this function performs the path finding the chosen maze
void funcPathFindingPhase(int numberOfMazes, const vector<vector<vector<Cell>>>& allMazes) {
    // getting inputs
	cout << "Enter a maze ID between 1 to " << numberOfMazes << " inclusive to find a path: ";
	int desiredMazeID;
	cin >> desiredMazeID;

	vector<vector<Cell>> currentMaze = allMazes[desiredMazeID-1];
	int cols = currentMaze.size(); int rows = currentMaze[0].size();

	// initiate a maze where all of its cells are unvisited at first (same size of currentMaze)
	vector<vector<bool>> unvisitedMaze(cols, vector<bool>(rows, true));

    // get entry coordinates
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	int entryX; int entryY;
	cin >> entryX >> entryY;

    // get exit coordinates
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	int exitX; int exitY;
	cin >> exitX >> exitY;

	Stack<Cell> pathStack; // stack for keeping track of the path we travel from entry to exit
	// Cell initialCell(0,0); // initial cells of position `(0,0)` to be pushed to stack
	Cell entryCell = currentMaze[entryX][entryY]; // the entry cell that corresponds to the current maze's cell
	Cell currentCell  = currentMaze[entryX][entryY]; // the traveling cell that is used for the loop
	Cell exitCell = currentMaze[exitX][exitY]; // the exit cell that we want to reach
	pathStack.push(currentCell); // the path starts with the entry cell, hence pushing it

    // the current cell has to constantly move until it reaches the end
	while (!(currentCell.x_pos == exitX && currentCell.y_pos == exitY)) {
		// see what the top cell is in the path's stack
		currentCell = pathStack.peek();
		int currentX = currentCell.x_pos; int currentY = currentCell.y_pos;
		unvisitedMaze[currentX][currentY] = false; // this cell has been visited, so can't be chosen again

		if (funcNeighborExistenceForPath(currentCell, unvisitedMaze)) {
			// so the current cell has at least one unvisited neighbor
			// just to see the unvisited neighbors of the current cell (for internal testing)
			vector<Cell> unvisitedNeighbors = funcGetUnvisitedNeighbors(currentCell, unvisitedMaze);
			
            // get a list of the unbroken walls of the current cell for choosing a random neighbor
			vector<string> unbrokenWalls = funcBrokenWallsForPath(currentCell, currentMaze, unvisitedMaze);			
			
            // this function modifies the currentCell by turning it into the next cell that it's going to visit
			funcMoveToAnotherCell(currentCell, currentMaze, unvisitedMaze, unbrokenWalls);
			
            // push the updated currentCell to the path's stack
			pathStack.push(currentCell);
			
            // update the unvisited maze's status for the next cell that has been visited
			unvisitedMaze[currentCell.x_pos][currentCell.y_pos] = false;
		} else {
			// traverse back until you find a traveling cell that move
			pathStack.pop();
		}
	}

    // print the final path to file
	Stack<Cell> finalPath = funcPrintPathStack(pathStack, desiredMazeID, entryX, entryY, exitX, exitY);
}

// this function checks if the current cell (given its x,y coordinates), has unvisited neighbors (given no walls in-between)
bool funcNeighborExistenceForPath(const Cell& currentCell, const vector<vector<bool>>& unvisitedMaze) {
	int cols = unvisitedMaze.size(); int rows = unvisitedMaze[0].size();
	int x = currentCell.x_pos; int y = currentCell.y_pos;
	
    if ((x > 0 && x < cols-1) &&
		(y > 0 && y < rows-1)) { // the cell is a non-border cell
		if ((unvisitedMaze[x-1][y] == true && !currentCell.left) || 
			(unvisitedMaze[x+1][y] == true && !currentCell.right) || 
			(unvisitedMaze[x][y-1] == true && !currentCell.down) ||
			(unvisitedMaze[x][y+1] == true && !currentCell.up)) {return true;}
	}
	if ((x == 0) && (y > 0 && y < rows-1)) { // the cell is in the first column, but not in the corners
		if ((unvisitedMaze[x+1][y] == true && !currentCell.right) || 
			(unvisitedMaze[x][y-1] == true && !currentCell.down) ||
			(unvisitedMaze[x][y+1] == true && !currentCell.up)) {return true;}
	}
	if ((x == cols-1) && (y > 0 && y < rows-1)) { // the cell is in the last column, but not in the corners
		if ((unvisitedMaze[x-1][y] == true && !currentCell.left) || 
			(unvisitedMaze[x][y-1] == true && !currentCell.down) ||
			(unvisitedMaze[x][y+1] == true && !currentCell.up)) {return true;}
	}
	if ((x > 0 && x < cols-1) && y == 0) { // the cell is in the first row, but in the corners
		if ((unvisitedMaze[x-1][y] == true && !currentCell.left) || 
			(unvisitedMaze[x+1][y] == true && !currentCell.right) || 
			(unvisitedMaze[x][y+1] == true && !currentCell.up)) {return true;}
	}
	if ((x > 0 && x < cols-1) && y == rows-1) { // the cell is in the last row, but in the corners
		if ((unvisitedMaze[x-1][y] == true && !currentCell.left) || 
			(unvisitedMaze[x+1][y] == true && !currentCell.right) || 
			(unvisitedMaze[x][y-1] == true && !currentCell.down)) {return true;}
	}
	if (x == 0 && y == 0) { // it's the bottom-left corner
		if ((unvisitedMaze[0][1] == true  && !currentCell.up) ||
			(unvisitedMaze[1][0] == true  && !currentCell.right)) {return true;}
	}
	if (x == 0 && y == rows-1) { // it's the top-left corner
		if ((unvisitedMaze[0][y-1] == true && !currentCell.down) || 
			(unvisitedMaze[1][y] == true && !currentCell.right)) {return true;}
	}
	if (x == cols-1 && y == 0) { // it's the bottom-right corner
		if ((unvisitedMaze[x-1][0] == true && !currentCell.left) ||
		(unvisitedMaze[x][1] == true && !currentCell.up)) {return true;}
	}
	if (x == cols-1 && y == rows-1) { // it's the top-right corner
		if ((unvisitedMaze[x-1][y] == true && !currentCell.left) || 
			(unvisitedMaze[x][y-1] == true && !currentCell.down)) {return true;}
	}

	return false; // if all checks fail, then that cell has no neighbors that it can visit (with no walls between them)
}

// this function gives a list of the broken walls a cell can travel through if its neighbors are unvisited
vector<string> funcBrokenWallsForPath(const Cell& currentCell, const vector<vector<Cell>>& currentMaze, const vector<vector<bool>>& unvisitedMaze) {
	vector<string> brokenWalls; // the possible candidates (walls) will be added here
	int x = currentCell.x_pos; int y = currentCell.y_pos; // get the coordinates of the current cell
	int cols = currentMaze.size(); int rows = currentMaze[0].size(); // get the size of the maze

	// check corners
	if (x == 0 && y == 0) { // it's bottom-left corner
		if (!currentCell.right && unvisitedMaze[x+1][y]) {brokenWalls.push_back("right");}
		if (!currentCell.up && unvisitedMaze[x][y+1]) {brokenWalls.push_back("up");}
	} else if (x == 0 && y == rows-1) { // it's top-left corner
		if (!currentCell.right && unvisitedMaze[x+1][y]) {brokenWalls.push_back("right");}
		if (!currentCell.down && unvisitedMaze[x][y-1]) {brokenWalls.push_back("down");}
	} else if (x == cols-1 && y == 0) {// it's bottom-right corner
		if (!currentCell.left && unvisitedMaze[x-1][y]) {brokenWalls.push_back("left");}
		if (!currentCell.up && unvisitedMaze[x][y+1]) {brokenWalls.push_back("up");}
	} else if (x == cols-1 && y == rows-1) {// it's top-right corner
		if (!currentCell.left && unvisitedMaze[x-1][y]) {brokenWalls.push_back("left");}
		if (!currentCell.down && unvisitedMaze[x][y-1]) {brokenWalls.push_back("down");}
	}

	// check non-corner border cells
	if ((x > 0 && x < cols-1) && y == 0) { // it's the bottom edge
		if (!currentCell.right && unvisitedMaze[x+1][y]) {brokenWalls.push_back("right");}
		if (!currentCell.left && unvisitedMaze[x-1][y]) {brokenWalls.push_back("left");}
		if (!currentCell.up && unvisitedMaze[x][y+1]) {brokenWalls.push_back("up");}
	} else if ((x > 0 && x < cols-1) && y == rows-1) {// it's the top edge
		if (!currentCell.right && unvisitedMaze[x+1][y]) {brokenWalls.push_back("right");}
		if (!currentCell.left && unvisitedMaze[x-1][y]) {brokenWalls.push_back("left");}
		if (!currentCell.down && unvisitedMaze[x][y-1]) {brokenWalls.push_back("down");}
	} else if (x == 0 && (y > 0 && y < rows-1)) {// it's the left edge
		if (!currentCell.right && unvisitedMaze[x+1][y]) {brokenWalls.push_back("right");}
		if (!currentCell.down && unvisitedMaze[x][y-1]) {brokenWalls.push_back("down");}
		if (!currentCell.up && unvisitedMaze[x][y+1]) {brokenWalls.push_back("up");}
	} else if (x == cols-1 && (y > 0 && y < rows-1)) {// it's the right edge
		if (!currentCell.down && unvisitedMaze[x][y-1]) {brokenWalls.push_back("down");}
		if (!currentCell.left && unvisitedMaze[x-1][y]) {brokenWalls.push_back("left");}
		if (!currentCell.up && unvisitedMaze[x][y+1]) {brokenWalls.push_back("up");}
	}

	// check non-border cells
	if ((x > 0 && x < cols-1) && (y > 0 && y < rows-1)) {
		if (!currentCell.right && unvisitedMaze[x+1][y]) {brokenWalls.push_back("right");}
		if (!currentCell.left && unvisitedMaze[x-1][y]) {brokenWalls.push_back("left");}
		if (!currentCell.up && unvisitedMaze[x][y+1]) {brokenWalls.push_back("up");}
		if (!currentCell.down && unvisitedMaze[x][y-1]) {brokenWalls.push_back("down");}
	}

	return brokenWalls;
}

// this function prints the final path to the desired txt file and returns the pathStack as it was
Stack<Cell> funcPrintPathStack(Stack<Cell>& pathStack, int currentID, int entryX, int entryY, int exitX, int exitY) {
	vector<string> pathToPrint;
	Stack<Cell> tempStack;
	
	// copy the contents of `pathStack` to `tempStack` (didn't use the deep copy I implemented in the header)
	while (!pathStack.isEmpty()) {
		Cell currentCell = pathStack.peek();
		tempStack.push(currentCell);
		pathStack.pop();
	}

	// get the the items in `tempStack` and push to the path to print stack for later
	while (!tempStack.isEmpty()) {
		Cell currentCell = tempStack.peek();
		string currentStep = to_string(currentCell.x_pos) + " " + to_string(currentCell.y_pos);
		pathToPrint.push_back(currentStep);
		tempStack.pop();
	}

	// return the elements back to `pathStack`
	while (!tempStack.isEmpty()) {
		Cell currentCell = tempStack.peek();
		pathStack.push(currentCell);
		tempStack.pop();
	}

    // convention: `maze_mazeID_path_entryX_entryY_exitX_exitY.txt`
	string fileName = "maze_" + to_string(currentID) + 
					"_path_" + to_string(entryX) + "_" + to_string(entryY) + 
					"_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
	ofstream outputFile(fileName);
	if (outputFile.is_open()) {	
		for (int i = 0; i < pathToPrint.size(); i++) {
			outputFile << pathToPrint[i] << endl;
		}
	} else {
		cout << "failed to create file.\n";
	}

    outputFile.close();

	return pathStack;
}

// this function gets the current cell, the info of the unvisited maze's cells, and the unbroken wall(s) of the current cell
void funcMoveToAnotherCell(Cell& currentCell, const vector<vector<Cell>>& myMaze, vector<vector<bool>>& unvisitedMaze, const vector<string>& unbrokenWalls) {
	// choose a random wall from unbrokenWalls
    int randomIndex = rand() % unbrokenWalls.size();
    string randomWall = unbrokenWalls[randomIndex];
    int x = currentCell.x_pos; int y = currentCell.y_pos;

    // if the neighboring cells is unvisited, then the current cell can move and its position will be updated
	if (randomWall == "up" && unvisitedMaze[x][y+1] && currentCell.up == false) {y += 1;}
	if (randomWall == "down" && unvisitedMaze[x][y-1] && currentCell.down == false) {y -= 1;}
	if (randomWall == "left" && unvisitedMaze[x-1][y] && currentCell.left == false) {x -= 1;}
	if (randomWall == "right" && unvisitedMaze[x+1][y] && currentCell.right == false) {x += 1;}

	// update the status of current cell as the next cell
	currentCell.x_pos = myMaze[x][y].x_pos; currentCell.y_pos = myMaze[x][y].y_pos;
	currentCell.up = myMaze[x][y].up; currentCell.down = myMaze[x][y].down;
	currentCell.right = myMaze[x][y].right; currentCell.left = myMaze[x][y].left;

	// update the unvisited maze's status for the next cell that has been visited
	unvisitedMaze[x][y] = false;
}

// this function cleans up the maze based on neighbors
void funcCleanMazeUp(vector<vector<Cell>>& maze) {
    for (int i = 0; i < maze.size(); i++) {
		for (int j = 0; j < maze[i].size(); j++) {
			// check for non-border cells
			if ((i > 0 && i < maze.size()-1) && (j > 0 && j < maze[i].size()-1)) {
                // check top wall
				if (maze[i][j].up == true && maze[i][j+1].down == false) {maze[i][j].up = false;}
				// check down wall
				if (maze[i][j].down == true && maze[i][j-1].up == false) {maze[i][j].down = false;}
				// check left wall
				if (maze[i][j].left == true && maze[i-1][j].right == false) {maze[i][j].left = false;}
				// check right wall
				if (maze[i][j].right == true && maze[i+1][j].left == false) {maze[i][j].right = false;}
			}
			// check for first column non-corner cells
			if (i == 0 && (j > 0 && j < maze[i].size()-1)) {
				// check top wall
				if (maze[i][j].up == true && maze[i][j+1].down == false) {maze[i][j].up = false;}
				// check down wall
				if (maze[i][j].down == true && maze[i][j-1].up == false) {maze[i][j].down = false;}
				// check right wall
				if (maze[i][j].right == true && maze[i+1][j].left == false) {maze[i][j].right = false;}
			}
			// check for last column non-corner cells
			if (i == maze.size()-1 && (j > 0 && j < maze[i].size()-1)) {
				// check top wall
				if (maze[i][j].up == true && maze[i][j+1].down == false) {maze[i][j].up = false;}
				// check down wall
				if (maze[i][j].down == true && maze[i][j-1].up == false) {maze[i][j].down = false;}
				// check left wall
				if (maze[i][j].left == true && maze[i-1][j].right == false) {maze[i][j].left = false;}
			}
			// check for first row non-corner cells
			if ((i > 0 && i < maze.size()-1) && j == 0) {
				// check top wall
				if (maze[i][j].up == true && maze[i][j+1].down == false) {maze[i][j].up = false;}
				// check left wall
				if (maze[i][j].left == true && maze[i-1][j].right == false) {maze[i][j].left = false;}
				// check right wall
				if (maze[i][j].right == true && maze[i+1][j].left == false) {maze[i][j].right = false;}
			}
			// check for last row non-corner cells
			if ((i > 0 && i < maze.size()-1) && j == maze[i].size()-1) {
				// check down wall
				if (maze[i][j].down == true && maze[i][j-1].up == false) {maze[i][j].down = false;}
				// check left wall
				if (maze[i][j].left == true && maze[i-1][j].right == false) {maze[i][j].left = false;}
				// check right wall
				if (maze[i][j].right == true && maze[i+1][j].left == false) {maze[i][j].right = false;}
			}
			// check top-left corner
			if (i == 0 && j == maze[i].size()-1) {
				// check down wall
				if (maze[i][j].down == true && maze[i][j-1].up == false) {maze[i][j].down = false;}
				// check right wall
				if (maze[i][j].right == true && maze[i+1][j].left == false) {maze[i][j].right = false;}
			}
			// check bottom-left corner
			if (i == 0 && j == 0) {
				// check top wall
				if (maze[i][j].up == true && maze[i][j+1].down == false) {maze[i][j].up = false;}
				// check right wall
				if (maze[i][j].right == true && maze[i+1][j].left == false) {maze[i][j].right = false;}
			}
			// check top-right corner
			if (i == maze.size()-1 && j == maze[i].size()-1) {
				// check down wall
				if (maze[i][j].down == true && maze[i][j-1].up == false) {maze[i][j].down = false;}
				// check left wall
				if (maze[i][j].left == true && maze[i-1][j].right == false) {maze[i][j].left = false;}
			}
			// check bottom-right corner
			if (i == maze.size()-1 && j == 0) {
				// check top wall
				if (maze[i][j].up == true && maze[i][j+1].down == false) {maze[i][j].up = false;}
				// check left wall
				if (maze[i][j].left == true && maze[i-1][j].right == false) {maze[i][j].left = false;}
			}
		}
	}
}

// this function returns a list of unvisited neighbors of the current cell
vector<Cell> funcGetUnvisitedNeighbors(const Cell& currentCell, const vector<vector<bool>>& unvisitedMaze) {
	vector<Cell> unvisitedNeighbors;
	int x = currentCell.x_pos; int y = currentCell.y_pos;
	
	if ((x > 0 && x < unvisitedMaze.size()-1) &&
		(y > 0 && y < unvisitedMaze[0].size()-1)) { // the cell is a non-border cell
		if (unvisitedMaze[x-1][y]) {
			// the left cell is unvisited
			Cell unvisitedCell(x-1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x+1][y]) {
			// the right cell is unvisited
			Cell unvisitedCell(x+1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y-1]) {
			// the bottom cell is unvisited
			Cell unvisitedCell(x,y-1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y+1]) {
			// the top cell is unvisited
			Cell unvisitedCell(x,y+1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if ((x == 0) && (y > 0 && y < unvisitedMaze[0].size()-1)) { // the cell is in the first colum, but not in the corners
		if (unvisitedMaze[x+1][y]) {
			// the right cell is unvisited
			Cell unvisitedCell(x+1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y-1]) {
			// the bottom cell is unvisited
			Cell unvisitedCell(x,y-1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y+1]) {
			// the top cell is unvisited
			Cell unvisitedCell(x,y+1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if ((x == unvisitedMaze.size()-1) && (y > 0 && y < unvisitedMaze[0].size()-1)) { // the cell is in the last column, but not in the corners
		if (unvisitedMaze[x-1][y]) {
			// the left cell is unvisited
			Cell unvisitedCell(x-1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y-1]) {
			// the bottom cell is unvisited
			Cell unvisitedCell(x,y-1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y+1]) {
			// the top cell is unvisited
			Cell unvisitedCell(x,y+1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if ((x > 0 && x < unvisitedMaze.size()-1) && y == 0) { // the cell is in the first column, but in the corners
		if (unvisitedMaze[x-1][y]) {
			// the left cell is unvisited
			Cell unvisitedCell(x-1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x+1][y]) {
			// the right cell is unvisited
			Cell unvisitedCell(x+1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y+1]) {
			// the top cell is unvisited
			Cell unvisitedCell(x,y+1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if ((x > 0 && x < unvisitedMaze.size()-1) && y == unvisitedMaze[0].size()-1) { // the cell is in the last column, but in the corners
		if (unvisitedMaze[x-1][y]) {
			// the left cell is unvisited
			Cell unvisitedCell(x-1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x+1][y]) {
			// the right cell is unvisited
			Cell unvisitedCell(x+1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y-1]) {
			// the bottom cell is unvisited
			Cell unvisitedCell(x,y-1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if (x == 0 && y == 0) { // it's the bottom-left corner
		if (unvisitedMaze[x+1][y]) {
			// the right cell is unvisited
			Cell unvisitedCell(x+1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y+1]) {
			// the top cell is unvisited
			Cell unvisitedCell(x,y+1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if (x == 0 && y == unvisitedMaze[0].size()-1) { // it's the top-left corner
		if (unvisitedMaze[x+1][y]) {
			// the rights cell is unvisited
			Cell unvisitedCell(x+1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y-1]) {
			// the bottom cell is unvisited
			Cell unvisitedCell(x,y-1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if (x == unvisitedMaze.size()-1 && y == 0) { // it's the bottom-right corner
		if (unvisitedMaze[x-1][y]) {
			// the left cell is unvisited
			Cell unvisitedCell(x-1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y+1]) {
			// the top cell is unvisited
			Cell unvisitedCell(x,y+1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	if (x == unvisitedMaze.size()-1 && y == unvisitedMaze[0].size()-1) { // it's the top-right corner
		if (unvisitedMaze[x-1][y]) {
			// the left cell is unvisited
			Cell unvisitedCell(x-1,y);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
		if (unvisitedMaze[x][y-1]) {
			// the bottom cell is unvisited
			Cell unvisitedCell(x,y-1);
			unvisitedNeighbors.push_back(unvisitedCell);
		}
	}
	
	return unvisitedNeighbors;
}

// this function reutrns a random neighbor to go to
Cell funcChooseRandomNeighbor(vector<vector<Cell>>& myMaze, Cell& currentCell, vector<string>& unbrokenWalls, vector<vector<bool>>& unvisitedMaze) {
	// the steps:
	// 1. given the current cell, it first chooses a random unbroken wall to break
	// 2. falsifies the status of the wall for that cell
	// 3. based on the broken wall, it creates a new cell with appropriate coordinates
	// 4. the new cell will have its neighboring wall also broken upon creation

    // choose a random wall from unbrokenWalls
    int randomIndex = rand() % unbrokenWalls.size();
    string randomWall = unbrokenWalls[randomIndex];

    int x = currentCell.x_pos; int y = currentCell.y_pos; // get the coordinates of currentCell
	Cell nextCell(x,y); // initialize the next cell, update it later

    // update currentCell's wall status and the nextCell's attributes
    if (randomWall == "up" && unvisitedMaze[x][y+1]) {
        currentCell.up = false;
        nextCell.y_pos = y+1;
		nextCell.down = false;
    }
	if (randomWall == "down" && unvisitedMaze[x][y-1]) {
        currentCell.down = false;
        nextCell.y_pos = y-1;
		nextCell.up = false;
    }
	if (randomWall == "left" && unvisitedMaze[x-1][y]) {
        currentCell.left = false;
		nextCell.x_pos = x-1;
		nextCell.right = false;
    }
	if (randomWall == "right" && unvisitedMaze[x+1][y]) {
        currentCell.right = false;
		nextCell.x_pos = x+1;
		nextCell.left = false;
    }

	unvisitedMaze[nextCell.x_pos][nextCell.y_pos] = false; // falsify that cell since it's visited
	myMaze[currentCell.x_pos][currentCell.y_pos] = currentCell;
	myMaze[nextCell.x_pos][nextCell.y_pos] = nextCell;
    
	return nextCell;
}

// this function returns a list of the unbroken walls of a cell
vector<string> funcGetUnbrokenWalls(const Cell& currentCell, const vector<vector<bool>>& unvisitedMaze, int rows, int cols) {
	// check based on position and if the neighbor is unvisited (a visited neighbor's wall should not be a candidate)
	// NOTE: border cells should always have certain walls unbroken, so they should not be candidates
	
    vector<string> walls;
	int x = currentCell.x_pos; int y = currentCell.y_pos; // get the coordinates of currentCell

	if (x == 0 && y == 0) { // it's the bottom-left corner
		if (unvisitedMaze[1][0] && currentCell.right) {walls.push_back("right");}
		if (unvisitedMaze[0][1] && currentCell.up) {walls.push_back("up");}
	} else if (x == 0 && y == rows-1) { // it's the top-left corner
		if (unvisitedMaze[1][y] && currentCell.right) {walls.push_back("right");}
		if (unvisitedMaze[0][y-1] && currentCell.down) {walls.push_back("down");}
	} else if (x == cols-1 && y == 0) {// it's the bottom-right corner
		if (unvisitedMaze[x-1][0] && currentCell.left) {walls.push_back("left");}
		if (unvisitedMaze[x][1] && currentCell.up) {walls.push_back("up");}
	} else if (x == cols-1 && y == rows-1) {// it's the top-right corner
		if (unvisitedMaze[x-1][y] && currentCell.left) {walls.push_back("left");}
		if (unvisitedMaze[x][y-1] && currentCell.down) {walls.push_back("down");}
	} else if ((x > 0 && x < cols-1) &&
		(y > 0 && y < rows-1)) {// the cell is a non-border cell
		if (unvisitedMaze[x-1][y] && currentCell.left) {walls.push_back("left");}
		if (unvisitedMaze[x+1][y] && currentCell.right) {walls.push_back("right");}
		if (unvisitedMaze[x][y-1] && currentCell.down) {walls.push_back("down");}
		if (unvisitedMaze[x][y+1] && currentCell.up) {walls.push_back("up");}
	} else if ((x == 0) && (y > 0 && y < rows-1)) {// the cell is in the first column, but not in the corners
		if (unvisitedMaze[x+1][y] && currentCell.right) {walls.push_back("right");}
		if (unvisitedMaze[x][y-1] && currentCell.down) {walls.push_back("down");}
		if (unvisitedMaze[x][y+1] && currentCell.up) {walls.push_back("up");}
	} else if ((x == cols-1) && (y > 0 && y < rows-1)) {// the cell is in the last column, but not in the corners
		if (unvisitedMaze[x-1][y] && currentCell.left) {walls.push_back("left");}
		if (unvisitedMaze[x][y-1] && currentCell.down) {walls.push_back("down");}
		if (unvisitedMaze[x][y+1] && currentCell.up) {walls.push_back("up");}
	} else if ((x > 0 && x < cols-1) && y == 0) {// the cell is in the first row, but not in the corners
		if (unvisitedMaze[x-1][y] && currentCell.left) {walls.push_back("left");}
		if (unvisitedMaze[x+1][y] && currentCell.right) {walls.push_back("right");}
		if (unvisitedMaze[x][y+1] && currentCell.up) {walls.push_back("up");}
	} else if ((x > 0 && x < cols-1) && y == rows-1) {// the cell is in the last row, but not in the corners
		if (unvisitedMaze[x-1][y] && currentCell.left) {walls.push_back("left");}
		if (unvisitedMaze[x+1][y] && currentCell.right) {walls.push_back("right");}
		if (unvisitedMaze[x][y-1] && currentCell.down) {walls.push_back("down");}
	}

	return walls;
}

// this function checks if the current cell (given its x,y coordinates), has unvisited neighbors
bool funcCellHasUnvisitedNeighbor(const vector<vector<bool>>& unvisitedMaze, const int x, const int y, const int rows, const int cols) {	
    if ((x > 0 && x < cols-1) &&
		(y > 0 && y < rows-1)) {// the cell is a non-border cell
		if ((unvisitedMaze[x-1][y] == true) || 
			(unvisitedMaze[x+1][y] == true) || 
			(unvisitedMaze[x][y-1] == true) ||
			(unvisitedMaze[x][y+1] == true)) {return true;}
	}
	if ((x == 0) && (y > 0 && y < rows-1)) {// the cell is in the first column, but not in the corners
		if ((unvisitedMaze[x+1][y] == true) || 
			(unvisitedMaze[x][y-1] == true) ||
			(unvisitedMaze[x][y+1] == true)) {return true;}
	}
	if ((x == cols-1) && (y > 0 && y < rows-1)) {// the cell is in the last column, but not in the corners
		if ((unvisitedMaze[x-1][y] == true) || 
			(unvisitedMaze[x][y-1] == true) ||
			(unvisitedMaze[x][y+1] == true)) {return true;}
	}
	if ((x > 0 && x < cols-1) && y == 0) {// the cell is in the first row, but in the corners
		if ((unvisitedMaze[x-1][y] == true) || 
			(unvisitedMaze[x+1][y] == true) || 
			(unvisitedMaze[x][y+1] == true)) {return true;}
	}
	if ((x > 0 && x < cols-1) && y == rows-1) {// the cell is in the last row, but in the corners
		if ((unvisitedMaze[x-1][y] == true) || 
			(unvisitedMaze[x+1][y] == true) || 
			(unvisitedMaze[x][y-1] == true)) {return true;}
	}
	if (x == 0 && y == 0) {// it's the bottom-left corner
		if (unvisitedMaze[0][1] == true || unvisitedMaze[1][0] == true) {return true;}
	}
	if (x == 0 && y == rows-1) {// it's the top-left corner
		if (unvisitedMaze[0][y-1] == true || unvisitedMaze[1][y] == true) {return true;}
	}
	if (x == cols-1 && y == 0) {// it's the bottom-right corner
		if (unvisitedMaze[x-1][0] == true || unvisitedMaze[x][1] == true) {return true;}
	}
	if (x == cols-1 && y == rows-1) {// it's the top-right corner
		if (unvisitedMaze[x-1][y] == true || unvisitedMaze[x][y-1] == true) {return true;}
	}

	return false;
}
