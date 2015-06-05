#ifndef BoardHeader
#define BoardHeader

#include "Abstract.h"

class Matrix: public Abstract{
private:
    
protected:
    ///Holds the game's data
    int **field;
    ///Rows on minefield
    int rows;
    ///columns on minefield
    int cols;
    ///Used to create the gameboard
    virtual void create(int, int);

public:
    /// Throw this if user tries to set negative row or column
    class tooSmall{};
    
    Matrix(int rows, int cols) {create(rows,cols);wipeB();}
    virtual ~Matrix(){destroy();}
    virtual void destroy();
    virtual void vRows(int);
    virtual void vColmn(int);
    virtual int retRows() const {return rows;}
    virtual int getCols() const {return cols;}
    virtual void wipeB();
    virtual void config();
    virtual void loadGame();
    virtual void print() const;
    int* operator[](int index) { return field[index];}
    int* operator[](int index) const { return field[index];}
};

#endif