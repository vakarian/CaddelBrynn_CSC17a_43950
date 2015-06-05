/* 
 * File:   main.cpp
 * Author: Brynn Caddel
 * Project 1 - MineSweeper 
 * CSC17A 43950
 */

///System Libraries
#include <cstdlib>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

///Data Structures
struct Settings {
    ///Holds the game's data
    int **field; 
    ///Rows on minefield
    int rows; 
    ///columns on minefield
    int columns;
    ///number of mines on minefield
    int mines; 
    enum Difficulty {BEGINNER, INTERMEDIATE, EXPERT};
    enum Flags {NONE = 10, MINE, NOMINE, LOSER};
};

using namespace std;

///Function Prototypes
//Descriptions of each function can be found in Function Definitions
Settings *create(int, int); 
Settings::Difficulty convert(int);
bool check(int, int, Settings::Difficulty);
bool noMine(Settings *, int, int);
bool resume(Settings *, int, int);
bool winCase(Settings *);
char *playerN();
int nMines(Settings::Difficulty);
int proxM(Settings *, int, int, int = Settings::MINE);
void ckborder(Settings *);
void clear(Settings *, int, int);
void destroy(Settings *); 
void flagSet(Settings *);
void hideMines(Settings *); 
void instructions();
void mineSet(Settings *);
void readFile(string);
void revealM(Settings *); 
void revealZ(Settings *, int, int);
void runGame(int, int, Settings::Difficulty,char*);
void userIn(int&, Settings::Difficulty&);
void writeBinary(Settings *, string);

///Execution Begins Here
int main(int argc, const char * argv[]) {
    ///Declare Variables: Menu
    int choice;                 
    bool exitMenu=true;
    char *player = playerN();
    ///Loop: Implemented Until Exit 
    cout << "    " << endl;
    do{
        ///Output Menu
        cout << "Select From The Menu" << endl;
        cout << "      " << endl;
        cout << "1. Play MineSweeper" <<endl;
        cout << "2. Instructions and Gameplay" <<endl;
        cout << "3. View Previous Game Statistics" <<endl;
        cout << "4. Press Anything Else to Exit" <<endl;
        cout << "      " << endl;
        cout<<"Selection: ";
        cin>>choice;
        switch(choice){
            case 1:  
            cout << "Hello, " << player << ", welcome to MineSweeper!" << endl;
            cout << "Press 'y' to continue." << endl;
            char ans;
            cin >> ans;
            cout << "      " << endl;
            cout << "      " << endl;

    ///This allows the user to choose the size of their minefield and the
    ///difficulty level of the game
    if (ans == 'y') {
        ///Creates the variables in the game: matrix size and difficulty
        int nrows;
        Settings::Difficulty userSet;
        ///This function collects the number of rows and difficulty and stores
        ///it in an array
        userIn(nrows, userSet);
        ///Error Checking: checks validity of user input
        ///If valid, then it creates the array
        if (check(nrows, nrows, userSet)) {
            while (ans == 'y' && check(nrows, nrows, userSet)) {
                runGame(nrows, nrows, userSet,player);
                cout << endl;
                cout << "Would you like to play again?";
                cin >> ans;
                cout << endl;
                ///User has the option to play again
                if (ans == 'y')
                    userIn(nrows, userSet);
            }
        }            ///Else, the program exits
        else
            cout << "Error: Minefield too small."
                "The size of the minefield is not compatible with the"
                "difficulty level. Please try a larger minefield if you're"
                "playing on Difficulty:EXPERT";
    }
    cout << "Game Over.";
    
    ///Take Out Trash(clean up)
     delete player;
                break;

            case 2:
                ///Displays instructions
                instructions(); 
                break;
                
            case 3:
               ///Reads Binary File
               readFile("result"); 
                
            ///Exits Menu  
            default: exitMenu = false; 
        }
  
    } while (exitMenu);

    return 0;
}

///Function Definitions

//Function prompts user to enter difficulty level and minefield size
void userIn(int &rows, Settings::Difficulty &userSet) {
    int dLevel;
    cout << "                                    " << endl;
    cout << "Difficulty levels:                  " << endl;
    cout << "| Beginner     = 0 |                " << endl;
    cout << "| Intermediate = 1 |                " << endl;
    cout << "| Expert       = 2 |                " << endl;
    cout << "Please enter your difficulty level: ";
    cin >> dLevel;
    userSet = convert(dLevel);

    cout << "                 " << endl;
    cout << "Minefield Size:  " << endl;
    cout << "(Your MineSweeper board will be of size nxn.)" << endl;
    cout << "Enter the value of n: ";
    cin >> rows;
    cout << "                 " << endl;
}

