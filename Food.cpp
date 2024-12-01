#include "Food.h"
#include "objPos.h"
#include "GameMechs.h" 
#include <stdlib.h>
#include <time.h>

#include <iostream>
using namespace std;


Food::Food()    // Contains hard code regarding food symbols
{

    foodBucket = new objPosArrayList();

    specialMenuLength = 5;

    specials = new char[specialMenuLength];
    specials[0] = '+';  // Hard code, necessary
    specials[1] = '-';  // Ensure, for all of these, switch case in Player.cpp is matching.
    specials[2] = '!';
    specials[3] = '?';
    specials[4] = '^';
}

Food::~Food()
{
    delete [] specials;
    delete foodBucket;
}

void Food::generateFood(objPosArrayList* blockOff, GameMechs mechs)
{


   

    delete foodBucket;

    foodBucket = new objPosArrayList();

    srand(time(NULL));
    int boardX = mechs.getBoardSizeX();
    int boardY = mechs.getBoardSizeY();
    int randX, randY, flag, specialCount;
    char randS;

    for(int i = 0; i < 5; i++)      // Populate food bucket
    {
        /*
        The implementation differs from the manual, but I decided to keep foodPos to use as a temporary objPos instance
        when generating the new positions
        */

        foodPos.pos->x = -10;
        foodPos.pos->y = -10;
        
        specialCount = 1 + rand() % 2;   // we want to have 1 or 2 special foods... this is '1 or 2'

        flag = 0;
        while (flag == 0)
        {
            randX=rand()%(boardX-1);
            randY=rand()%(boardY-1);
            if(i < specialCount)
            {
                /*
                randS is a random symbol from the specials menu
                If you change the constructor this is likely where a seg fault would occur... be careful!
                */
                
                randS = specials[rand()%specialMenuLength];
            }
            else
            {
                /*
                randS is any alphanumeric character
                This could accidentally be a special. We will purposefully leave this in
                */
                randS = 33 + rand()%94;
            }
            
            if (!(randX && randY)) continue;  // If either of these is 0 it's in the border, so try again

            if (randS == '*' || randS == ' ' || randS == '#') continue;     // banned symbol list

            

            foodPos.setObjPos(randX, randY, randS); // This will not be inserted if something's wrong

            if (blockOff->checkFor(foodPos) == false && foodBucket->checkFor(foodPos, i) == false)  // checks both lists for overlap
            {
                
                flag = 1;
            }

            int k;
            for (k=0; k<i; k++)
            {
                if (foodBucket->getElement(k).getSymbol() == randS)     // Ensure no duplicate symbols
                {    
                    flag = 0;
                    cout << randS;
                    cout << "\n";
                    break;
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

objPosArrayList * const Food::getFoodBucket()
{
    return foodBucket;
}
