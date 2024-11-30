#include "Player.h"


Player::Player(GameMechs* thisGMRef)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;

    // more actions to be included

    objPos startPos(thisGMRef->getBoardSizeX()/2,thisGMRef->getBoardSizeY()/2,'*');

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

    const char shutdownKey = 27, upKey = 'w', leftKey = 'a', downKey = 's', rightKey = 'd', loseKey = 'x', scoreKey = 'z';

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

    playerPosList->removeTail();    // Did this first to avoid issues nearing 200 elements
    playerPosList->insertHead(newPlayerPos);
}

// More methods to be added