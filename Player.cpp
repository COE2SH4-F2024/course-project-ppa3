#include "Player.h"


Player::Player(GameMechs* thisGMRef, Food* thisFoodRef)
{
    mainGameMechsRef = thisGMRef;
    mainFoodRef = thisFoodRef;
    myDir = STOP;

    // more actions to be included

    objPos startPos(mainGameMechsRef->getBoardSizeX()/2,mainGameMechsRef->getBoardSizeY()/2,'*');

    playerPosList = new objPosArrayList;
    playerPosList->insertHead(startPos);

    /*
    This block will start the player off with length 7 for debugging purposes.

    objPos startPos2(thisGMRef->getBoardSizeX()/2+1,thisGMRef->getBoardSizeY()/2,'*');  
    objPos startPos3(thisGMRef->getBoardSizeX()/2+2,thisGMRef->getBoardSizeY()/2,'*');
    objPos startPos4(thisGMRef->getBoardSizeX()/2+3,thisGMRef->getBoardSizeY()/2,'*');  
    objPos startPos5(thisGMRef->getBoardSizeX()/2+4,thisGMRef->getBoardSizeY()/2,'*');
    objPos startPos6(thisGMRef->getBoardSizeX()/2+5,thisGMRef->getBoardSizeY()/2,'*');  
    objPos startPos7(thisGMRef->getBoardSizeX()/2+6,thisGMRef->getBoardSizeY()/2,'*');

    playerPosList->insertHead(startPos2);
    playerPosList->insertHead(startPos3);
    playerPosList->insertHead(startPos4);
    playerPosList->insertHead(startPos5);
    playerPosList->insertHead(startPos6);
    playerPosList->insertHead(startPos7);

    */
}


Player::~Player()
{
    // delete any heap members here
    // idk that we have any lmfao
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

    objPos newPlayerPos = playerPosList->getHeadElement();

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

    

    if (checkFoodConsumption())
    {
        if (playerPosList->getSize() >= ARRAY_MAX_CAP) playerPosList->removeTail(); // Avoiding misbehaviour around capacity
        
        playerPosList->insertHead(newPlayerPos);    // Not in the method or we'd have to pass in newPlayerPos

        increasePlayerLength();     // I'd love to just remove this method tbh it's useless and they say we have freedom
    }

    else
    {
        playerPosList->removeTail();    // We cannot do this in reverse because motion would misbehave at capacity
        playerPosList->insertHead(newPlayerPos);
    }
}

// More methods to be added
bool Player::checkFoodConsumption()
{
    objPos foodPos = mainFoodRef->getFoodPos();
    return (playerPosList->getHeadElement().isPosEqual(&foodPos));
}

void Player::increasePlayerLength()
{   
    mainFoodRef->generateFood(playerPosList, *mainGameMechsRef);
    mainGameMechsRef->incrementScore();
}