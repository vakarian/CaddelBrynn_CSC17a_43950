/* 
 * File:   main.cpp
 * Author: Brynn Caddel
 * Project 2 - MineSweeper 
 * CSC17A - 43950
 */

///System Libraries
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <vector>
#include "GameData.h"
#include "Board.h"
#include "Template.h"
#include "Abstract.h"


using namespace std;

//Execution Begins Here

int main(int argc, const char * argv[]) {
    ///Used when recording the length of a game
    srand(static_cast<unsigned int> (time(0)));

    ///Used for Template
    BoardGame<Matrix> m(new Settings(10, 10));

    try {
        m->config();
    }
    //Error Checking:
    //catch used to determine if board is too large or too small
    catch (Settings::tooSmall) {
        cout << "Error: Minefield was either too small or too large."
                "If you've entered a board size larger than 10x10,"
                "try entering a smaller size. Or your board may be too"
                "small to be compatible with your selected difficulty level."
                "Try selecting a larger board if you would like to play on"
                "Difficulty: EXPERT. ";
    }    catch (const char* ord) {
        cout << ord << endl;
    }

    return 0;
}

///Function Definitions

/// Function returns true if input is valid

bool Settings::check() const {
    ///Makes sure that the number of mines does not exceed spots in
    ///the minefield
    return (((rows * cols) > mines) && (mines > 0));
}

/// Function reveals what is underneath the square that the user has selected
/// and whether to continue based on what is revealed
/// i.e selecting a mine means you lost, game over
bool Settings::cont(int row, int col) {
    /// check if user selected a losing square
    if (field[row][col] == Settings::MINE)
        return false;

    /// Square is a zero, clear the surrounding area if necessary
    else if (noMine(row, col)) {
        revealZ(row, col); /// show cleared area
        ckborder();
        revealM();
        return true;
    }        /// Square had adjacent mine
        /// reveal the number to the user
    else {
        field[row][col] = proxM(row, col);
        revealM();
        return true;
    }
}

/// Function returns true if
/// there are 0 mines adjacent to selected square
bool Settings::noMine(int row, int col) const {
    if (proxM(row, col))
        return false; /// nAdjacent returned 1 or more
    return true; /// nAdjacent returned 0
}

/// Function checks whether the player has won
/// if there are no EMPTY spaces left the game is won
bool Settings::winCase() const {
    for (int i = 0; i != rows; ++i)
        for (int j = 0; j != cols; ++j)
            /// if there are empty spaces player has not won
            if (field[i][j] == Settings::NONE)
                return false;
    /// there were no empty spaces left. Player has won
    return true;
}

///This creates the board based off user selections of size
void Settings::create(int row, int col) {
    ///Used when creating dynamic board
    rows = row;
    cols = col;

    /// Creates each of the rows
    field = new int *[rows];

    /// Creates each of the columns
    for (int row = 0; row != rows; ++row)
        field[row] = new int [cols];
    //iterates to create board
}

///If input is less than or equal to 0, it throws input
void Settings::vRows(int row) {
    if (row <= 0)
        throw tooSmall();
    rows = row;
}

///If input is less than or equal to 0, it throws input
void Settings::vColmn(int col) {
    if (col <= 0)
        throw tooSmall();
    cols = col;
}

///Function prompts user to enter difficulty level and minefield size
void Settings::userIn() {
    ///Allows user to input the size of their board
    cout << "                 " << endl;
    cout << " ---------------------------------- " << endl;
    cout << "|         Minefield Size:          |" << endl;
    cout << " ---------------------------------- " << endl;
    cout << "Note: Board will be of size nxn." << endl;
    cout << "Enter the value of n: ";
    int row;
    cin >> row;
    /// Invalid sizes. Restricted between 1 and 10 to ensure it can be saved in
    /// a binary file with out blowing things up
    if (row > 10 || row < 1)
        throw tooSmall();
    rows = row;
    cols = row;
    int rigour;
    ///Allows user to enter difficulty of their game
    cout << "                                    " << endl;
    cout << " ---------------------------------- " << endl;
    cout << "|        Difficulty Levels:        |" << endl;
    cout << " ---------------------------------- " << endl;
    cout << "| Beginner     = 0                 |" << endl;
    cout << "| Intermediate = 1                 |" << endl;
    cout << "| Expert       = 2                 |" << endl;
    cout << " ---------------------------------- " << endl;
    cout << "Please enter your difficulty level: ";
    cin >> rigour;
    cout << "                                    " << endl;
    cout << "                                    " << endl;
    mines = nMines(convert(rigour));
}

