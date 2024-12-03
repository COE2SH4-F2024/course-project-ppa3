#include "Board.h"


/*
Purpose of this class is to receive and store the data printed on the screen, such that
a) the blindness special effect is easier to implement
b) you don't need to index 2 lists for every cell on the board, improving performance dramatically

This improves upon the naive routine of printing each cell in sequence.

Normally, time complexity for an mxn board and snake of length L is something like O (m x n x L) not counting food
Now it's more like O (m x n) at the cost of like 500 bytes extra storage
Also the main function will look way prettier
*/

#define DEFAULT_SPACE_CHAR ' '
#define DEFAULT_BORDER_CHAR '#'

Board::Board()
{
    boardX = 30;
    boardY = 15;

    // Familiar 2D DMA array routine
    int i;
    boardData = new char*[boardX];  
    for (i=0; i<boardX; i++)    
    {
        boardData[i] = new char[boardY];
    }

    // Iterates through the whole board and sets all cells = ' '
    // Then sets the border '#'
    // Obviously I could have done this when I looped before but then it wouldnt have been as readable
    clearInterior();
    drawBorder();
}

Board::~Board()
{
    int i;
    for(i=0; i<boardX; i++)     
    {
        delete [] boardData[i];
    }
    delete [] boardData;
}

Board::Board(const Board &B)
{
    boardX = B.getBoardX();
    boardY = B.getBoardY();
    
    // Familiar 2D DMA array routine, but deep copying all elements of B
    int i,j;
    boardData = new char*[boardX];  
    for (i=0; i<boardX; i++)    
    {
        boardData[i] = new char[boardY];
        for (j=0; j<boardY; j++)
        {
            boardData[i][j] = B.getData(i,j);
        }
    }
}

Board& Board::operator=(const Board &B)
{
    boardX = B.getBoardX();
    boardY = B.getBoardY();

    // 2D Array Deep Copy
    int i,j;
    for (i=0; i<boardX; i++)    
    {
        for (j=0; j<boardY; j++)
        {
            boardData[i][j] = B.getData(i,j);
        }
    }
    
    return *this;
}

Board::Board(int x, int y)
{
    // board must be at least 4x4 with borders
    if (x >= 4)    boardX = x;
    else boardX = 30;
    if (y >= 4)    boardY = y;
    else boardY = 15;



    // Familiar 2D DMA array routine
    int i;  
    boardData = new char*[boardX];
    for (i=0; i<boardX; i++)    
    {
        boardData[i] = new char[boardY];
    }

    // Iterates through the whole board and sets all cells = ' '
    // Then sets the border '#'
    // Obviously could have done this in loop before but then it wouldnt have been as readable
    clearInterior();
    drawBorder();
}

int Board::getBoardX() const
{
    return boardX;
}

int Board::getBoardY() const
{
    return boardY;
}

char Board::getData(int x, int y) const     // get the char at a specific location
{
    return boardData[x][y];
}

// Clears all of the array except the border
void Board::clearInterior()
{
    int i,j;
    for (i=1; i<boardX-1; i++)
    {
        for (j=1; j<boardY-1; j++)
        {
            boardData[i][j] = DEFAULT_SPACE_CHAR;
        }
    }
}

void Board::drawBorder()    // reset border to borderChar
{
    char borderChar = DEFAULT_BORDER_CHAR;
    
    int i;

    // iterate through all border cells and make the change
    for(i=0; i<boardX; i++)
    {
        boardData[i][0] = borderChar;
        boardData[i][boardY-1] = borderChar;
    }

    for(i=1; i<boardY; i++)
    {
        boardData[0][i] = borderChar;
        boardData[boardX-1][i] = borderChar;
    }
}

void Board::drawBorder(char symbol)     // generic drawBorder, for expandability
{
    char borderChar = symbol;
    
    int i;

    // go through every border character
    for(i=0; i<boardX; i++)
    {
        boardData[i][0] = borderChar;
        boardData[i][boardY-1] = borderChar;
    }

    for(i=1; i<boardY; i++)
    {
        boardData[0][i] = borderChar;
        boardData[boardX-1][i] = borderChar;
    }
}

void Board::writeToBoard(objPos o)  // Makes a change to the board using the coordinates and symbol of an objPos
{
    boardData[o.pos->x][o.pos->y] = o.symbol;
}


void Board::writeToBoard(objPosArrayList opal)  // The above, repeated through a list
{
    int i, listLength;
    listLength = opal.getSize();
    
    for (i=0; i<listLength; i++)    
    {
        writeToBoard(opal.getElement(i));   // Writes the list, element by element, to the board
    }
}
// Purpose of this is to store the list before printing, instead of indexing through the list for every cell