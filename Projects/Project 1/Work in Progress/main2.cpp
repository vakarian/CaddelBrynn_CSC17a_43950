//System Libraries
#include <cstdlib>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

//Data Structures
struct Settings {
    /// Determines how many mines to set
    enum Difficulty {BEGINNER, INTERMEDIATE, EXPERT};
    /// Flags representing various square possibilities
    enum Flags {EMPTY=10, MINE, CLEAR, LOSER};
    /// This is the minefield
    short **field;
    /// The total number of rows
    short rows;
    /// The total number of columns
    short columns;
    /// number of mines
    short mines;
};

using namespace std;

//Function Prototypes
Settings *create(short, short);
void destroy(Settings *);   //Deallocates dynamic memory
void revealM(Settings *);   //Reveals location of mines
void hideMines(Settings *); //Function that hides location of mines
Settings::Difficulty convert(short);
bool check(short, short, Settings::Difficulty);
short nMines(Settings::Difficulty);
void setMines(Settings *);
void setFlags(Settings *);
short proxM(Settings *, short, short, short = Settings::MINE);
bool noMine(Settings *, short, short);
void clrArea(Settings *, short, short);
void setPerim(Settings *);
void showZeros(Settings *, short, short);
bool hasWon(Settings *);
bool cont(Settings *, short, short);
void playMS(short, short, Settings::Difficulty);
void userIn(short&, Settings::Difficulty&);
char *userName();
void writeBin(Settings *, string);
void readBin(string);

//Execution Begins Here
int main(int argc, const char * argv[]) {
    cout << "Welcome to MineSweeper!"       <<endl;
    cout << "Press 'y' to continue." <<endl;
    char ans;
    cin >> ans;
    
   //This allows the user to choose the size of their minefield and the
   //difficulty level of the game
    if (ans == 'y') {
        //Creates the variables in the game: matrix size and difficulty
        short nrows;
        Settings::Difficulty userSet;
        //This function collects the number of rows and difficulty and stores
        //it in an array
        userIn(nrows, userSet);
        //Error Checking: checks validity of user input
        //If valid, then it creates the array
        if (check(nrows, nrows, userSet)) {
            while (ans == 'y' && check(nrows, nrows, userSet)) {
                playMS(nrows, nrows, userSet);
                cout << endl;
                cout << "Would you like to play again?";
                cin >> ans;
                cout << endl;
                //User has the option to play again
                if (ans =='y')
                    userIn(nrows, userSet);
            }
        }
        //Else, the program exits
        else
            cout << "Error: Minefield too small."
                    "The size of the minefield is not compatible with the"
                    "difficulty level. Please try a larger minefield if you're"
                    "playing on Difficulty:EXPERT";
    }
    cout << "Game Over.";
    
    readBin("result");
    
    return 0;
}

//Function Definitions
void userIn(short &rows, Settings::Difficulty &userSet) {
    short dLevel;
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
    cout << "Your MineSweeper board will be of size nxn." << endl;
    cout << "Example: When n=4" << endl;
    cout << "   1 2 3 4 " << endl;
    cout << " 1 x x x x " << endl;
    cout << " 2 x x x x " << endl;
    cout << " 3 x x x x " << endl;
    cout << " 4 x x x x " << endl;
    cout << "Enter the value of n: ";
    cin >> rows;
    cout << "                 " << endl;
}

/// Function returns true if input is valid
bool check(short rows, short cols, Settings::Difficulty dLevel) {
    //Makes sure that the number of mines does not exceed spots in
    //the minefield
    return (rows * cols) > nMines(dLevel);
}

