#include "IntroSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::IntroSort::name = "Intro sort";

sv::IntroSort::IntroSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::IntroSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength(), static_cast<size_t>(2 * std::log2(visualizer.getArrayLength())));
    sv::insertionSort(visualizer, 0, visualizer.getArrayLength());

    visualizer.visualizerDone();
}

const std::string& sv::IntroSort::getAlgorithmName() const
{
    return name;
}

void sv::IntroSort::subSort(SortVisualizer& visualizer, size_t start, size_t end, size_t recursions)
{
    if (end - start < 16 || visualizer.isAborted()) return;
    else if (recursions == 0)
    {
        sv::heapSort(visualizer, start, end);
    }
    else
    {
        size_t leftPtr = start, rightPtr = end;
        visualizer.visualizerSwap(start, (start + end) / 2);

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

        subSort(visualizer, start, leftPtr - 1, recursions - 1);
        subSort(visualizer, leftPtr, end, recursions - 1);
    }
}
