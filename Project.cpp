#include "MacUILib.h"
#include "objPos.h"

// is this legal?

#include "GameMechs.h"
#include "Player.h"


#define DELAY_CONST 100000     // default 100000

// We need some pointers in global scope... no way around this, as the routines return nothing
GameMechs *mechs; 
Player *player;
Food *pFood;

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
    
    // Populate the board with food. Will refresh when you eat something
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

void DrawScreen(void)       // if you dont understand this i recommend reading Board.cpp and objPosArrayList.cpp for more
{
    MacUILib_clearScreen();
    
    int i,j,boardX,boardY;
    boardX=mechs->getBoardSizeX();
    boardY=mechs->getBoardSizeY();
    
    
    // Summary of why we use Board class: more time-efficient for long snakes, and makes draw routine clearer
    // Acts more or less as a canvas that we draw on, then print from
    // For more detailed justification read Board.cpp

    // clear board and write food to it
    mechs->getBoard()->clearInterior();
    mechs->getBoard()->writeToBoard(*pFood->getFoodBucket());

    // write player positions (whole snake) to the board. Only write head if we are blind from eating '!'
    if (player->getBlindTime() == 0)
    {
        mechs->getBoard()->drawBorder();    // set border to defauls
        mechs->getBoard()->writeToBoard(*player->getPlayerPos());
    }

    else    // if we are blind we will only see the head element
    {
        mechs->getBoard()->writeToBoard(player->getPlayerPos()->getHeadElement());
        mechs->getBoard()->drawBorder(' ');  // the border is invisible
    }

    // iterate through and print the entire board
    for (i=0; i<boardY; i++)
    {
        for (j=0; j<boardX; j++)
        {
            MacUILib_printf("%c", mechs->getBoard()->getData(j,i));  // the character to print
        }
        MacUILib_printf("\n");  // newline after each row
    }

    // Report score and lives
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

}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST/mechs->getSpeedMultiplier()); // 0.1s delay, will change if you eat '^'
    // The reason we use / with a "multiplier" is that decreasing (dividing) the delay will increase (multiply) the speed
}


void CleanUp(void)
{
    MacUILib_clearScreen();
    if(mechs->getLoseFlagStatus() == true)      // This happens if you lose all lives or (in debug controls) use loseKey
    {
        MacUILib_printf("You lose!\n");
    }

    else if (player->getPlayerPos()->atCapacity())      // included this branch to avoid motion issues at capacity
    {
        MacUILib_printf("You win! The snake has become so large that it cannot physically move.\n");
        MacUILib_printf("For the first time since its creation in 1976, the snake from Snake can finally rest peacefully.\n");
        MacUILib_printf("If you made it this far, we should have set the array capacity higher :)\n");
    }
    
    else MacUILib_printf("Terminated successfully!\n");     // This is exclusive to the escape key

    MacUILib_printf("Your final score: %d", mechs->getScore());     // Prints the final score


    delete player;    
    delete pFood;
    delete mechs;
    
    
    MacUILib_uninit();      
    
}
