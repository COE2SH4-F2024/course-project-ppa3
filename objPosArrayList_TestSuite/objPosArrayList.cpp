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
    if (listSize < arrayCapacity)
    {
        aList[listSize].setObjPos(thisPos);
        listSize++;
    }
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
    else if (index >= arrayCapacity)    // Lecture suggests to use listSize here instead, but the test case would cause seg fault
    {
        index = arrayCapacity - 1;      // Due to this. In first case, listSize is 0 so you reference aList[-1]
    }
    return aList[index];
}   // We want no 'out of bounds' access but the test cases explicitly want to be able to access anything on the whole 200-member heap array
    // This implementation prevents seg fault but could cause semantic errors - BE CAREFUL