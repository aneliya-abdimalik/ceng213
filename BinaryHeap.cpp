#include "BinaryHeap.h"

BinaryHeap::BinaryHeap()
{
    // TODO: or not
}

void BinaryHeap::swap(HeapElement *x, HeapElement *y)
{
    HeapElement temp = *x;
    *x = *y;
    *y = temp;
}

bool BinaryHeap::Add(int uniqueId, double weight)
{
    for(int i = 0; i<HeapSize();i++){
        if(uniqueId == elements[i].uniqueId) return false;
    }
    int i = HeapSize();
    HeapElement elem;
    elem.uniqueId = uniqueId;
    elem.weight = weight;
    elements.push_back(elem);
    heapify_up(i);
    return true;
}

void BinaryHeap::heapify_down(int i)
{
    int left = 2*i;
    int right = 2*i + 1;
    int smallest = i;
    if (left < HeapSize() && elements[left].weight < elements[i].weight)
        smallest = left;
    if (right < HeapSize() && elements[right].weight < elements[smallest].weight)
        smallest = right;
    if (smallest != i)
    {
        swap(&elements[i], &elements[smallest]);
        heapify_down(smallest);
    }
}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    if(HeapSize() == 0) 
        return false;
    outUniqueId = elements[0].uniqueId;
    outWeight = elements[0].weight;
    elements[0].uniqueId = elements[HeapSize() - 1].uniqueId;
    elements[0].weight = elements[HeapSize() - 1].weight;
    elements.erase(elements.begin() + HeapSize() - 1);
    if(HeapSize() > 1)
        heapify_down(0);
    return true;
}

void BinaryHeap::heapify_up(int i)
{
    while (i != 0 && elements[(int)(i/2)].weight > elements[i].weight)
    {
        swap(&elements[i], &elements[(int)(i/2)]);
        i /= 2;
    }
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    for(int i = 0; i<HeapSize() ;i++)
    {
        if(uniqueId == elements[i].uniqueId)
        {
            if(elements[i].weight > newWeight)
                elements[i].weight = newWeight,
                heapify_up(i);
            else if(elements[i].weight < newWeight)
                elements[i].weight = newWeight,
                heapify_down(i);
            else
                elements[i].weight = newWeight;
            return true;
        }
    }
    return false;
}

int BinaryHeap::HeapSize() const
{
    return elements.size();
}