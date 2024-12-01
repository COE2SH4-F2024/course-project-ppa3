#include <iostream>
#include "MacUILib.h"
#include "objPos.h"

// is this legal?

#include "GameMechs.h"
#include "Player.h"


using namespace std;

#define DELAY_CONST 100000
#define FOODS 5

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
}

void DrawScreen(void)
{
    MacUILib_clearScreen();  
    
    int i,j,boardX,boardY;
    boardX=mechs->getBoardSizeX();
    boardY=mechs->getBoardSizeY();
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
    MacUILib_printf("Score: %d", mechs->getScore());
    MacUILib_printf("\nLives: %d", player->getLives());
    //MacUILib_printf("Player X: %d", player->getPlayerPos().pos->x);     // debug
}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}


void CleanUp(void)
{
    MacUILib_clearScreen();
    if(mechs->getLoseFlagStatus() == true) {MacUILib_printf("You lose!");}
    else {MacUILib_printf("Terminated successfully!");}

    delete player;
    
    
    delete pFood;
    

    delete mechs;
    MacUILib_printf("May i uninit? Yes");
    
    MacUILib_uninit();      // Not firing, for some reason
    
}
