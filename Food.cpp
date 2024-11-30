#include "Food.h"
#include "objPos.h"
#include "GameMechs.h"  // We NEED to check that we're allowed to do this.
#include <stdlib.h>
#include <time.h>


Food::Food()
{
    // What to do here?
}

Food::~Food()
{
    // What to do here?
}

void Food::generateFood(objPos blockOff, GameMechs mechs)
{
    /*
    HEAR ME OUT
    Manual does not instruct us to include GameMechs here BUT this is the above and beyond design...
    The original design intended for generateFood to be able to access gameMechs, because boardX, boardY are necessary info
    So it makes sense to include either gameMechs or 2 int parameters
    If its illegal, the bonus marks will likely take some of the heat for us :)
    Lets try to find out if this is allowed
    */

    foodPos.pos->x = -10;
    foodPos.pos->y = -10;

    srand(time(NULL));
    int boardX = mechs.getBoardSizeX();
    int boardY = mechs.getBoardSizeY();
    int randX, randY, flag;
    char randS;
    
    do 
    {
        randX=rand()%(boardX-1);
        randY=rand()%(boardY-1);
        flag = randX && randY;  // If either of these is 0 it's in the border, so we go again
        if ((randX == blockOff.pos->x) && (randY == blockOff.pos->y))
        {
            flag = 0;   // if the blocked-off coordinate (i.e. the player's location) == the chosen coordinate, try again
        }
    } while (flag == 0);

    do
    {
        flag = 1;
        randS = 33+(rand()%94);   // i.e. [33, 126], alphanumeric but not spacebar

        if (randS == blockOff.getSymbol()   ||  randS == '#')   // Do we need to hardcode #? Is there a better approach?
        {
            flag = 0;
        }
    } while (flag == 0);

    foodPos.setObjPos(randX,randY,randS);
}

objPos const Food::getFoodPos()
{
    return foodPos;
}