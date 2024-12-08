#include "QuickSort.h"

const std::string sv::QuickSort::name = "Quick sort (left/right pointers)";

sv::QuickSort::QuickSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::QuickSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength());

    visualizer.visualizerDone();
}

const std::string& sv::QuickSort::getAlgorithmName() const
{
    return name;
}

void sv::QuickSort::subSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if (end - start < 2 || visualizer.isAborted()) return;
    else
    {
        size_t leftPtr = start, rightPtr = end;

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
        }

        visualizer.visualizerSwap(start, leftPtr - 1);

        subSort(visualizer, start, leftPtr - 1);
        subSort(visualizer, leftPtr, end);
    }
}