void Settings::config() {
    int choice;
    bool exitMenu = true;
    char *player = playerN();
    ///Loop: Implemented Until Exit 
    cout << "    " << endl;
    do {
        ///Output Menu
        cout << " ---------------------------------- " << endl;
        cout << "|       Select From The Menu       |" << endl;
        cout << " ---------------------------------- " << endl;
        cout << "| 1. Play MineSweeper              |" << endl;
        cout << "| 2. Instructions and Gameplay     |" << endl;
        cout << "| 3. Press Anything Else to Exit   |" << endl;
        cout << " ---------------------------------- " << endl;
        cout << "Selection: ";
        cin>>choice;
        cout << "          " <<endl;
        cout << "          " <<endl;
        switch (choice) {
            case 1:
                /// ask user if they want to play
                cout << "Hello, " << player << ", welcome to MineSweeper!" << endl;
                cout << "Would you like to continue? " << endl;
                cout << "| y for yes | n for no |" << endl;
                char input;
                cin >> input;
                cout << "          " <<endl;

                /// If yes, then it prompts the user to play the game
                if (input == 'y') {
                    cout << "Would you like to load a previous game? " << endl;
                    cout << "To continue:" << endl;
                    cout << "| y for yes | n for no |" << endl;
                    char inputdeux;
                    cin >> inputdeux;
                     cout << "          " <<endl;

                    ///Loads the previous game
                    if (inputdeux == 'y') {
                        loadGame();
                    } else
                        /// Retrieves row and column information from user
                        userIn();
                    ///Verifies input
                    if (check()) {
                        while (input == 'y' && check()) {
                            ///Runs the actual program
                            run();
                            cout << endl;
                            cin.ignore();
                            cout << "Would you like to play again?" << endl;
                            cout << "To continue:" << endl;
                            cout << "| y for yes | n for no |" << endl;
                            cin >> input;
                            cout << endl;
                            /// Retrieves data if user wants to continue
                            if (input == 'y') {
                                userIn();
                                ///Wipes the boards
                                wipeB();
                            }
                        }
                    }///Error Checking: information was invalid
                    else
                        throw tooSmall();
                }
                cout << "Game Over." << endl;


                /// Taking out the trash
                delete player;
                break;

            case 2:
                instructions();
                break;

                ///Exits Menu  
            default: exitMenu = false;
        }

    } while (exitMenu);


}

///Function allows user to enter rows, columns, and difficulty
void Settings::run() {
    ///Calls function that sets mines
    mineSet();
    ///Calls function that reveals the mines
    revealM();
    int row, col;
    ///Initializes turn at 0 to start turn count
    int turn = 0;
    ///Implemented to record length of game
    int initialTime = static_cast<unsigned int> (time(0));
    do {
        int tNought = static_cast<unsigned int> (time(0));
        cout << "Turn: " << turn++ << endl;
        /// Row Selection
        do {
            ///Allows user to save and exit
            cout << "To save and exit: enter -1" << endl;
            cout << "Row: ";
            cin >> row;
            /// If user wants to save game and exit, they can press 0
            if (row == -1) {
                ///Calls function to save game board information
                saveGame();
                return;
            }
            ///Error checking: checks validity of bounds for rows
        } while (row < 0 || row >= rows);
        /// Column Selection
        do {
            cout << "Column: ";
            cin >> col;
            ///Error Checking: checks validity of bounds for columns
        } while (col < 0 || col >= cols);

        /// End Process
        int tFinal = static_cast<unsigned int> (time(0));
        cout << "Turn took: " << tFinal - tNought << " seconds.\n";
        cout << endl;
    } while (cont(row, col) && !winCase());

    ///Once the game has either been won or lost
    ///Win Case
    if (winCase()) {
        cout << "You win! " << endl;
        cout << "You win! The Special Forces will now contact you," << endl;
        cout << " as your skill in cleverly evading mines may prove" << endl;
        cout << " to be useful out on the field." << endl;
        flagSet();
    }        ///Lose Case
    else {
        cout << "*boom* You have lost the game..." << endl;
        flagSet();
        field[row][col] = Settings::LOSER;
    }

    ///This displays the length of time it took for user to complete game
    int finalTime = static_cast<unsigned int> (time(0));
    cout << "You've completed the game in"
            " " << finalTime - initialTime << " seconds." << endl;

    ///This prints the final board
    print();
}

