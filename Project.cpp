#include <iostream>
#include "MacUILib.h"
#include "objPos.h"

// is this legal?

#include "GameMechs.h"
#include "Player.h"


using namespace std;

#define DELAY_CONST 100000

GameMechs *mechs; // Should be deleted one day
Player *player; // Should also go away
Food *pFood; // We don't like globals >:(

void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);



int main(void)
{
    Initialize();

    while(mechs->getExitFlagStatus() == false)  
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();

}


void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();   
    

    mechs = new GameMechs();
    pFood = new Food();
    player = new Player(mechs, pFood);
    

    pFood->generateFood(player->getPlayerPos(), *mechs);
}

void GetInput(void)
{
   if (MacUILib_hasChar())
   {
    mechs->setInput(MacUILib_getChar());
   }
}

void RunLogic(void)
{
    if (mechs->getInput() != 0)
    {
        player->updatePlayerDir();
    }
    player->movePlayer();

    player->decrementStatusEffects(DELAY_CONST);    // if any status effects, decrease them by the appropriate # of microseconds
}

void DrawScreen(void)
{
    MacUILib_clearScreen();
    
    int i,j,boardX,boardY;
    boardX=mechs->getBoardSizeX();
    boardY=mechs->getBoardSizeY();
    
    /*
    // Original implementation, without Board class:

    bool foodFlag = false; // simplifies implementation

    for (i=0; i<boardY; i++)
    {
        for (j=0; j<boardX; j++)
        {
            if (j==0||i==0||j+1==boardX||i+1==boardY)
            {
                MacUILib_printf("%c", '#');
            }
            else
            {
                int k, snakeLength;
                snakeLength = player->getPlayerPos()->getSize();

                foodFlag = false;

                for (int k = 0; k< FOODS; k++)
                {
                    if(pFood->getFoodPos(k).pos->x == j && pFood->getFoodPos(k).pos->y == i)
                    {
                        MacUILib_printf("%c", pFood->getFoodPos(k).symbol);
                        foodFlag = true;
                        break;
                    }
                }

                for(k=0; k<snakeLength; k++)
                {
                    if (player->getPlayerPos()->getElement(k).pos->x==j && player->getPlayerPos()->getElement(k).pos->y==i)
                    {
                        MacUILib_printf("%c", player->getPlayerPos()->getElement(k).symbol);
                        break;
                    }
                }
                if (k==snakeLength && !foodFlag) MacUILib_printf("%c", ' ');
            }
        }
        MacUILib_printf("\n"); // Vertical spacing
    }

    // original implementation ends here

    */
    
    
    // new modularization starts here

    

    // clear board and write snake + food to it
    mechs->getBoard()->wipeInterior();
    mechs->getBoard()->writeToBoard(*pFood->getFoodBucket());

    if (player->getBlindTime() == 0)
    {
        mechs->getBoard()->drawBorder();    // Ensures the border is at its regular state, which by default is '#'
        mechs->getBoard()->writeToBoard(*player->getPlayerPos());
    }

    else    // if we are blind we will only see the head element
    {
        mechs->getBoard()->writeToBoard(player->getPlayerPos()->getHeadElement());
        mechs->getBoard()->drawBorder(' ');  // the border is invisible
    }

    // Now we print the entire board from the canvas
    for (i=0; i<boardY; i++)
    {
        for (j=0; j<boardX; j++)
        {
            MacUILib_printf("%c", mechs->getBoard()->getData(j,i));  // the character in question
        }
        MacUILib_printf("\n");
    }
    // New modularization ends here... the following logic belongs to both pathways

    MacUILib_printf("Score: %d", mechs->getScore());
    MacUILib_printf("\nLives: %d", player->getLives());
    
    // Report on how much longer you are dizzy, if necessary
    // converted from microseconds to seconds, also rounded up
    if (player->getDizzyTime() != 0) 
    {
        MacUILib_printf("\nDizzy for %d seconds!", 1+player->getDizzyTime()/1000000);      
    }

    // Report on how much longer you are blind, if necessary
    // converted from microseconds to seconds, also rounded up
    if (player->getBlindTime() != 0)
    {
        MacUILib_printf("\nBlind for %d seconds!", 1+player->getBlindTime()/1000000);
    }


    //MacUILib_printf("Player X: %d", player->getPlayerPos().pos->x);     // debug
}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST/mechs->getSpeedMultiplier()); // 0.1s delay, will change if you eat '^'
    // The reason we use / with a "multiplier" is that decreasing (dividing) the delay will increase (multiply) the speed
}


void CleanUp(void)
{
    MacUILib_clearScreen();
    if(mechs->getLoseFlagStatus() == true) 
    {
        MacUILib_printf("You lose!\n");
    }
    else if (player->getPlayerPos()->atCapacity())
        {
            MacUILib_printf("You win! The snake has become so large that it cannot physically move.\n");
            MacUILib_printf("For the first time since its creation in 1976, the snake from Snake can finally rest peacefully.\n");
            MacUILib_printf("If you made it this far, we should have set the array capacity higher :)\n");
        }
    
    else MacUILib_printf("Terminated successfully!\n");
    MacUILib_printf("Your final score: %d", mechs->getScore());

    delete player;
    
    
    delete pFood;
    

    delete mechs;
    
    
    MacUILib_uninit();      
    
}
