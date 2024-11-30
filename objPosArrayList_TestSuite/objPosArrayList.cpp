#include "objPosArrayList.h"
#include <iostream> // delete this eventually
using namespace std;    // also delete this eventually

// Check lecture contents on general purpose array list construction, 
// and modify it to support objPos array list construction.

objPosArrayList::objPosArrayList()
{
    aList = new objPos[ARRAY_MAX_CAP];  // check if aList (the pointer) also needs to be on the heap, or just the array?
    arrayCapacity = ARRAY_MAX_CAP;
    listSize = 0; 
    int i;
    for (i=0; i<arrayCapacity; i++)
    {
        aList[i].setObjPos(0,0,0);
        cout << aList[i].pos->x;    // delete this
    }
}

objPosArrayList::~objPosArrayList()
{
    delete[] aList;
}

int objPosArrayList::getSize() const
{
    return listSize;
}

void objPosArrayList::insertHead(objPos thisPos)
{
    if (listSize<arrayCapacity)
    {
        listSize++;
        int i;
        for (i=listSize;i>=0;i--)
        {
            aList[i+1].setObjPos(aList[i]);
        }
        aList[0].setObjPos(thisPos);
    }
}

void objPosArrayList::insertTail(objPos thisPos)
{
    listSize++;
    aList[listSize].setObjPos(thisPos);
}

void objPosArrayList::removeHead()
{
    if (listSize > 0)
    {
        int i;
        for (i=0; i<listSize-1; i++)
        {
            aList[i].setObjPos(aList[i+1]);
        }
        listSize--;
    }
}

void objPosArrayList::removeTail()
{
    if (listSize > 0)
    {
        listSize--;
    }
}

objPos objPosArrayList::getHeadElement() const
{
    if (listSize > 0)
    {
        return aList[0];
    }
}

objPos objPosArrayList::getTailElement() const
{
    if (listSize > 0)
    {
        return aList[listSize-1];
    }
}

objPos objPosArrayList::getElement(int index) const
{
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= listSize)
    {
        index = listSize - 1;
    }
    return aList[index];
}