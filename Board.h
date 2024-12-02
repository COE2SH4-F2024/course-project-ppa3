#include "objPos.h"
#include "objPosArrayList.h"

class Board {
    private:
        int boardX;
        int boardY;
        char** boardData;

    public:
        Board();
        ~Board();
        Board(const Board &B);  // copy constructor
        Board& operator=(const Board &B);


        Board(int x, int y);    // alt constructor

        int getBoardX() const;
        int getBoardY() const;
        char getData(int x, int y) const;

        // Large scale operations
        void wipeInterior();    // default ' '
        void drawBorder();      // default '#'
        void drawBorder(char symbol);

        // Put something on the board, using a single objPos or a whole list
        void writeToBoard(objPos o);
        void writeToBoard(objPosArrayList opal);


};