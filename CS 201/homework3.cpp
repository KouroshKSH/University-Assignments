#include <iostream>
#include <string>

using namespace std;

void funcPlayGame(string a, string b, string c, string d, int &e, int &f, string g[][3]);
void funcPlayerMove(string a, string b, string c[][3]);
bool funcCheckFormat(string a);
bool funcCheckWinner(string a, string b, string c[][3]);
void funcPrint(string a[][3]);
bool funcNotEmpty(int a, int b, string c[][3]);
void funcResetBoard(string c[][3]);

int main()
{
    cout << "Welcome to Tic-Tac-Toe!\n";
    string player1, player2;
    cout << "Player 1, please enter your name: ";
    cin >> player1;
    cout << "Hello, " << player1 <<". You are going to play with X\n";
    cout << "Player 2, please enter your name: ";
    cin >> player2;
    cout << "Hello, " << player2 <<". You are going to play with O\n";

    // Initialize the game
    string p1Sym = "X", p2Sym = "O";
    int p1Score = 0, p2Score = 0;
    string tictactoeBoard[3][3] = {{" ", " ", " "}, {" ", " ", " "}, {" ", " ", " "}};

    // Game 1
    cout << "\nStarting a new game...\n";
    funcPlayGame(player1, p1Sym, player2, p2Sym, p1Score, p2Score, tictactoeBoard);

    if (funcCheckWinner(player1, p1Sym, tictactoeBoard) && funcCheckWinner(player2, p2Sym, tictactoeBoard))
    {
        cout << "The game ended in a draw!\n";
    } else if (funcCheckWinner(player1, p1Sym, tictactoeBoard) == true)
    {
        p1Score += 1;
        cout << player1 << " is the winner!\n";
    } else if (funcCheckWinner(player2, p2Sym, tictactoeBoard) == true)
    {
        p2Score += 1;
        cout << player2 << " is the winner!\n";
    }

    cout << "Current Scores: \n";
    cout << player1 << ": " << p1Score << "   "<< player2 << ": " << p2Score << endl;

    // Game 2
    cout << "\nStarting a new game...\n";
    funcResetBoard(tictactoeBoard);
    funcPlayGame(player2, p2Sym, player1, p1Sym, p2Score, p1Score, tictactoeBoard);
    
    if (funcCheckWinner(player1, p1Sym, tictactoeBoard) == true && funcCheckWinner(player2, p2Sym, tictactoeBoard) == true)
    {
        cout << "The game ended in a draw!\n";
    } else if (funcCheckWinner(player1, p1Sym, tictactoeBoard) == true)
    {
        p1Score += 1;
        cout << player1 << " is the winner!\n";
    } else if (funcCheckWinner(player2, p2Sym, tictactoeBoard) == true)
    {
        p2Score += 1;
        cout << player2 << " is the winner!\n";
    }
    
    cout << "Current Scores: \n";
    cout << player1 << ": " << p1Score << "   "<< player2 << ": " << p2Score << endl;

    cout << "\nFinal Scores: \n";
    cout << player1 << ": " << p1Score << "   "<< player2 << ": " << p2Score << endl;
    
    // Determine final winner
    if (p1Score > p2Score)
    {
        cout << player1 << " is the overall winner!\n";
    } else if (p2Score > p1Score) {
        cout << player2 << " is the overall winner!\n";
    } else {
        cout << "The overall game ended in a draw!\n";
    }
    
    return 0;
}

void funcPlayGame(string player1, string p1Sym, string player2, string p2Sym, int &p1Score, int &p2Score, string tictactoeBoard[][3])
{
    // Run until the game ends (win or draw)
    while (funcCheckWinner(player1, p1Sym, tictactoeBoard) == false && funcCheckWinner(player2, p2Sym, tictactoeBoard) == false)
    {
        funcPrint(tictactoeBoard);
        funcPlayerMove(player1, p1Sym, tictactoeBoard);
        
        if (funcCheckWinner(player1, p1Sym, tictactoeBoard) == false && funcCheckWinner(player2, p2Sym, tictactoeBoard) == false)
        {
            funcPrint(tictactoeBoard);
            funcPlayerMove(player2, p2Sym, tictactoeBoard);
        }
    } 
}

void funcPlayerMove(string player, string symbol, string tictactoeBoard[][3])
{
    string pMove;
    cout << player << ", please enter your move: ";
    cin >> pMove;
    bool flag = funcCheckFormat(pMove);
    if (flag == false)
    {
        while (flag == false)
        {
            cout << player << ", please enter your move: ";
            cin >> pMove;
            flag = funcCheckFormat(pMove);
        }
    }

    // Check validity of the spot
    int x = stoi(&pMove[0]) - 1, y = stoi(&pMove[2]) - 1;
    if (tictactoeBoard[x][y] == " ")
    {
        tictactoeBoard[x][y] = symbol;
    } else {
        flag = funcNotEmpty(x, y, tictactoeBoard);
        while (flag == true)
        {
            cout << "That location is already full!\n";
            cout << player << ", please enter your move: ";
            cin >> pMove;
            x = stoi(&pMove[0]) - 1; y = stoi(&pMove[2]) - 1;
            if (tictactoeBoard[x][y] == " ")
            {
                tictactoeBoard[x][y] = symbol;
                flag = false;
            }
        }
    }
}

bool funcCheckFormat(string move)
{
    // Return true only if it contains numbers between 1 & 3 with a dash, e.g.: 1-2
    if ((move.length() == 3) && (1 <= stoi(&move[0]) && stoi(&move[0]) <= 3) && (move[1] == '-') && (1 <= stoi(&move[2]) && stoi(&move[2]) <= 3))
    {
        return true;
    }
    cout << "Please enter correct move format!\n";
    return false;
}

bool funcCheckWinner(string player, string symbol, string board[][3])
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
        {
            // Won a row
            return true;
        } else if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)
        {
            // Won a column
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == symbol &&  board[1][1] == symbol &&  board[2][2] == symbol)
    {
        // Main diagonal has 3 similar symbols
        return true;
    } else if (board[0][2] == symbol &&  board[1][1] == symbol &&  board[2][0] == symbol)
    {
        // Antidiagonal has 3 similar symbols
        return true;
    } else if (board[0][0] != " " && board[0][1] != " " && board[0][2] != " " && board[1][0] != " " && board[1][1] != " " && board[1][2] != " " && board[2][0] != " " && board[2][1] != " " && board[2][2] != " ") {
        // Draw
        return true;
    }

    // No winning condition has been met
    return false;
}

void funcPrint(string board[][3])
{
    cout << "Current board: " << endl;
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < 3; j++){
            if (j == 2){
                cout << board[i][j];
            }else if (j == 0){
                cout << " " << board[i][j] << " | ";
            }else if (j == 1){
                cout << board[i][j] << " | ";
            }
        }
        cout << endl;
        if (i == 0 || i == 1){
            cout << "-----------" << endl;
        }
    }
}

bool funcNotEmpty(int x, int y, string tictactoeBoard[][3])
{
    // Returns true when the specified place is filled already
    if (tictactoeBoard[x][y] == "X" || tictactoeBoard[x][y] == "O")
    {
        return true;
    } else {
        return false;
    }
}

void funcResetBoard(string board[][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = " ";
        }
    }
}