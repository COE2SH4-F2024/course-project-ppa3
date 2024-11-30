#include "objPos.h"
#include "GameMechs.h"  // Make sure this is ok

class Food
{
    private:
        objPos foodPos;
    public:
        Food(); // Default Constructor
        ~Food();    // Destroyer

        void generateFood(objPosArrayList* blockOff, GameMechs mechs);
        objPos const getFoodPos();
};