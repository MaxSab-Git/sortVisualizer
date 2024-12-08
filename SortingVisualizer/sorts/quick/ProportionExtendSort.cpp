#include "ProportionExtendSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::ProportionExtendSort::name = "Proportion extend sort";

sv::ProportionExtendSort::ProportionExtendSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::ProportionExtendSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength());

    visualizer.visualizerDone();
}

const std::string& sv::ProportionExtendSort::getAlgorithmName() const
{
	return name;
}

void sv::ProportionExtendSort::subSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if (end - start < 2 || visualizer.isAborted()) return;
    else
    {
        size_t subListLen = (end - start) >> 4;

        if (subListLen < 32) sv::insertionSort(visualizer, start, start + subListLen);
        else sv::heapSort(visualizer, start, start + subListLen);

        size_t leftPtr = start + (subListLen >> 1), rightPtr = end;
        size_t pivot = leftPtr;

        while (true)
        {
            bool result;

            ++leftPtr;
            while (leftPtr < rightPtr)
            {
                if (!visualizer.visualizerCompare(leftPtr, pivot, result, SortVisualizer::lessEquals)) return;
                if (result) ++leftPtr;
                else break;
            }
            --rightPtr;
            while (leftPtr < rightPtr)
            {
                if (!visualizer.visualizerCompare(rightPtr, pivot, result, SortVisualizer::greater)) return;
                if (result) --rightPtr;
                else break;
            }

            if (leftPtr >= rightPtr) break;

            visualizer.visualizerSwap(leftPtr, rightPtr);
        }

        visualizer.visualizerSwap(pivot, leftPtr - 1);

        subSort(visualizer, start, leftPtr - 1);
        subSort(visualizer, leftPtr, end);
    }
}
