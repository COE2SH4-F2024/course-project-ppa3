#ifndef PLAYER_H
#define PLAYER_H

#include "GameMechs.h"
#include "objPos.h"
#include "objPosArrayList.h"
#include "Food.h"   // I'm like 80% sure we are allowed to do this

class Player
{
    // Construct the remaining declaration from the project manual.

    // Only some sample members are included here

    // You will include more data members and member functions to complete your design.

    
    public:
        enum Dir {UP, DOWN, LEFT, RIGHT, STOP};  // This is the direction state

        Player(GameMechs* thisGMRef, Food* thisFoodRef);    // The player needs to be able to cause food regen
        ~Player();

        objPosArrayList* getPlayerPos() const; // Upgrade this in iteration 3.       
        void updatePlayerDir();
        void movePlayer();

        // More methods to be added here
        bool checkFoodConsumption();
        void increasePlayerLength();

    private:
        objPosArrayList* playerPosList; // Upgrade this in iteration 3.       
        enum Dir myDir;

        // Need a reference to the Main Game Mechanisms
        GameMechs* mainGameMechsRef;
        Food* mainFoodRef;
};

#endif