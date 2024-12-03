#include "Player.h"

#include <iostream>
using namespace std;


Player::Player(GameMechs* thisGMRef, Food* thisFoodRef)
{
    mainGameMechsRef = thisGMRef;
    mainFoodRef = thisFoodRef;
    myDir = STOP;

    // Special effects
    invulnerability = 0;
    dizzyTime = 0;
    blindTime = 0;

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

    // define the keys
    const char shutdownKey = 27, upKey = 'w', leftKey = 'a', downKey = 's', rightKey = 'd', 
    loseKey = 'x', scoreKey = 'z', foodKey = 'f';

    if(mainGameMechsRef->getInput() != 0)  // if not null character
    {
        if (dizzyTime == 0)     // if controls are not inverted
        {
            switch(mainGameMechsRef->getInput())    // decision based on input
            {          

                case shutdownKey:  // 27 is ESC
                    mainGameMechsRef->setExitTrue(); // exit
                    break;

                case upKey:
                    if (myDir!=DOWN) {myDir=UP;}    // turn up unless going down
                    break;

                case leftKey:
                    if (myDir!=RIGHT) {myDir=LEFT;} // turn left unless going right
                    break;

                case downKey:
                    if (myDir!=UP) {myDir=DOWN;}    // and so on
                    break;

                case rightKey:
                    if (myDir!=LEFT) {myDir=RIGHT;}
                    break;

                /*  Debug-exclusive keys

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

                */

                default:
                    break;
            }
        }
        else    // THIS IS NOT THE MAIN CASE. WE HAVE FLIPPED THE direction CONTROLS FOR GAMEPLAY PURPOSES
        {
            switch(mainGameMechsRef->getInput())
            {                      
                case shutdownKey:  // 27 is ESC
                    mainGameMechsRef->setExitTrue(); // exit
                    break;

                case downKey:   // downKey sends you up
                    if (myDir!=DOWN) {myDir=UP;}
                    break;

                case rightKey:  // rightKey sends you left
                    if (myDir!=RIGHT) {myDir=LEFT;}
                    break;

                case upKey: // flipped
                    if (myDir!=UP) {myDir=DOWN;}
                    break;

                case leftKey:   // flipped
                    if (myDir!=LEFT) {myDir=RIGHT;}
                    break;

                /*  Debug keys are left as-is
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
                */

                default:
                    break;
            }
        }
        mainGameMechsRef->clearInput();
    }
}

void Player::movePlayer()
{
    // Wrap logic transplanted from PPA3

    int xWrap = mainGameMechsRef->getBoardSizeX()-2;    // These exist to minimize extra calls
    int yWrap = mainGameMechsRef->getBoardSizeY()-2;

    newPlayerPos = playerPosList->getHeadElement();     // objPos containing your next position.

    switch(myDir)   // change newPlayerPos based on direction
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

    playerPosList->insertHead(newPlayerPos);    // snake head is now at the new position, will deal with tail soon



    char eatenSymbol = checkFoodConsumption();  // null if nothing eaten, else this is the character we just ate
    
    if (eatenSymbol != 0)
    {
        switch (eatenSymbol)    // Different foods have different effects, + some increment score more than others
        {
            case '+':   // Extra life! A life is lost each frame when the head is in the body.
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
            
            case '!':   // Blindness: only snake head and foods are visible for 3 seconds.
                mainGameMechsRef->incrementScore();
                blindTime = 1000000 * 3;    // 3 sec.
                break;

            case '?':   // Dizziness: flip direction controls for 5 seconds. up is down, down is up, etc.
                mainGameMechsRef->incrementScore();
                dizzyTime = 1000000 * 5;    // 5 sec.
                break;

            case '^':   // raises game speed irreversibly. May eventually outpace human reaction time but thats just part of the fun
                mainGameMechsRef->incrementScore();
                mainGameMechsRef->incrementSpeedMultiplier();
                break;

            default:    // just normal food, as long as you ate SOMETHING score is incremented
                mainGameMechsRef->incrementScore();
                break;

        }
        mainFoodRef->generateFood(playerPosList, *mainGameMechsRef);    // If something was eaten regenerate all food

    }
    else playerPosList->removeTail();   // If we didn't eat anything, head grew so tail must shrink
        

    

    //Lose condition check <- iterate starting from second element

    if (playerPosList->checkFor(newPlayerPos, -1, 1))   // check playerPosList for newPlayerPos from index 1 (2nd) to end
    {
        if (!invulnerability)   // If you don't have extra lives and you hit yourself, game over
            {
                mainGameMechsRef->setLoseFlag();
                mainGameMechsRef->setExitTrue();
            }
            else    // If you do have lives, lose one and proceed
            {
                invulnerability --;
            }
    }




   // Movement is unsupported if the list is at capacity, so we will make this a win condition.
   // Good luck getting this to happen
    if (playerPosList->atCapacity() == true)     
        mainGameMechsRef->setExitTrue();
}

// More methods to be added

char Player::checkFoodConsumption()     // Returns the symbol of the consumed food, or null if none
{
    return mainFoodRef->getFoodBucket()->getSymbolIfHas(newPlayerPos);
}

int Player::getLives()      // How many lives does the player have? 1 by default
{
    return invulnerability + 1;
}

int Player::getDizzyTime()  // How much longer, in game microseconds, will you be dizzy? Usually 0
{
    return dizzyTime;
}

int Player::getBlindTime()  // How much longer, in game microseconds, will you be blind? Usually 0
{
    return blindTime;
}

/* 
If you have a status effect this will decrement them.
This is called every cycle with the preprocessor delay constant
As a result, if time has been sped up by eating '^', effects will pass faster from your perspective
*/ 
void Player::decrementStatusEffects(int decrementAmount)
{
    if (dizzyTime >= decrementAmount) dizzyTime -= decrementAmount;     // decrease the remaining time you are dizzy
    else dizzyTime = 0;

    if (blindTime >= decrementAmount) blindTime -= decrementAmount;     // decrease the remaining time you are blind
    else blindTime = 0;
}
