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

    const char shutdownKey = 27, upKey = 'w', leftKey = 'a', downKey = 's', rightKey = 'd', loseKey = 'x', scoreKey = 'z', foodKey = 'f';

    if(mainGameMechsRef->getInput() != 0)  // if not null character
    {
        if (dizzyTime == 0)     // if controls are not inverted
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

                /*
                A note if you're worried:
                - yes, eventually the time delay would reach 0 if you ate enough of these
                - however you'd have to eat like 100k of them
                - by the time you ate 200 the game would end in victory
                */

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

int Player::getDizzyTime()
{
    return dizzyTime;
}

int Player::getBlindTime()
{
    return blindTime;
}

void Player::decrementStatusEffects(int decrementAmount)
{
    if (dizzyTime >= decrementAmount) dizzyTime -= decrementAmount;     // decrease the remaining time you are dizzy
    else dizzyTime = 0;

    if (blindTime >= decrementAmount) blindTime -= decrementAmount;     // decrease the remaining time you are blind
    else blindTime = 0;
}