/// Function returns true if input is valid
bool check(int rows, int cols, Settings::Difficulty dLevel) {
    ///Makes sure that the number of mines does not exceed spots in
    ///the minefield
    return (rows * cols) > nMines(dLevel);
}

///Function allows user to enter rows, columns, and difficulty
void runGame(int nrows, int ncols, Settings::Difficulty dLevel,char *p) {
    srand(static_cast<unsigned int> (time(0)));
    Settings *mineFld = create(nrows, ncols);
    mineFld->mines = nMines(dLevel);
    mineSet(mineFld);
    hideMines(mineFld);
    int row, col;
    do {
        /// Select the row
        do {
            cout << "Enter the row: ";
            cin >> row;
            ///Error checking: checks validity of bounds for rows
        } while (row < 0 || row >= mineFld->rows);
        do {
            cout << "Enter the column: ";
            cin >> col;
            ///Error Checking: checks validity of bounds for columns
        } while (col < 0 || col >= mineFld->columns);
        cout << endl;
    } while (resume(mineFld, row, col) && !winCase(mineFld));

    ///Once the game has either been won or lost
    ///Win Case
    if (winCase(mineFld)) {
        cout << "You win! The Special Forces will now contact you," << endl;
        cout << " as your skill in cleverly evading mines may prove" << endl;
        cout << " to be useful out on the field." << endl;
        flagSet(mineFld);
    }        ///Lose Case
    else {
        cout << "*boom* You have lost the game..." << endl;
        flagSet(mineFld);
        mineFld->field[row][col] = Settings::LOSER;
    }
    /// Print the complete minefield
    revealM(mineFld);

    /// write result to binary file
    writeBinary(mineFld, "result");
    /// deallocate the game area
    destroy(mineFld);
}

///Function generates the grid the game is to be played on
Settings* create(int rows, int cols) {
    ///Dynamically allocating memory for minefield
    Settings *out = new Settings;
    out->rows = rows;
    out->columns = cols;

    ///Creates 2D playing field
    out->field = new int *[rows];

    ///Creates each new row
    for (int row = 0; row != rows; ++row)
        out->field[row] = new int [cols];

    ///Ensures that each space is empty
    for (int i = 0; i != rows; ++i)
        for (int j = 0; j != rows; ++j)
            out->field[i][j] = Settings::NONE;
    return out;
}

