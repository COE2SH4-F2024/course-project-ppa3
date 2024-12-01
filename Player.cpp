#include "Player.h"


Player::Player(GameMechs* thisGMRef, Food* thisFoodRef)
{
    mainGameMechsRef = thisGMRef;
    mainFoodRef = thisFoodRef;
    myDir = STOP;
    invulnerability = 0;

    // more actions to be included

    playerPosList = new objPosArrayList;

    playerPosList->insertHead(mainGameMechsRef->getBoardSizeX()/2,mainGameMechsRef->getBoardSizeY()/2,'*'); // Overload of insertHead
}


Player::~Player()
{
    // delete any heap members here
    delete playerPosList;
}

objPosArrayList* Player::getPlayerPos() const
{
    // return the reference to the playerPos arrray list
    return playerPosList;
}

void Player::updatePlayerDir()
{
        // PPA3 input processing logic

    const char shutdownKey = 27, upKey = 'w', leftKey = 'a', downKey = 's', rightKey = 'd', loseKey = 'x', scoreKey = 'z', foodKey = 'f';

    if(mainGameMechsRef->getInput() != 0)  // if not null character
    {
        switch(mainGameMechsRef->getInput())
        {                      
            case shutdownKey:  // 27 is ESC
                mainGameMechsRef->setExitTrue(); // exit
                break;
            case upKey:
                if (myDir!=DOWN) {myDir=UP;}
                break;
            case leftKey:
                if (myDir!=RIGHT) {myDir=LEFT;}
                break;
            case downKey:
                if (myDir!=UP) {myDir=DOWN;}
                break;
            case rightKey:
                if (myDir!=LEFT) {myDir=RIGHT;}
                break;
            case scoreKey:
                mainGameMechsRef->incrementScore();
                break;
            case loseKey:
                mainGameMechsRef->setLoseFlag();
                mainGameMechsRef->setExitTrue();
                break;
            case foodKey:
                mainFoodRef->generateFood(playerPosList, *mainGameMechsRef);
                break;
            default:
                break;
        }
        mainGameMechsRef->clearInput();
    }
}

void Player::movePlayer()
{
    // PPA3 Finite State Machine logic
    // [TODO] : Next, you need to update the player location by 1 unit 
    //          in the direction stored in the program

    int xWrap = mainGameMechsRef->getBoardSizeX()-2;    // These exist to minimize extra calls
    int yWrap = mainGameMechsRef->getBoardSizeY()-2;

    newPlayerPos = playerPosList->getHeadElement();

    switch(myDir)
    {
        case DOWN:
            newPlayerPos.pos->y %= yWrap; // if about to hit the border, wrap around
            newPlayerPos.pos->y++;
            break;
        case LEFT:
            newPlayerPos.pos->x--;
            if (!newPlayerPos.pos->x) {newPlayerPos.pos->x = xWrap;} // if in the border, wrap around
            break;
        case UP:
            newPlayerPos.pos->y--;
            if (!newPlayerPos.pos->y) {newPlayerPos.pos->y = yWrap;}
            break;
        case RIGHT:
            newPlayerPos.pos->x %= xWrap;
            newPlayerPos.pos->x++;
            break;
        case STOP:
            return;     // To avoid adding to the square we are already on
            break;
    }

    

    playerPosList->insertHead(newPlayerPos);

    char eatenSymbol = checkFoodConsumption();  // null if nothing eaten, else it's the character we just ate
    
    if (eatenSymbol != 0)
    {
        switch (eatenSymbol)
        {
            case '+':
                invulnerability ++;
                mainGameMechsRef->incrementScore();
                break;

            case '-':   // Shrink by 1 if big enough, but increase score by 2
                mainGameMechsRef->incrementScore(2);
                if(playerPosList->getSize() > 2)
                {
                    playerPosList->removeTail(2);
                }                    
                break;
            
            default:
                mainGameMechsRef->incrementScore();
                break;

        }
        mainFoodRef->generateFood(playerPosList, *mainGameMechsRef);    // If something was eaten regenerate all food

    }
    else playerPosList->removeTail();   // If we didn't eat anything, head grew so tail must shrink
        

        


    
    /*
    if (checkFoodConsumption())   // I envision this becoming a switch statement once we add different kinds of food
    {
        //will get rid of hard code later
        for(int i = 0; i < 5; i++)
        {
            if(mainFoodRef->getFoodPos(i).isPosEqual(&newPlayerPos))
            {
                switch (mainFoodRef->getFoodPos(i).getSymbol())
                {
                    case '1':
                        mainGameMechsRef->incrementScore();
                        break;
                    case '2':   // Shrink by 1 if large enough, but increase score by 2
                        mainGameMechsRef->incrementScore(2);
                        if(playerPosList->getSize() > 2)
                        {
                            playerPosList->removeTail(2);
                        }                    
                        break;
                    case '3':
                        invulnerability ++;
                        mainGameMechsRef->incrementScore();
                        break;
                        
                }

            }
        }
        mainFoodRef->generateFood(playerPosList, *mainGameMechsRef);
    }
    else playerPosList->removeTail();
    */
    

    //Lose condition check <- iterate starting from second element

    if (playerPosList->checkFor(newPlayerPos, -1, 1))
    {
        if (!invulnerability)
            {
                mainGameMechsRef->setLoseFlag();
                mainGameMechsRef->setExitTrue();
            }
            else
            {
                invulnerability --;
            }
    }

    /*
    for (int i = 1; i < playerPosList->getSize(); i++)
    {
        if (playerPosList->getElement(i).isPosEqual(&newPlayerPos))
        {
            if (!invulnerability)
            {
                mainGameMechsRef->setLoseFlag();
                mainGameMechsRef->setExitTrue();
            }
            else
            {
                invulnerability --;
            }
        }
    }
    */

   // Movement is unsupported if the list is at capacity, so we will make this a win condition.
    if (playerPosList->atCapacity() == true)     
        mainGameMechsRef->setExitTrue();
}

// More methods to be added
char Player::checkFoodConsumption()
{
    //will get rid of hard code later
    return mainFoodRef->getFoodBucket()->getSymbolIfHas(newPlayerPos);
}

int Player::getLives()
{
    return invulnerability + 1;
}
