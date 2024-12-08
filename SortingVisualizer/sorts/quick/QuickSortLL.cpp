#include "QuickSortLL.h"

const std::string sv::QuickSortLL::name = "Quick sort (left/left pointers)";

sv::QuickSortLL::QuickSortLL(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::QuickSortLL::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength());

    visualizer.visualizerDone();
}

const std::string& sv::QuickSortLL::getAlgorithmName() const
{
    return name;
}

void sv::QuickSortLL::subSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if (end - start < 2 || visualizer.isAborted()) return;
    else
    {

        size_t leftPtr = start + 1;

        for (size_t i = leftPtr; i < end; i++)
        {
            bool result;
            if (!visualizer.visualizerCompare(i, start, result, SortVisualizer::lessEquals)) return;
            if (result) visualizer.visualizerSwap(leftPtr++, i);
        }

        visualizer.visualizerSwap(start, leftPtr - 1);

        subSort(visualizer, start, leftPtr - 1);
        subSort(visualizer, leftPtr, end);
    }
}