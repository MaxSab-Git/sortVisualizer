#include "SortingHelpers.h"

void sv::insertionSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    bool result;
    for (size_t i = start + 1; i < end; i++)
    {
        if (!visualizer.visualizerCompare(i - 1, i, result, SortVisualizer::greater)) return;
        if (result)
        {
            unsigned int tmp = visualizer.getValue(i);
            size_t insertIndex = start;
            for (size_t j = i; j > start; j--)
            {
                if (!visualizer.visualizerExternCompare(j - 1, tmp, result, SortVisualizer::greater)) return;
                if (result)
                {
                    visualizer.visualizerCopy(j, j - 1);
                }
                else
                {
                    insertIndex = j;
                    break;
                }
            }
            visualizer.visualizerWrite(insertIndex, tmp);
        }
    }
}

void sv::reverseInsertionSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    bool result;
    for (size_t i = start + 1; i < end; i++)
    {
        if (!visualizer.visualizerCompare(i - 1, i, result, SortVisualizer::lessEquals)) return;
        if (result)
        {
            unsigned int tmp = visualizer.getValue(i);
            size_t insertIndex = start;
            for (size_t j = i; j > start; j--)
            {
                if (!visualizer.visualizerExternCompare(j - 1, tmp, result, SortVisualizer::lessEquals)) return;
                if (result)
                {
                    visualizer.visualizerCopy(j, j - 1);
                }
                else
                {
                    insertIndex = j;
                    break;
                }
            }
            visualizer.visualizerWrite(insertIndex, tmp);
        }
    }
}

void sv::shiftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t end)
{
    bool result;

    size_t child = i * 2 + 1;
    while (start + child < end)
    {
        if (start + child + 1 < end)
        {
            if (!visualizer.visualizerCompare(start + child + 1, start + child, result, SortVisualizer::greater)) return;
            if (result) child += 1;
        }

        visualizer.visualizerCompare(start + child, start + i, result, SortVisualizer::greater);
        if (result)
        {
            visualizer.visualizerSwap(start + child, start + i);
            i = child;
        }
        else break;

        child = i * 2 + 1;
    }
}

void sv::heapSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if (start >= end) return;


    for (size_t i = (start + end) / 2; i > start; i--)
    {
        shiftDown(visualizer, start, i - 1 - start, end);
        if (visualizer.isAborted()) return;
    }

    for (size_t i = end - 1; i > start; i--)
    {
        if (!visualizer.visualizerSwap(start, i)) return;
        shiftDown(visualizer, start, 0, i);
    }
}

void sv::merge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end)
{
    bool result;
    size_t leftPtr = start, rightPtr = middle;
    for (size_t i = start; i < end; i++)
    {
        if (visualizer.isAborted()) return;

        if (leftPtr < middle)
        {
            if (rightPtr >= end)
            {
                visualizer.visualizerCopy(i, leftPtr, VisualOperation::ArrayAccess::AuxMain);
                leftPtr++;
            }
            else
            {
                visualizer.visualizerCompare(leftPtr, rightPtr, result, SortVisualizer::lessEquals, sv::VisualOperation::ArrayAccess::Main);
                if (result)
                {
                    visualizer.visualizerCopy(i, leftPtr, VisualOperation::ArrayAccess::AuxMain);
                    leftPtr++;
                }
                else
                {
                    visualizer.visualizerCopy(i, rightPtr, VisualOperation::ArrayAccess::AuxMain);
                    rightPtr++;
                }
            }
        }
        else
        {
            visualizer.visualizerCopy(i, rightPtr, VisualOperation::ArrayAccess::AuxMain);
            rightPtr++;
        }
    }

    for (size_t i = start; i < end; i++)
    {
        if (!visualizer.visualizerCopy(i, i, VisualOperation::ArrayAccess::MainAux)) return;
    }
}

