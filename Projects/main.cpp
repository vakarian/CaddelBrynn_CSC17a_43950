/* 
 * File:   main.cpp
 * Author: Brynn Caddel
 * Created on May 1, 2015, 3:35 PM
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>
#include "Settings.h"


struct MineField {
    short **data;
    short rows;
    short cols;
    short mines;
};

struct Settings {
    Difficulty diff1;
};

using namespace std;

//Function Prototypes
MineField *create(short,short);
void prompt(short&, Settings);
Difficulty shortToDiff(short);

int main(int argc, char** argv) {
    //Get the user's name
    string userName;
    cout << "Welcome to MineSweeper. Unlike the title suggests, we are not "
            "actually trying to sweep up mines in this game. If you're "
            "interested in playing a game of Minesweeper, please press 'y'.";
    char ans;
    cin >> ans;
    
    if (ans == 'y'){
        short nrows;
        Settings x;
        prompt(nrows, x);
    }
    return 0;
}

//Functions
void prompt(short &matrix, Settings &x){
    //User enters size of the board
    cout << "Your MineSweeper board will be of size nxn."
            "Enter the value of n:";
    cin >> matrix;
    //User enters their difficulty level
    short diff;
    cout << "Difficulty levels:"
            "| Easy = 0 | Normal = 1 | Hard = 2 |"
            "Please enter your difficulty level: ";
    cin >> diff;
}

Difficulty shortToDiff(short choice){
    switch(choice) {
        case(0):
            return EASY;
            break;
        case(1):
            return NORMAL;
            break;
        case(2):
            return HARD;
            break;
        default:
            return NORMAL;
            break;
    }
}