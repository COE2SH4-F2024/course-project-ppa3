#include "GameMechs.h"

#include <iostream>
using namespace std;

GameMechs::GameMechs()
{
    input = 0;
    exitFlag = false;
    loseFlag = false;
    score = 0;
    speedMultiplier = 1;

    boardSizeX = 30;
    boardSizeY = 15;

    
    pGameBoard = new Board(boardSizeX, boardSizeY);      // initialize gameboard storage on the heap
    
}

GameMechs::GameMechs(int boardX, int boardY)
{
    input = 0;
    exitFlag = false;
    loseFlag = false;
    score = 0;
    boardSizeX = boardX;
    boardSizeY = boardY;
    if (boardSizeX<1) boardSizeX=30; // for safety
    if (boardSizeY<1) boardSizeY=15;

    cout << "wow";
    pGameBoard = new Board(boardSizeX, boardSizeY);      // initialize gameboard storage on the heap
}

// do you need a destructor?
GameMechs::~GameMechs()
{
    delete pGameBoard;       // debug

    // the short answer is no we do not need a destructor

    /* 
    the long answer is, if we want to store the board on the heap, 
    and write to it rather than printing it all in one shot,
    improving the performance of the program significantly and enabling many bonus features,
    then a destructor may be beneficial
    */
}

bool GameMechs::getExitFlagStatus() const
{
    return exitFlag;
}

bool GameMechs::getLoseFlagStatus() const
{
    return loseFlag;
}
    

char GameMechs::getInput() const
{
    return input;
}

int GameMechs::getScore() const
{
    return score;
}

void GameMechs::incrementScore()
{
    score++;
}

void GameMechs::incrementScore(int inc)
{
    score += inc;
}

int GameMechs::getBoardSizeX() const
{
    return boardSizeX;
}

int GameMechs::getBoardSizeY() const
{
    return boardSizeY;
}


void GameMechs::setExitTrue()
{
    exitFlag = true;
}

void GameMechs::setLoseFlag()
{
    loseFlag = true;
}

void GameMechs::setInput(char this_input)
{
    input = this_input;
}

void GameMechs::clearInput()
{
    input = 0;
}

// More methods should be added here

int GameMechs::getSpeedMultiplier()
{
    return speedMultiplier;
}

void GameMechs::incrementSpeedMultiplier()
{
    speedMultiplier++;
}

Board* GameMechs::getBoard()
{
    return pGameBoard;
}