#ifndef GameData
#define GameData

#include <string>
#include "Board.h"

class Settings : public Matrix {
private:
    //Used in determining how many mines are placed on board
    enum Difficulty {
        BEGINNER, INTERMEDIATE, EXPERT
    };

    //Used to keep track of empty spaces and spaces with mines
    enum Squares {
        NONE = 10, MINE, NOMINE, LOSER
    };
    int mines;
    //Descriptions of each function can be found in Function Definitions in main
    Settings::Difficulty convert(int);
    bool check() const;
    bool cont(int, int);
    bool noMine(int, int) const;
    bool winCase() const;
    char *playerN();
    int nMines(Settings::Difficulty) const;
    void create(int, int);
    void ckborder();
    void flagSet();
    void mineSet();
    int proxM(int, int, int = Settings::MINE) const;
    void revealZ(int, int);
    void userIn();

public:
    ///Constructors and Destructors

    Settings(int row, int col) : Matrix(row, col) {
        wipeB();
    }
    ///Descriptions of each function can be found in Function Definitions in main

    int retColmn() const {
        return cols;
    }

    int retRows() const {
        return rows;
    }

    int getMines() const {
        return mines;
    }
    void config();
    void instructions();
    void loadGame();
    void run();
    void print() const;
    void revealM() const;
    void saveGame();
    void vColmn(int);
    void vRows(int);
    void wipeB();

    Settings& operator=(const Settings&);
};

#endif /* defined(__Project_2__Minesweeper__) */