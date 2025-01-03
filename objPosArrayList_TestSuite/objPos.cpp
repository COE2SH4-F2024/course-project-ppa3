#include "objPos.h"

objPos::objPos()    // Constructor
{
    pos = new Pos;
    pos->x = 0;
    pos->y = 0;
    symbol = 0; //NULL
}

objPos::objPos(int xPos, int yPos, char sym)
{
    pos = new Pos;
    pos->x = xPos;
    pos->y = yPos;
    symbol = sym;
}

// Respect the rule of six / minimum four
// [TODO] Implement the missing special member functions to meet the minimum four rule

objPos::~objPos()   // Destructor
{
    delete pos;
}


objPos::objPos(const objPos &a) // Copy Constructor
{
    pos = new Pos;
    if (a.pos != 0)     // NOT a null ptr
    {
        pos->x = a.pos->x;
        pos->y = a.pos->y;
    }
    symbol = a.symbol;
}


objPos& objPos::operator=(const objPos &a)  // Assignment Operator Overload
{   
    if (this != &a)
    {
        this->pos->x = a.pos->x;
        this->pos->y = a.pos->y;
        this->symbol = a.symbol;
    }
    return *this;
    /*
    if (this != &a)
    {
        pos = new Pos
        pos->x = a.pos->x;
        pos->y = a.pos->y;
        symbol = a.symbol;
    }
    What's the difference?
    */
}


void objPos::setObjPos(objPos o)
{
    pos->x = o.pos->x;
    pos->y = o.pos->y;
    symbol = o.symbol;
}

void objPos::setObjPos(int xPos, int yPos, char sym)
{
    pos->x = xPos;
    pos->y = yPos;
    symbol = sym;
}

objPos objPos::getObjPos() const
{
    objPos returnPos;
    returnPos.pos->x = pos->x;
    returnPos.pos->y = pos->y;
    returnPos.symbol = symbol;
    
    return returnPos;
}

char objPos::getSymbol() const
{
    return symbol;
}

bool objPos::isPosEqual(const objPos* refPos) const
{
    return ((refPos->pos->x == pos->x) && (refPos->pos->y == pos->y));
}

char objPos::getSymbolIfPosEqual(const objPos* refPos) const
{
    if(isPosEqual(refPos))
        return symbol;
    else
        return 0;
}
