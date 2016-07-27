#pragma once

#include "constants.h"

#include <vector>

template<typename T>
class SelfGrowingPool
{
public:
    SelfGrowingPool(unsigned int preAllocated = 0) {
        usedElements = 0;
        if (preAllocated > 0) Alloc(preAllocated);
    }

    void ReleaseAll()
    {
        usedElements = 0;
    }

    T& UseOne()
    {
        while (usedElements >= elements.size())
        {
            elements.push_back(T());
        }
        return elements[usedElements++];
    }

    T& operator[](int i) {
        return elements[i];
    }

    unsigned int GetUsedSize() {
        return usedElements;
    }

    unsigned int GetTotalSize() {
        return elements.size();
    }

    void Free() {
        elements.clear();
    }

    void Alloc(unsigned int n) {
        elements.resize(n);
    }

private:
    unsigned int usedElements;
    std::vector<T> elements;
};
