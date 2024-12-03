#ifndef OBJPOS_ARRAYLIST_H
#define OBJPOS_ARRAYLIST_H

#define ARRAY_MAX_CAP 200

#include "objPos.h"

class objPosArrayList
{
    private:
        objPos* aList;
        int listSize;
        int arrayCapacity;

    public:
        objPosArrayList();
        ~objPosArrayList();

        objPosArrayList(const objPosArrayList &opal);   // implemented min 4 for fun/ expandability
        objPosArrayList& operator=(const objPosArrayList &opal);

        int getSize() const;
        int getCapacity() const;    // added by me
        bool atCapacity() const;    // added by me
        void insertHead(objPos thisPos);
        void insertHead(int x, int y, char s);  // overload by me
        void insertTail(objPos thisPos);
        void removeHead();
        void removeTail();
        void removeTail(int count); // another overload
        void removeAll();   // lazy reset to 0 
        
        objPos getHeadElement() const;
        objPos getTailElement() const;
        objPos getElement(int index) const;

        bool checkFor(objPos myPos);   // implemented to modularize list indexing
        bool checkFor(objPos myPos, int manualSize);   // overload of above
        bool checkFor(objPos myPos, int manualSize, int iStart);   // overload of above
        char getSymbolIfHas(objPos myPos);
};

#endif