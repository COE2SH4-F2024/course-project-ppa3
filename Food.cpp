#include "Food.h"
#include <stdlib.h>
#include <time.h>


Food::Food()    // Copy Constructor // Contains hard code regarding food symbols
{

    foodBucket = new objPosArrayList();
    targetBucketSize = FOOD_COUNT;  // how many foods in the bucket / on the board

    specialMenuLength = AVAILABLE_SPECIALS; // how many specials are there? In our case '+' '-' '!' '?' '^' -> 5 specials

    specials = new char[specialMenuLength];
    specials[0] = '+';  // Hard code, necessary
    specials[1] = '-';  // Ensure, for all of these, switch case in Player.cpp is matching.
    specials[2] = '!';
    specials[3] = '?';
    specials[4] = '^';
}

Food::~Food()   // Destructor
{
    delete [] specials;
    delete foodBucket;
}

void Food::generateFood(objPosArrayList* blockOff, GameMechs &mechs)
{
    foodBucket->removeAll();    // reset FoodBucket to 0 members

    srand(time(NULL));  // random seeding
    int boardX = mechs.getBoardSizeX();
    int boardY = mechs.getBoardSizeY();
    int randX, randY, flag, specialCount;
    char randS;

    for(int i = 0; i < targetBucketSize; i++)      // Populate entire food bucket
    {
        /*
        The implementation differs from the manual, but I decided to keep foodPos to use as a temporary objPos instance
        when generating the new positions
        */

        foodPos.pos->x = -10;
        foodPos.pos->y = -10;
        
        specialCount = 1 + rand() % 2;   // we want to have 1 or 2 special foods on the screen... this is '1 or 2'

        flag = 0;   // controls while loop
        while (flag == 0)
        {
            // generate random coords in a way that avoids right/down borders
            randX=rand()%(boardX-1);
            randY=rand()%(boardY-1);

            if (i < specialCount)   // different procedure for the specials
            {
                /*
                randS is a random symbol from the specials menu
                If you change the constructor this is likely where a seg fault could occur... be careful!
                */
                
                randS = specials[rand()%specialMenuLength];
            }
            else
            {
                /*
                randS is any alphanumeric character
                This could accidentally be a special. We will purposefully leave this in
                */
                randS = 33 + rand()%94;     // i.e. [33,126]
            }
            
            if (!(randX && randY)) continue;  // If either of these is 0 it's in the border, so try again

            if (randS == '*' || randS == ' ' || randS == '#') continue;     // banned symbol list

            

            foodPos.setObjPos(randX, randY, randS); // prepare food. we will discard this later if something's wrong

            if (blockOff->checkFor(foodPos) == false && foodBucket->checkFor(foodPos, i) == false)  // checks both lists for overlap
            {
                flag = 1;
            }



            int k;
            for (k=0; k<i; k++)
            {
                if (foodBucket->getElement(k).getSymbol() == randS)     // Ensure no duplicate symbols on the board ever
                {    
                    flag = 0;
                    break;
                }
            }

            if (flag == 1)  // if all tests passed we throw the food in the bucket
            {
                foodBucket->insertTail(foodPos);
            }
        }
    }
}

// self explanatory getters

objPos const Food::getFoodPos()
{
    return foodPos;
}

objPos const Food::getFoodPos(int index)
{
    return foodBucket->getElement(index).getObjPos();
}

objPosArrayList * const Food::getFoodBucket()
{
    return foodBucket;
}
