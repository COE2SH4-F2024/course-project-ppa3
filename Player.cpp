#include "Player.h"


Player::Player(GameMechs* thisGMRef)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;

    playerPos.setObjPos(10,10,'*');

    // more actions to be included
}


Player::~Player()
{
    // delete any heap members here
    // idk that we have any lmfao
}

objPos Player::getPlayerPos() const
{
    // return the reference to the playerPos arrray list
    return playerPos;
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

    int xWrap = mainGameMechsRef->getBoardSizeX()-2;
    int yWrap = mainGameMechsRef->getBoardSizeY()-2;

    switch(myDir)
    {
        case DOWN:
            playerPos.pos->y %= (yWrap); // if about to hit the border, wrap around
            playerPos.pos->y++;
            break;
        case LEFT:
            playerPos.pos->x--;
            if (!playerPos.pos->x) {playerPos.pos->x = xWrap;} // if in the border, wrap around
            break;
        case UP:
            playerPos.pos->y--;
            if (!playerPos.pos->y) {playerPos.pos->y = yWrap;}
            break;
        case RIGHT:
            playerPos.pos->x %= (xWrap);
            playerPos.pos->x++;
            break;
    }
}

// More methods to be added