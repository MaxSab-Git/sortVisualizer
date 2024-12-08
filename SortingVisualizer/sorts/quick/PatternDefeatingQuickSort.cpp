#include "PatternDefeatingQuickSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::PatternDefeatingQuickSort::name = "Pattern defeating quick sort";

sv::PatternDefeatingQuickSort::PatternDefeatingQuickSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::PatternDefeatingQuickSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength(), static_cast<size_t>(2 * std::log2(visualizer.getArrayLength())));
    sv::insertionSort(visualizer, 0, visualizer.getArrayLength());

    visualizer.visualizerDone();
}

const std::string& sv::PatternDefeatingQuickSort::getAlgorithmName() const
{
	return name;
}

void sv::PatternDefeatingQuickSort::subSort(SortVisualizer& visualizer, size_t start, size_t end, size_t recursions)
{
    if (end - start < 16 || visualizer.isAborted()) return;
    else if (recursions == 0)
    {
        sv::heapSort(visualizer, start, end);
    }
    else
    {
        size_t leftPtr = start, rightPtr = end;

        {
            size_t first = start, middle = (end - start) / 2, last = end - 1;
            size_t pivot = start;

            if (end - start >= 128)
            {
                pivot = findMedian(visualizer,
                    findMedian(visualizer, first, first + 1, first + 2),
                    findMedian(visualizer, middle - 1, middle, middle + 1),
                    findMedian(visualizer, last - 2, last - 1, last)
                );
            }
            else pivot = findMedian(visualizer, first, middle, last);

            visualizer.visualizerSwap(start, pivot);
        }

        bool noSwap = true;
        while (true)
        {
            bool result;

            ++leftPtr;
            while (leftPtr < rightPtr)
            {
                if (!visualizer.visualizerCompare(leftPtr, start, result, SortVisualizer::lessEquals)) return;
                if (result) ++leftPtr;
                else break;
            }
            --rightPtr;
            while (leftPtr < rightPtr)
            {
                if (!visualizer.visualizerCompare(rightPtr, start, result, SortVisualizer::greater)) return;
                if (result) --rightPtr;
                else break;
            }

            if (leftPtr >= rightPtr) break;

            visualizer.visualizerSwap(leftPtr, rightPtr);
            noSwap = false;
        }

        visualizer.visualizerSwap(start, leftPtr - 1);

        if (noSwap)
        {
            bool result;
            size_t swapCounter = 0;
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
                            if (++swapCounter >= 8)
                            {
                                insertIndex = j - 1;
                                break;
                            }
                        }
                        else
                        {
                            insertIndex = j;
                            break;
                        }
                    }
                    visualizer.visualizerWrite(insertIndex, tmp);
                }

                if (swapCounter >= 8) break;
            }
            if (swapCounter < 8) return;
        }

        subSort(visualizer, start, leftPtr - 1, recursions - 1);
        subSort(visualizer, leftPtr, end, recursions - 1);
    }
}

size_t sv::PatternDefeatingQuickSort::findMedian(SortVisualizer& visualizer, size_t first, size_t middle, size_t last)
{
    bool result;
    visualizer.visualizerCompare(first, middle, result, SortVisualizer::less);
    if (result)
    {
        visualizer.visualizerCompare(middle, last, result, SortVisualizer::less);
        if (result)
        {
            return middle;
        }

        visualizer.visualizerCompare(last, first, result, SortVisualizer::less);
        if (result)
        {
            return first;
        }
    }

    visualizer.visualizerCompare(middle, first, result, SortVisualizer::less);
    if (result)
    {
        visualizer.visualizerCompare(last, middle, result, SortVisualizer::less);
        if (result)
        {
            return middle;
        }

        visualizer.visualizerCompare(first, last, result, SortVisualizer::less);
        if (result)
        {
            return first;
        }
    }

    return last;
}
