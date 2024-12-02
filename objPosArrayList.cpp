#include "objPosArrayList.h"

// Paste your Tested implementation here.

// Check lecture contents on general purpose array list construction, 
// and modify it to support objPos array list construction.

objPosArrayList::objPosArrayList()
{
    aList = new objPos[ARRAY_MAX_CAP];  
    arrayCapacity = ARRAY_MAX_CAP;
    listSize = 0;
}

objPosArrayList::~objPosArrayList()
{
    delete[] aList;
}

objPosArrayList::objPosArrayList(const objPosArrayList &opal)
{
    aList = new objPos[ARRAY_MAX_CAP];  
    arrayCapacity = ARRAY_MAX_CAP;
    listSize = 0; 
    int i;

    for (i=0; i<opal.getSize(); i++)
    {
        insertTail(opal.getElement(i));
    }
}

objPosArrayList& objPosArrayList::operator=(const objPosArrayList &opal)
{
    if (this != nullptr && this != &opal)
    {
        this->aList = new objPos[ARRAY_MAX_CAP];
        this->arrayCapacity = ARRAY_MAX_CAP;
        this->listSize = 0;
        int i;

        for (i=0; i<opal.getSize(); i++)
        {
            this->insertTail(opal.getElement(i));
        }  
    }
}

int objPosArrayList::getSize() const
{
    return listSize;
}

bool objPosArrayList::atCapacity() const    // Manually added to check if the snake (or any other list) is full
{
    if (listSize == arrayCapacity) return true;
    return false;
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

void objPosArrayList::insertHead(int x, int y, char s)
{
    if (listSize<arrayCapacity)
    {
        listSize++;
        int i;
        for (i=listSize;i>=0;i--)
        {
            aList[i+1].setObjPos(aList[i]);
        }
        aList[0].setObjPos(x,y,s);
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

void objPosArrayList::removeTail(int count)
{
    if (listSize >= count)
    {
        listSize -= count;
    }
}

void objPosArrayList::removeAll()
{
    listSize = 0;
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

bool objPosArrayList::checkFor(objPos myPos)
{
    int i;
    for (i=0; i<listSize; i++)
    {
        if (getElement(i).isPosEqual(&myPos))   return true;
    }
    return false;   
}

bool objPosArrayList::checkFor(objPos myPos, int manualSize)
{
    
    int i;
    if (manualSize > listSize)  manualSize = listSize;  // no seg fault
    
    for (i=0; i<manualSize; i++)     
    {
        if (getElement(i).isPosEqual(&myPos))   return true;
    }
    return false;   
}

bool objPosArrayList::checkFor(objPos myPos, int manualSize, int iStart)
{
    
    int i;
    if (manualSize < 0 || manualSize > listSize)  manualSize = listSize;  // no seg fault

    for (i=iStart; i<manualSize; i++)     
    {
        if (getElement(i).isPosEqual(&myPos))   return true;
    }
    return false;   
}

char objPosArrayList::getSymbolIfHas(objPos myPos)
{
    int i;
    char mySymbol;
    for (i=0; i<listSize; i++)
    {
        /*
        Check if myPos [ideally new position of head] and element of the list [ideally food bucket] coincide
        If so, collect and return the symbol from the matching element
        If not, getSymbolIfPosEqual will return null and nothing will return
        */

        mySymbol = getElement(i).getSymbolIfPosEqual(&myPos);
        if (mySymbol != 0)   return mySymbol;
    }
    return 0;
}
// Paste your Tested implementation here.
// Paste your Tested implementation here.