///Character array with strings to satisfy rubric
char* Settings::playerN() {
    cout << "Enter your first name: ";
    string in;
    cin >> in;
    typedef string::size_type sType;
    sType size = in.size();
    char *name = new char[size + 1];
    for (sType i = 0; i != size; ++i) {
        *(name + i) = in[i];
    }
    *(name + size + 1) = '\0';

    return name;
}

/// Function that clears the grid on which game will be player
void Settings::wipeB() {
    /// Make sure each square is empty
    for (int i = 0; i != rows; ++i)
        for (int j = 0; j != rows; ++j)
            field[i][j] = Settings::NONE;
}

///This function takes the int variable and returns it as Settings::Difficulty
Settings::Difficulty Settings::convert(int choice) {
    ///The purpose of these cases is to  determine how many mines to set 
    switch (choice) {
        case (0):
            return Settings::BEGINNER;
            break;
        case (1):
            return Settings::INTERMEDIATE;
            break;
        case (2):
            return Settings::EXPERT;
        default:
            return Settings::BEGINNER;
            break;
    }
}

///Once the game is won/lost, this function prints the location of the mines
///revealed
void Settings::print() const {
    cout << "Board: ";
    for (int row = 0; row != rows; ++row) {
        for (int col = 0; col != cols; ++col) {
            ///Used to identify mines/empty spots/proximity to mines
            if (*(*(field + row) + col) == Settings::LOSER)
                cout << "t  ";
            else if (*(*(field + row) + col) == Settings::MINE)
                cout << "x  ";
            else if (!noMine(row, col))
                cout << proxM(row, col) << "  ";
            else
                cout << "0  ";
        }
        cout << endl;
    }
    cout << endl;
}

///Once the game is won/lost, this function prints the location of the mines
///revealed
void Settings::revealM() const {
    /// Print the column index
    for (int i = 0; i != cols; ++i) {
        if (i == 0)
            cout << "  ";
        cout << setw(3) << i;
    }
    cout << endl;
    for (int row = 0; row != rows; ++row) {
        for (int col = 0; col != cols; ++col) {
            if (col == 0 && row < 10) cout << row << "  ";
            if (col == 0 && row >= 10) cout << row << " ";
            ///Keeps empty squares and mines hidden
            if (field[row][col] == Settings::NONE ||
                    field[row][col] == Settings::MINE)
                cout << setw(3) << right << "+ ";
                ///Reveals cleared areas
            else if (field[row][col] == Settings::NOMINE)
                cout << setw(2) << 0 << " ";
                /// Print out the actual value of the square
            else
                cout << setw(2) << field[row][col] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

///There are a varying number of mines laid out depending on the user inputted
///difficulty level
int Settings::nMines(Settings::Difficulty userSet) const {
    if (userSet == Settings::BEGINNER)
        return (rows * cols) / 10;
    else if (userSet == Settings::INTERMEDIATE)
        return (rows * cols) / 6;
    else
        return (rows * cols) / 4;
}

///Function randomly places mines in the field
void Settings::mineSet() {
    ///Determines how many mines will be used
    int copyM = mines;
    ///Continues looping through mines until they're all laid out
    while (copyM) {
        for (int i = 0; i != rows; ++i) {
            for (int j = 0; j != cols; ++j) {
                /// place mines if result of rand()%15 == 0
                if ((rand() % 100) % 10 == 0) {
                    ///only place mines if mines are still available
                    /// and current space is empty
                    if (copyM && field[i][j] == Settings::NONE) {
                        /// set the mine
                        field[i][j] = Settings::MINE;
                        /// decrement number of mines available
                        --copyM;
                    }
                }
            }
        }
    }
}

///Determine how many mines are adjacent to selected square
int Settings::proxM(int row, int col, int FLAG) const {
    ///The number of adjacent mines
    int adjMine = 0;
    ///Ignore first/last row/column
    /// Search interior
    if (row > 0 && col > 0 && row < rows - 1 && col < cols - 1) {
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j)
                if (field[i][j] == FLAG)
                    ++adjMine;
        }
    }        /// checks first row, doesn't check first or last column
    else if (row == 0 && col > 0 && col < cols - 1) {
        for (int i = row; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j)
                if (field[i][j] == Settings::MINE)
                    ++adjMine;
        }
    }        /// checks last row, doesn't check first or last column
    else if (row == rows - 1 && col > 0 && col < cols - 1) {
        for (int i = row - 1; i <= row; ++i) {
            for (int j = col - 1; j <= col + 1; ++j)
                if (field[i][j] == Settings::MINE)
                    ++adjMine;
        }
    }        ///checks first column, doesn't check first or last row
        ///sweeps to the right
    else if (col == 0 && row > 0 && row < rows - 1) {
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col; j <= col + 1; ++j)
                if (field[i][j] == Settings::MINE)
                    ++adjMine;
        }
    }        /// checks last column, doesn't check first or last row
        /// sweeps to the left
    else if (col == cols - 1 && row > 0 && row < rows - 1) {
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col; ++j)
                if (field[i][j] == Settings::MINE)
                    ++adjMine;
        }
    }        /// checks top left corner
    else if (row == 0 && col == 0) {
        if (field[row][col + 1] == Settings::MINE) ++adjMine;
        if (field[row + 1][col] == Settings::MINE) ++adjMine;
        if (field[row + 1][col + 1] == Settings::MINE) ++adjMine;
    }        /// checks top right corner
    else if (row == 0 && col == cols - 1) {
        if (field[row][col - 1] == Settings::MINE) ++adjMine;
        if (field[row + 1][col] == Settings::MINE) ++adjMine;
        if (field[row + 1][col - 1] == Settings::MINE) ++adjMine;
    }        /// checks bottom left corner
    else if (row == rows - 1 && col == 0) {
        if (field[row - 1][col] == Settings::MINE) ++adjMine;
        if (field[row - 1][col + 1] == Settings::MINE) ++adjMine;
        if (field[row][col + 1] == Settings::MINE) ++adjMine;
    }        /// checks bottom right corner
    else if (row == rows - 1 && col == cols - 1) {
        if (field[row - 1][col - 1] == Settings::MINE) ++adjMine;
        if (field[row - 1][col] == Settings::MINE) ++adjMine;
        if (field[row][col - 1] == Settings::MINE) ++adjMine;
    }
    /// return number of mines from appropriate if statement

    return adjMine;
}