//Function allows user to enter rows, columns, and difficulty
void playMS(short nrows, short ncols, Settings::Difficulty dLevel) {
    srand(static_cast<unsigned int>(time(0)));
    Settings *mineFld = create(nrows, ncols);
    mineFld->mines=nMines(dLevel);
    setMines(mineFld); 
    hideMines(mineFld);
    short row, col;
    do {
        /// Select the row
        do {
            cout << "Enter the row: ";
            cin >> row;
        //Error checking: checks validity of bounds for rows
        } while (row < 0 || row >= mineFld->rows);
        do {
            cout << "Enter the column: ";
            cin >> col;
        //Error Checking: checks validity of bounds for columns
        } while (col < 0 || col >= mineFld->columns);
        cout << endl;
    } while (cont(mineFld, row, col) && !hasWon(mineFld));
    
    //Once the game has either been won or lost
    //Win Case
    if (hasWon(mineFld)) {
        cout << "You win! The Special Forces will now contact you,"  << endl;
        cout << " as your skill in cleverly evading mines may prove" << endl;
        cout << " to be useful out on the field."                    << endl;
        setFlags(mineFld);
    }
    //Lose Case
    else{
        cout << "*boom* You have lost the game..." << endl;
        setFlags(mineFld);
        mineFld->field[row][col]= Settings::LOSER;
    }
    /// Print the complete minefield
    revealM(mineFld);
    
    /// write result to binary file
    writeBin(mineFld, "result");
    /// deallocate the game area
    destroy(mineFld);
}

//Function generates the grid the game is to be played on
Settings* create(short rows, short cols) {
    //Dynamically allocating memory for minefield
    Settings *out = new Settings;
    out->rows=rows;
    out->columns = cols;
    
    //Creates 2D playing field
    out->field = new short *[rows];
    
    //Creates each new row
    for (short row = 0; row != rows; ++row)
        out->field[row] = new short [cols];
    
    //Ensures that each space is empty
    for (short i = 0; i != rows; ++i)
        for (short j = 0; j != rows; ++j)
            out->field[i][j] = Settings::EMPTY;
    return out;
}

