#include "objPos.h"
#include "GameMechs.h"

class Food
{
    private:
        objPos foodPos;
        objPosArrayList* foodBucket;
        int targetBucketSize;

        char* specials;
        int specialMenuLength;

    public:
        Food(); // Default Constructor
        ~Food();    // Destroyer

        void generateFood(objPosArrayList* blockOff, GameMechs &mechs);

        objPos const getFoodPos();
        objPos const getFoodPos(int index);
        objPosArrayList* const getFoodBucket();    
        
};