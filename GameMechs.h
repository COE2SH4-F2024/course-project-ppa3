#ifndef GAMEMECHS_H
#define GAMEMECHS_H

#include <cstdlib>
#include <time.h>

// #include "objPos.h"
// #include "objPosArrayList.h"
#include "Board.h"

using namespace std;

class GameMechs
{
    private:
        char input;
        bool exitFlag;
        bool loseFlag;
        int score;
        int speedMultiplier;

        int boardSizeX;
        int boardSizeY;

        Board* pGameBoard;    // new addition, stores the board to be printed

    public:
        GameMechs();
        GameMechs(int boardX, int boardY);
        ~GameMechs(); // is this one needed at all? Why or why not?
        GameMechs(const GameMechs &mechs);
        
        bool getExitFlagStatus() const; 
        void setExitTrue();
        bool getLoseFlagStatus() const;
        void setLoseFlag();

        char getInput() const;
        void setInput(char this_input);
        void clearInput();

        int getBoardSizeX() const;
        int getBoardSizeY() const;
        
        int getScore() const;
        void incrementScore();
        void incrementScore(int inc);   // overload for bonus features 
        
        // More methods should be added here
        int getSpeedMultiplier() const; 
        void incrementSpeedMultiplier(); 
        Board* getBoard() const; 
};

#endif