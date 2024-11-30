#include <iostream>
#include "MacUILib.h"
#include "objPos.h"

// is this legal?

#include "GameMechs.h"
#include "Player.h"
#include "Food.h"

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
    //  debug: cout << "hello!";
    player = new Player(mechs);
    pFood = new Food;

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
        // Temporary food debug logic
        if (mechs->getInput() == 'g')
        {
            pFood->generateFood(player->getPlayerPos(), *mechs);
        }
        
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

    for (i=0; i<boardY; i++)
    {
        for (j=0; j<boardX; j++)
        {
            if (j==0||i==0||j+1==boardX||i+1==boardY)
            {
                MacUILib_printf("%c", '#');
                // MacUILib_printf("%d", mechs->getBoardSizeY()); //debug
            }
            else if (player->getPlayerPos().pos->x==j && player->getPlayerPos().pos->y==i)
            {
                MacUILib_printf("%c", player->getPlayerPos().symbol);
            }
            else if (pFood->getFoodPos().pos->x==j && pFood->getFoodPos().pos->y==i)
            {
                MacUILib_printf("%c", pFood->getFoodPos().symbol);
            }
            else {MacUILib_printf("%c", ' ');}
        }
        MacUILib_printf("\n"); // Vertical spacing
    }
    MacUILib_printf("Score: %d", mechs->getScore());
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
    delete mechs;
    delete pFood;
    MacUILib_uninit();
}
