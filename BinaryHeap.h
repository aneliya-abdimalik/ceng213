#pragma once

#include <vector>

struct HeapElement
{
    int     uniqueId;
    double  weight;
};

class BinaryHeap
{
    private:
    std::vector<HeapElement> elements;

    // Do not remove this the tester will utilize this
    // to access the private parts.
    friend class HW3Tester;
    void swap(HeapElement *fs, HeapElement *sn);
    void heapify_up(int i);
    void heapify_down(int i);
    

    protected:
    public:
        // Constructors & Destructor
                            BinaryHeap();
        //
        bool                Add(int uniqueId, double weight);
        bool                PopHeap(int& outUniqueId, double& outWeight);
        bool                ChangePriority(int uniqueId, double newWeight);
        int                 HeapSize() const;
};