/// Clear an area whose values are CLEAR
/// i.e 0 adjacent mines
void Settings::revealZ(int row, int col) {
    /// check bounds
    if (row >= rows || row < 0 || col >= cols || col < 0)
        return;
    if (noMine(row, col) && field[row][col] != Settings::NOMINE) {
        field[row][col] = Settings::NOMINE;
        /// go up one row
        revealZ(row + 1, col);
        /// go down one row
        revealZ(row - 1, col);
        /// go right one col
        revealZ(row, col + 1);
        /// go left one col
        revealZ(row, col - 1);
    }        /// space was not clear or already shown
    else
        return;
}

/// Function shows how many mines are adjacent to selected square
/// for the entire Minesweeper
void Settings::flagSet() {
    for (int i = 0; i != rows; ++i)
        for (int j = 0; j != cols; ++j)
            /// don't look for adjacent mines in areas where
            /// mine is already located
            if (field[i][j] != Settings::MINE)
                field[i][j] = proxM(i, j);
}

/// Function finds the perimeter of the cleared areas
void Settings::ckborder() {
    for (int row = 0; row != rows; ++row) {
        /// avoid searching at left and right edge of array
        for (int col = 0; col != cols; ++col) {
            /// when you're not on the bounds of the array
            if (row > 0 && row < rows - 1
                    && col > 0 && col < cols - 1) {
                if (field[row][col] == Settings::NOMINE) {
                    /// check that the previous number has mines adjacent
                    if (field[row][col - 1] != Settings::NOMINE)
                        field[row][col - 1] = proxM(row, col - 1);
                    /// check if the next number has mines adjacent
                    if (field[row][col + 1] != Settings::NOMINE)
                        field[row][col + 1] = proxM(row, col + 1);
                    if (field[row - 1][col] != Settings::NOMINE)
                        field[row - 1][col] = proxM(row - 1, col);
                    /// check if the next number has mines adjacent
                    if (field[row + 1][col] != Settings::NOMINE)
                        field[row + 1][col] = proxM(row + 1, col);
                    /// check the adjacent corners
                    if (field[row - 1][col - 1] != Settings::NOMINE)
                        field[row - 1][col - 1] = proxM(row - 1, col - 1);
                    if (field[row - 1][col + 1] != Settings::NOMINE)
                        field[row - 1][col + 1] = proxM(row - 1, col + 1);
                    if (field[row + 1][col - 1] != Settings::NOMINE)
                        field[row + 1][col - 1] = proxM(row + 1, col - 1);
                    if (field[row + 1][col + 1] != Settings::NOMINE)
                        field[row + 1][col + 1] = proxM(row + 1, col + 1);
                }
            }
        }
    }
}