void sv::timMerge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end)
{
    if (middle - start <= end - middle)
    {
        for (size_t i = start; i < middle; i++)
        {
            if (!visualizer.visualizerCopy(i - start, i, VisualOperation::ArrayAccess::AuxMain)) return;
        }

        bool result;
        size_t leftPtr = 0, rightPtr = middle;
        for (size_t i = start; i < end; i++)
        {
            if (visualizer.isAborted()) return;

            if (leftPtr < middle - start)
            {
                if (rightPtr >= end)
                {
                    visualizer.visualizerCopy(i, leftPtr, VisualOperation::ArrayAccess::MainAux);
                    leftPtr++;
                }
                else
                {
                    visualizer.visualizerCompare(leftPtr, rightPtr, result, SortVisualizer::lessEquals, sv::VisualOperation::ArrayAccess::AuxMain);
                    if (result)
                    {
                        visualizer.visualizerCopy(i, leftPtr, VisualOperation::ArrayAccess::MainAux);
                        leftPtr++;
                    }
                    else
                    {
                        visualizer.visualizerCopy(i, rightPtr, VisualOperation::ArrayAccess::Main);
                        rightPtr++;
                    }
                }
            }
            else
            {
                visualizer.visualizerCopy(i, rightPtr, VisualOperation::ArrayAccess::Main);
                rightPtr++;
            }
        }
    }
    else
    {
        for (size_t i = middle; i < end; i++)
        {
            if (!visualizer.visualizerCopy(i - middle, i, VisualOperation::ArrayAccess::AuxMain)) return;
        }

        bool result;
        size_t leftPtr = start, rightPtr = 0;
        for (size_t i = end; i > start; i--)
        {
            if (visualizer.isAborted()) return;

            if (leftPtr < middle)
            {
                if (rightPtr >= end - middle)
                {
                    visualizer.visualizerCopy(i - 1, start + middle - leftPtr - 1, VisualOperation::ArrayAccess::Main);
                    leftPtr++;
                }
                else
                {
                    visualizer.visualizerCompare(start + middle - leftPtr - 1, end - middle - rightPtr - 1, result, SortVisualizer::greater, sv::VisualOperation::ArrayAccess::MainAux);
                    if (result)
                    {
                        visualizer.visualizerCopy(i - 1, start + middle - leftPtr - 1, VisualOperation::ArrayAccess::Main);
                        leftPtr++;
                    }
                    else
                    {
                        visualizer.visualizerCopy(i - 1, end - middle - rightPtr - 1, VisualOperation::ArrayAccess::MainAux);
                        rightPtr++;
                    }
                }
            }
            else
            {
                visualizer.visualizerCopy(i - 1, end - middle - rightPtr - 1, VisualOperation::ArrayAccess::MainAux);
                rightPtr++;
            }
        }
    }
}

void sv::rotateMerge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end)
{
    if (end <= middle || middle <= start || visualizer.isAborted()) return;
    size_t middleLeftArray = (middle + start) / 2;

    size_t insertIndex = binarySearch(visualizer, middle, end, middleLeftArray);

    rotate(visualizer, middleLeftArray, middle, insertIndex);
    rotateMerge(visualizer, start, middleLeftArray, middleLeftArray + (insertIndex - middle));
    rotateMerge(visualizer, middleLeftArray + (insertIndex - middle) + 1, insertIndex, end);
}

void sv::inPlaceMerge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end)
{
    bool result;
    size_t leftPtr = start, rightPtr = middle;
    
    while (leftPtr < rightPtr && rightPtr < end)
    {
        if (!visualizer.visualizerCompare(leftPtr, rightPtr, result, SortVisualizer::lessEquals)) return;
        if (result)
        {
            leftPtr++;
        }
        else
        {
            unsigned int tmp = visualizer.getValue(rightPtr);
            for (size_t j = rightPtr; j > leftPtr; j--)
            {
                if (!visualizer.visualizerCopy(j, j - 1)) return;
            }

            visualizer.visualizerWrite(leftPtr, tmp);

            leftPtr++;
            rightPtr++;
        }
    }
}

size_t sv::roundToNextPowerOf2(size_t i)
{
    i--;
    i |= (i >> 1);
    i |= (i >> 2);
    i |= (i >> 4);
    i |= (i >> 8);
    i |= (i >> 16);
    i++;
    return i;
}

size_t sv::roundToPreviousPowerOf2(size_t i)
{
    i = i | (i >> 1);
    i = i | (i >> 2);
    i = i | (i >> 4);
    i = i | (i >> 8);
    i = i | (i >> 16);
    return i - (i >> 1);
}

void sv::rotate(SortVisualizer& visualizer, size_t start, size_t middle, size_t end)
{
    size_t smLenght = middle - start;
    size_t meLenght = end - middle;

    while (smLenght > 0 && meLenght > 0)
    {
        if (meLenght < smLenght)
        {
            for (size_t i = middle; i < end; i++)
            {
                visualizer.visualizerSwap(i - meLenght, i);
            }

            smLenght -= meLenght;
            middle -= meLenght;
            end -= meLenght;
        }
        else
        {
            for (size_t i = start; i < middle; i++)
            {
                visualizer.visualizerSwap(i, i + smLenght);
            }

            meLenght -= smLenght;
            middle += smLenght;
            start += smLenght;
        }
    }
}

size_t sv::binarySearch(SortVisualizer& visualizer, size_t start, size_t end, size_t elem)
{
    size_t middle;
    bool result;
    while (end > start && !visualizer.isAborted())
    {
        middle = (end + start) >> 1;
        visualizer.visualizerCompare(middle, elem, result, SortVisualizer::less);
        if (result) start = middle + 1;
        else end = middle;
    }

    return start;
}

size_t sv::binarySearchForInsertionSort(SortVisualizer& visualizer, size_t start, size_t end, size_t elem)
{
    size_t middle;
    bool result;
    while (end > start && !visualizer.isAborted())
    {
        middle = (end + start) >> 1;
        visualizer.visualizerCompare(middle, elem, result, SortVisualizer::lessEquals);
        if (result) start = middle + 1;
        else end = middle;
    }

    return start;
}
