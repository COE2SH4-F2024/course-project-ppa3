#include "Food.h"
#include "objPos.h"
#include "GameMechs.h"  // We NEED to check that we're allowed to do this.
#include <stdlib.h>
#include <time.h>


Food::Food()
{
    // What to do here?
    foodBucket = new objPosArrayList();
}

Food::~Food()
{
    // What to do here?
    delete foodBucket;
}

void Food::generateFood(objPosArrayList* blockOff, GameMechs mechs)
{
    /*
    HEAR ME OUT
    Manual does not instruct us to include GameMechs here BUT this is the above and beyond design...
    The original design intended for generateFood to be able to access gameMechs, because boardX, boardY are necessary info
    So it makes sense to include either gameMechs or 2 int parameters
    If its illegal, the bonus marks will likely take some of the heat for us :)
    Lets try to find out if this is allowed
    */

   /*
   The implementation differs from the manual, but I decided to keep foodPos to use as a temporary objPos instance
   when generating the new positions
   */

    foodPos.pos->x = -10;
    foodPos.pos->y = -10;

    for(int i = 0; i < foodBucket->getSize() + 1; i++)
    {
        delete foodBucket;

        foodBucket = new objPosArrayList();
    }

    srand(time(NULL));
    int boardX = mechs.getBoardSizeX();
    int boardY = mechs.getBoardSizeY();
    int randX, randY, flag;
    char randS;

    for(int i = 0; i < 5; i++)
    {
        flag = 0;
        while (flag == 0)
        {
            randX=rand()%(boardX-1);
            randY=rand()%(boardY-1);
            if(i < 3)
            {
                randS = 49; 
            }
            else
            {
                randS = 50 + rand()%(2);
            }
            
            if (!(randX && randY)) continue;  // If either of these is 0 it's in the border, so we go again

            if (randS == '*' || randS == ' ' || randS == '#') continue;

            foodPos.setObjPos(randX, randY, randS); // This will be changed if something's wrong

            int k, snakeLength;
            snakeLength = blockOff->getSize();

            flag = 1;

            for (k=0; k<snakeLength; k++) 
            {
                if (blockOff->getElement(k).isPosEqual(&foodPos))
                {
                    flag = 0;
                    break;
                }
            }
            if (flag == 1)
            {
                for (int k = 0; k < i; k++)
                {
                    if (foodBucket->getElement(k).isPosEqual(&foodPos))
                    {
                        flag = 1;
                    }
                }
            }

            if (flag == 1)
            {
                foodBucket->insertTail(foodPos);
            }
        }
    }
}

objPos const Food::getFoodPos()
{
    return foodPos;
}

objPos const Food::getFoodPos(int index)
{
    return foodBucket->getElement(index).getObjPos();
}