///This function takes the int variable and returns it as Settings::Difficulty
Settings::Difficulty convert(int choice) {
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

///Deallocating memory function
void destroy(Settings *mineFld) {
    ///This deletes each row that was dynamically allocated
    for (int i = 0; i != mineFld->rows; ++i)
        delete[] mineFld->field[i];
    ///This deletes the dynamically allocated structure
    delete mineFld;
}

///Once the game is won/lost, this function prints the location of the mines
///revealed
void revealM(Settings* mineFld) {
    for (int row = 0; row != mineFld->rows; ++row) {
        for (int col = 0; col != mineFld->columns; ++col) {
            ///
            if (*(*(mineFld->field + row) + col) == Settings::LOSER)
                cout << "#  ";
            else if (*(*(mineFld->field + row) + col) == Settings::MINE)
                cout << "*  ";
            else if (!noMine(mineFld, row, col))
                cout << proxM(mineFld, row, col) << "  ";
            else
                cout << "0  ";
        }
        cout << endl;
    }
    cout << endl;
}

///This function prints the field with mines hidden
void hideMines(Settings* mineFld) {
    //Print the column index
    for (int i = 0; i != mineFld->columns; ++i) {
        //Makes spacing look nice
        if (i == 0)
            cout << "  ";
        cout << setw(3) << i;
    }
    cout << endl;
    for (int row = 0; row != mineFld->rows; ++row) {
        for (int col = 0; col != mineFld->columns; ++col) {
            if (col == 0 && row < 10) cout << row << "  ";
            if (col == 0 && row >= 10) cout << row << " ";
            /// keeps empty spaces and mines hidden
            if (mineFld->field[row][col] == Settings::NONE ||
                    mineFld->field[row][col] == Settings::MINE)
                cout << setw(3) << right << "x ";
                /// print out the clear areas
            else if (mineFld->field[row][col] == Settings::NOMINE)
                cout << setw(2) << 0 << " ";
                /// Print out the actual value of the square
            else
                cout << setw(2) << mineFld->field[row][col] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

///There are a varying number of mines laid out depending on the user inputted
///difficulty level
int nMines(Settings::Difficulty userSet) {
    if (userSet == Settings::BEGINNER)
        return 15;
    else if (userSet == Settings::INTERMEDIATE)
        return 30;
    else
        return 45;
}

///Function randomly places mines in the field
void mineSet(Settings *mineFld) {
    ///Determines how many mines will be used
    int mines = mineFld->mines;

    ///Continues looping through mines until they're all laid out
    while (mines) {
        for (int i = 0; i != mineFld->rows; ++i) {
            for (int j = 0; j != mineFld->columns; ++j) {
                /// place mines if result of rand()%15 == 0
                if ((rand() % 100) % 10 == 0) {
                    ///only place mines if mines are still available
                    /// and current is empty
                    if (mines && mineFld->field[i][j] == Settings::NONE) {
                        /// set the mine
                        mineFld->field[i][j] = Settings::MINE;
                        --mines;
                    }
                }
            }
        }
    }
}

/// Function returns how  many 'flag' elements surround a given square
int proxM(Settings *mineFld, int row, int col, int FLAG) {
    int adjmine = 0; /// the number of adjacent mines
    
    ///Ignore first/last row/column
    /// Search interior
    if (row > 0 && col > 0 && row < mineFld->rows - 1 && col < mineFld->columns - 1) {
        /// search the 3x3 grid surrounding a cell
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j)
                if (mineFld->field[i][j] == FLAG)
                    ++adjmine;
        }
    }        /// checks first row, doesn't check first or last column
    else if (row == 0 && col > 0 && col < mineFld->columns - 1) {
        for (int i = row; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }        /// checks last row, doesn't check first or last column
    else if (row == mineFld->rows - 1 && col > 0 && col < mineFld->columns - 1) {
        for (int i = row - 1; i <= row; ++i) {
            for (int j = col - 1; j <= col + 1; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }        /// checks first column, doesn't check first or last row
        ///sweeps to the right
    else if (col == 0 && row > 0 && row < mineFld->rows - 1) {
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col; j <= col + 1; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }        /// checks last column, doesn't check first or last row
        /// sweeps to the left
    else if (col == mineFld->columns - 1 && row > 0 && row < mineFld->rows - 1) {
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }        /// checks top left corner
    else if (row == 0 && col == 0) {
        if (mineFld->field[row][col + 1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row + 1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row + 1][col + 1] == Settings::MINE) ++adjmine;
    }        /// checks top right corner
    else if (row == 0 && col == mineFld->columns - 1) {
        if (mineFld->field[row][col - 1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row + 1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row + 1][col - 1] == Settings::MINE) ++adjmine;
    }        /// checks bottom left corner
    else if (row == mineFld->rows - 1 && col == 0) {
        if (mineFld->field[row - 1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row - 1][col + 1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row][col + 1] == Settings::MINE) ++adjmine;
    }        /// checks bottom right corner
    else if (row == mineFld->rows - 1 && col == mineFld->columns - 1) {
        if (mineFld->field[row - 1][col - 1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row - 1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row][col - 1] == Settings::MINE) ++adjmine;
    }
    /// return number of mines 
    return adjmine;
}

///Returns true if there are no mines adjacent to space
bool noMine(Settings * mineFld, int row, int col) {
    if (proxM(mineFld, row, col))
        /// there was at least one mine adjacent
        return false; 
     /// area was clear
    return true; 
}

///clear and area whose values are clear
void revealZ(Settings *mineFld, int row, int col) {
    /// check bounds
    if (row >= mineFld->rows || row < 0 || col >= mineFld->columns || col < 0)
        return;
    if (noMine(mineFld, row, col) && mineFld->field[row][col] != Settings::NOMINE) {
        mineFld->field[row][col] = Settings::NOMINE;
        /// go up one row
        revealZ(mineFld, row + 1, col);
        /// go down one row
        revealZ(mineFld, row - 1, col);
        /// go right one col
        revealZ(mineFld, row, col + 1);
        /// go left one col
        revealZ(mineFld, row, col - 1);
    }        /// space was not clear or already shown
    else
        return;
}

/// Function shows how many mines are adjacent to selected square
/// for the entire minefield
void flagSet(Settings *mineFld) {
    for (int i = 0; i != mineFld->rows; ++i)
        for (int j = 0; j != mineFld->columns; ++j)
            /// don't look for adjacent mines in areas where
            /// mine is already located
            if (mineFld->field[i][j] != Settings::MINE)
                mineFld->field[i][j] = proxM(mineFld, i, j);
}

/// Reveals what is under space
///If mine, you lose the game..If no mine, game resumes
bool resume(Settings * mineFld, int row, int col) {
    /// check if user selected a losing square
    if (mineFld->field[row][col] == Settings::MINE)
        return false;

        /// Square is a zero, clear the surrounding area if necessary
    else if (noMine(mineFld, row, col)) {
        revealZ(mineFld, row, col); /// show cleared area
        ckborder(mineFld);
        hideMines(mineFld);
        return true;
    }        /// Square had adjacent mine
        /// reveal the number to the user
    else {
        mineFld->field[row][col] = proxM(mineFld, row, col);
        hideMines(mineFld);
        return true;
    }
}

/// Function checks whether the player has won
bool winCase(Settings *mineFld) {
    for (int i = 0; i != mineFld->rows; ++i)
        for (int j = 0; j != mineFld->columns; ++j)
            /// if there are empty spaces player has not won
            if (mineFld->field[i][j] == Settings::NONE)
                return false;
    /// there were no empty spaces left. Player has won
    return true;
}

/// Function find the perimeter of the cleared areas
void ckborder(Settings *mineFld) {
    for (int row = 0; row != mineFld->rows; ++row) {
        /// avoid search at left and right edge of array
        for (int col = 0; col != mineFld->columns; ++col) {
            /// when you're not on the bounds of the array
            if (row > 0 && row < mineFld->rows - 1
                    && col > 0 && col < mineFld->columns - 1)
                if (mineFld->field[row][col] == Settings::NOMINE) {
                    /// check that the previous number has mines adjacent
                    if (mineFld->field[row][col - 1] != Settings::NOMINE)
                        mineFld->field[row][col - 1] = proxM(mineFld, row, col - 1);
                    /// check if the next number has mines adjacent
                    if (mineFld->field[row][col + 1] != Settings::NOMINE)
                        mineFld->field[row][col + 1] = proxM(mineFld, row, col + 1);
                    if (mineFld->field[row - 1][col] != Settings::NOMINE)
                        mineFld->field[row - 1][col] = proxM(mineFld, row - 1, col);
                    /// check if the next number has mines adjacent
                    if (mineFld->field[row + 1][col] != Settings::NOMINE)
                        mineFld->field[row + 1][col] = proxM(mineFld, row + 1, col);
                    /// check the adjacent corners
                    if (mineFld->field[row + 1][col - 1] != Settings::NOMINE)
                        mineFld->field[row - 1][col - 1] = proxM(mineFld, row - 1, col - 1);
                    if (mineFld->field[row - 1][col + 1] != Settings::NOMINE)
                        mineFld->field[row - 1][col + 1] = proxM(mineFld, row - 1, col + 1);
                    if (mineFld->field[row + 1][col - 1] != Settings::NOMINE)
                        mineFld->field[row + 1][col - 1] = proxM(mineFld, row + 1, col - 1);
                    if (mineFld->field[row + 1][col + 1] != Settings::NOMINE)
                        mineFld->field[row + 1][col + 1] = proxM(mineFld, row + 1, col + 1);
                }
        }
    }
}

///Function that displays the rules of the game
void instructions(){
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
    cout << "    " <<endl;
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

//character array with strings to satisfy rubric
char *playerN() {
    cout << "Enter your name: ";
    string in;
    cin >> in;
   
    short size = in.size();
    char *name = new char[size+1];
    for (short i = 0; i != size; ++i) {
        *(name+i) = in[i];
    }
    *(name+size+1) = '\0';
    
    return name;
}

///Writes Settings structure to binary file
void writeBinary(Settings *mineFld, string fileName) {
    fstream out(fileName.c_str(), ios::out | ios::binary);    /// Open the file
    out.write(reinterpret_cast<char *>(&mineFld),sizeof(*mineFld)); ///Write to the file
    out.close();
}

///Reads from Binary File
void readFile(string fileName) {
    char response;
    cout << "Would you like to view a previous game from a binary file?" << endl;
    cout << "Hit 'y' if yes: " << endl;
    cin >> response;
    if (response == 'y') {
        cout << "     " << endl;
        cout << "Results from the last game:" << endl;
        /// Create space to hold the file read
        Settings *result;
        fstream in(fileName.c_str(), ios::in | ios::binary);
        in.read(reinterpret_cast<char *>(&result), sizeof(*result));
        revealM(result);
        in.close();
    }

}