//This function takes the short variable and returns it as Settings::Difficulty
Settings::Difficulty convert(short choice) {
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

//Deallocating memory function
void destroy(Settings *mineFld) {
    //This deletes each row that was dynamically allocated
    for (short i = 0; i != mineFld->rows; ++i)
        delete[] mineFld->field[i];
    //This deletes the dynamically allocated structure
    delete mineFld;
}

//Once the game is won/lost, this function prints the location of the mines
//revealed
void revealM(Settings* mineFld) {
    for (short row = 0; row != mineFld->rows; ++row){
        for (short col = 0; col != mineFld->columns; ++col) {
            ///
            if ( *(*(mineFld->field+row) + col) == Settings::LOSER)
                cout << "#  ";
            else if (*(*(mineFld->field+row) + col) == Settings::MINE)
                cout << "x  ";
            else if (!noMine(mineFld, row, col))
                     cout << proxM(mineFld, row, col) << "  ";
            else
                cout << "0  ";
        }
        cout << endl;
    }
    cout << endl;
}

//This function prints the field with mines hidden
void hideMines(Settings* mineFld) {
    //Print the column index
    for (short i = 0; i != mineFld->columns; ++i){
        //Makes spacing look nice
        if (i==0)
            cout << "  ";
        cout << setw(3) << i;
    }
    cout << endl;
    for (short row = 0; row != mineFld->rows; ++row){
        for (short col = 0; col != mineFld->columns; ++col){
            if(col == 0 && row < 10) cout << row << "  ";
            if (col == 0 && row >= 10) cout << row << " ";
            /// KEEP EMPTY spaces and MINEs hidden
            if (mineFld->field[row][col] == Settings::EMPTY ||
                mineFld->field[row][col] == Settings::MINE)
                cout << setw(3) << right  << "* ";
            /// print out the CLEARed area
            else if (mineFld->field[row][col] == Settings::CLEAR)
                cout << setw(2)<< 0 << " ";
            /// Print out the actual value of the square
            else
                cout << setw(2)<< mineFld->field[row][col] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//There are a varying number of mines laid out depending on the user inputted
//difficulty level
short nMines(Settings::Difficulty userSet) {
    if (userSet==Settings::BEGINNER)
        return 15;
    else if (userSet==Settings::INTERMEDIATE)
        return 30;
    else
         return 45;
}

//Function randomly places mines in the field
void setMines(Settings *mineFld) {
    //Determines how many mines will be used
    short mines = mineFld->mines;
    
    //Continues looping through mines until they're all laid out
    while (mines) {
        for (short i = 0; i != mineFld->rows; ++i) {
            for (short j = 0; j != mineFld->columns; ++j) {
                /// place mines if result of rand()%15 == 0
                if ((rand() % 100) % 10 == 0){
                    ///only place mines if mines are still available
                    /// and current is empty
                    if (mines && mineFld->field[i][j] == Settings::EMPTY) {
                        /// set the mine
                        mineFld->field[i][j] = Settings::MINE;
                        --mines;
                    }
                }
            }
        }
    }
}

// Function returns how  many 'flag' elements surround a given square
short proxM(Settings *mineFld, short row, short col, short FLAG) {
    short adjmine=0;              /// the number of adjacent mines
    
    /// not on first or last row or first or last column
    /// most of the searches take place in this area
    if ( row > 0 && col > 0 && row < mineFld->rows-1 && col < mineFld->columns-1) {
        /// search the 3x3 grid surrounding a cell
        for (short i = row-1; i <= row+1; ++i) {
            for (short j = col-1; j <= col+1; ++j)
                if (mineFld->field[i][j] == FLAG)
                    ++adjmine;
        }
    }
    /// on the first row, not on first or last column
    else if ( row == 0 && col > 0 && col < mineFld->columns - 1) {
        for (short i = row; i <= row+1; ++i) {
            for (short j = col-1; j <= col+1; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }
    /// on the last row, not on first or last column
    else if ( row == mineFld->rows-1 && col > 0 && col < mineFld->columns - 1) {
        for (short i = row-1; i <= row; ++i) {
            for (short j = col-1; j <= col+1; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }
    /// on the first column, not on first or last row
    /// search to the right
    else if ( col == 0 && row > 0 && row < mineFld->rows - 1) {
        for (short i = row-1; i <= row+1; ++i) {
            for (short j = col; j <= col+1; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }
    /// on the last column, not on first or last row
    /// search to the left
    else if ( col == mineFld->columns-1 && row > 0 && row < mineFld->rows - 1) {
        for (short i = row-1; i <= row+1; ++i) {
            for (short j = col-1; j <= col; ++j)
                if (mineFld->field[i][j] == Settings::MINE)
                    ++adjmine;
        }
    }
    /// top left corner
    else if (row == 0 && col == 0) {
        if (mineFld->field[row][col+1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row+1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row+1][col+1] == Settings::MINE) ++adjmine;
    }
    /// top right corner
    else if (row == 0 && col == mineFld->columns-1) {
        if (mineFld->field[row][col-1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row+1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row+1][col-1] == Settings::MINE) ++adjmine;
    }
    /// bottom left corner
    else if (row == mineFld->rows-1 && col == 0) {
        if (mineFld->field[row-1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row-1][col+1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row][col+1] == Settings::MINE) ++adjmine;
    }
    /// bottom right corner
    else if (row == mineFld->rows-1 && col == mineFld->columns-1) {
        if (mineFld->field[row-1][col-1] == Settings::MINE) ++adjmine;
        if (mineFld->field[row-1][col] == Settings::MINE) ++adjmine;
        if (mineFld->field[row][col-1] == Settings::MINE) ++adjmine;
    }
    /// return number of mines from appropriate if statement
    return adjmine;
}

/// Function is true if there 0 landmines adjacent to
/// selected square
bool noMine(Settings * mineFld, short row, short col) {
    if (proxM(mineFld, row, col))
        return false;            /// there was at least one mine adjacent
    return true;                 /// area was clear
}

/// Clear an area whose values are clear
/// i.e 0 adjacent  mines
void showZeros(Settings *mineFld, short row, short col) {
    /// check bounds
    if ( row >= mineFld->rows || row < 0 || col >= mineFld->columns || col < 0)
        return;
    if (noMine(mineFld, row, col) && mineFld->field[row][col] != Settings::CLEAR){
        mineFld->field[row][col] = Settings::CLEAR;
        /// go up one row
        showZeros(mineFld, row+1, col);
        /// go down one row
        showZeros(mineFld, row-1, col);
        /// go right one col
        showZeros(mineFld, row, col+1);
        /// go left one col
        showZeros(mineFld, row, col-1);
    }
    /// space was not clear or already shown
    else
        return;
}

/// Function shows how many mines are adjacent to selected square
/// for the entire minefield
void setFlags(Settings *mineFld) {
    for (short i = 0; i != mineFld->rows; ++i)
        for (short j = 0; j != mineFld->columns; ++j)
            /// don't look for adjacent mines in areas where
            /// mine is already located
            if (mineFld->field[i][j] != Settings::MINE)
                mineFld->field[i][j] = proxM(mineFld, i, j);
}

/// Function reveals what is underneath the square that the user has selected
/// and whether to continue based on what is revealed
/// i.e selecting a mine means you lost, game over
bool cont(Settings * mineFld, short row, short col) {
    /// check if user selected a losing square
    if (mineFld->field[row][col] == Settings::MINE)
        return false;

    /// Square is a zero, clear the surrounding area if necessary
    else if (noMine(mineFld, row, col) ){
        showZeros(mineFld, row, col); /// show cleared area
        setPerim(mineFld);
        hideMines(mineFld);
        return true;
    }
    /// Square had adjacent mine
    /// reveal the number to the user
    else {
        mineFld->field[row][col] = proxM(mineFld, row, col);
        hideMines(mineFld);
        return true;
    }
}

/// Function checks whether the player has won
bool hasWon(Settings *mineFld) {
        for (short i = 0; i != mineFld->rows; ++i)
            for (short j = 0; j != mineFld->columns; ++j)
                /// if there are empty spaces player has not won
                if (mineFld->field[i][j] == Settings::EMPTY)
                    return false;
        /// there were no empty spaces left. Player has won
        return true;
    }

/// Function find the perimeter of the cleared areas
void setPerim(Settings *mineFld) {
    for (short row = 0; row != mineFld->rows; ++row ) {
        /// avoid search at left and right edge of array
        for (short col = 0; col != mineFld->columns; ++col) {
            /// when you're not on the bounds of the array
            if (row > 0 && row < mineFld->rows-1
                && col > 0 &&  col <mineFld->columns-1)
                if (mineFld->field[row][col] == Settings::CLEAR) {
                    /// check that the previous number has mines adjacent
                    if (mineFld->field[row][col-1] != Settings::CLEAR)
                        mineFld->field[row][col-1] = proxM(mineFld, row, col-1);
                    /// check if the next number has mines adjacent
                    if (mineFld->field[row][col+1] != Settings::CLEAR)
                        mineFld->field[row][col+1] = proxM(mineFld,row, col+1);
                    if (mineFld->field[row-1][col] != Settings::CLEAR)
                        mineFld->field[row-1][col] = proxM(mineFld, row-1, col);
                    /// check if the next number has mines adjacent
                    if (mineFld->field[row+1][col] != Settings::CLEAR)
                        mineFld->field[row+1][col] = proxM(mineFld,row+1, col);
                    /// check the adjacent corners
                    if (mineFld->field[row+1][col-1] != Settings::CLEAR)
                        mineFld->field[row-1][col-1] = proxM(mineFld,row-1, col-1);
                    if (mineFld->field[row-1][col+1] != Settings::CLEAR)
                        mineFld->field[row-1][col+1] = proxM(mineFld,row-1, col+1);
                    if (mineFld->field[row+1][col-1] != Settings::CLEAR)
                        mineFld->field[row+1][col-1] = proxM(mineFld,row+1, col-1);
                    if (mineFld->field[row+1][col+1] != Settings::CLEAR)
                        mineFld->field[row+1][col+1] = proxM(mineFld,row+1, col+1);  
                }
        }
    }
}

/// Function writes the minefield structure to a binary file
void writeBin(Settings *mineFld, string fileName) {
    /// Write the result to a binary file
    fstream out(fileName.c_str(), ios::out | ios::binary);    /// open the file
    out.write(reinterpret_cast<char *>(&mineFld),sizeof(*mineFld)); /// write to the file
    out.close();
}

/// Function prints the data variable from the Minefield structure
/// written to a binary file
void readBin(string fileName) {
    /// Ask user if they want to see the result of the last game
    char response;
    cout << "Would you like to see the result of the last game as "
    "read from a binary file?\n"
    "Hit 'y' if yes: ";
    cin >> response;
    if (response == 'y') {
        cout << "\nResult of your last game:\n";
        /// Create space to hold the file read
        Settings *result;
        fstream in(fileName.c_str(), ios::in | ios::binary);
        in.read(reinterpret_cast<char *>(&result), sizeof(*result));
        revealM(result);
        in.close();
    }
}