void Settings::saveGame() {
    fstream saveFile("gameSave", ios::out | ios::binary);
    saveFile.write(reinterpret_cast<char*> (this), sizeof (*this));
    saveFile.close();
}

/// Function prints the data variable from the Minesweeper structure
/// written to a binary file
void Settings::loadGame() {
    fstream saveFile("gameSave", ios::in | ios::binary);
    if (!saveFile.is_open())
        throw "No previous settings found\n";

    saveFile.read(reinterpret_cast<char*> (this), sizeof (*this));
    //print();
    saveFile.close();
}

Settings& Settings::operator=(const Settings &right) {
    create(right.retRows(), right.retColmn());

    for (int i = 0; i != right.retRows(); ++i) {
        for (int j = 0; j != right.retColmn(); ++j)
            field[i][j] = right[i][j];
    }
    return *this;
}

///This creates the board based off user selections of size
void Matrix::create(int row, int col) {
    ///Used when creating dynamic board
    rows = row;
    cols = col;

    /// Creates each of the rows
    field = new int *[rows];

    /// Creates each of the columns
    for (int row = 0; row != rows; ++row)
        field[row] = new int [cols];
    //iterates to create board
}

/// Function that clears the grid on which game will be played
///Allows user to play game again
void Matrix::wipeB() {
    for (int i = 0; i != rows; ++i)
        for (int j = 0; j != cols; ++j)
            field[i][j] = 0;
}

void Matrix::loadGame() {
}

void Matrix::config() {
}

/// Function deallocates memory
void Matrix::destroy() {
    ///Deletes each row
    for (int i = 0; i != rows; ++i)
        delete[] field[i];
    ///Deletes the dynamically allocated structures
    delete field;
}

void Matrix::vRows(int row) {
    ///Checks whether the input value for rows is less than of equal to 0
    if (row <= 0)
        throw tooSmall();
    rows = row;
}

void Matrix::vColmn(int col) {
    ///Checks whether the input value for columns is less than of equal to 0
    if (col <= 0)
        throw tooSmall();
    cols = col;
}

void Matrix::print() const {
    for (int i = 0; i != rows; ++i) {
        for (int j = 0; j != cols; ++j) {
            std::cout << field[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

///Function that displays the rules of the game
void Settings::instructions() {
    cout << "The Objective:" << endl;
    cout << "To locate all the mines without clicking/uncovering them." << endl;
    cout << "If you uncover a mine, you lose the game." << endl;
    cout << " " << endl;
    cout << "Difficulty Levels:" << endl;
    cout << "Beginner: 9x9 Area: 10 mines" << endl;
    cout << "Intermediate: 16x16 35 mines" << endl;
    cout << "Expert: 25x25 45 mines" << endl;
    cout << "Disclaimer: If you choose to make a grid that isn't stated " << endl;
    cout << "above, then the number of mines will be increased or decreased." << endl;
    cout << "accordingly" << endl;
    cout << "    " << endl;
    cout << "Your MineSweeper board will always be of size nxn." << endl;
    cout << "Example: When n=4" << endl;
    cout << "   1 2 3 4 " << endl;
    cout << " 1 x x x x " << endl;
    cout << " 2 x x x x " << endl;
    cout << " 3 x x x x " << endl;
    cout << " 4 x x x x " << endl;
    cout << "    " << endl;
    cout << "Gameplay:" << endl;
    cout << "When the game begins, every square will be covered," << endl;
    cout << "and it is up to you to discover in which squares the mines are." << endl;
    cout << "By selecting a row, followed by a column, you will reveal one " << endl;
    cout << "of the squares underneath. It will either be blank, marked by" << endl;
    cout << "a number to indicate how many adjacent mines are present, or a" << endl;
    cout << "mine. Uncovering a mine will end the game." << endl;
    cout << " " << endl;
    cout << "Numbered Squares: " << endl;
    cout << "These are the squares you use to decipher where the mines are hidden" << endl;
    cout << "Each of these has a number which determines how many mines are in the" << endl;
    cout << "surrounding 8 squares(e.g. North, North-East, East, South-East, South," << endl;
    cout << "South-West, West, and North-West of the square)." << endl;
    cout << " " << endl;
    cout << "Good luck!" << endl;
    cout << " " << endl;
};



