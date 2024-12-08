#include "DualPivotQuickSort.h"

const std::string sv::DualPivotQuickSort::name = "Dual pivot quick sort";

sv::DualPivotQuickSort::DualPivotQuickSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::DualPivotQuickSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength());
    visualizer.visualizerDone();
}

const std::string& sv::DualPivotQuickSort::getAlgorithmName() const
{
	return name;
}

void sv::DualPivotQuickSort::subSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if (end - start < 2 || visualizer.isAborted()) return;
    else
    {
        size_t leftPtr = start + 1, middlePtr = start + 1, rightPtr = end - 2;

        {
            bool result;
            visualizer.visualizerCompare(start, end - 1, result, SortVisualizer::greater);
            if (result) visualizer.visualizerSwap(start, end - 1);

            while (middlePtr <= rightPtr)
            {
                if (!visualizer.visualizerCompare(middlePtr, start, result, SortVisualizer::lessEquals)) return;
                if (result)
                {
                    visualizer.visualizerSwap(leftPtr, middlePtr);
                    leftPtr++;
                    middlePtr++;
                    continue;
                }

                visualizer.visualizerCompare(middlePtr, end - 1, result, SortVisualizer::greater);
                if (result)
                {
                    visualizer.visualizerSwap(middlePtr, rightPtr);
                    rightPtr--;
                }
                else
                {
                    middlePtr++;
                }
            }
        }

        visualizer.visualizerSwap(leftPtr - 1, start);
        visualizer.visualizerSwap(middlePtr, end - 1);

        subSort(visualizer, start, leftPtr - 1);
        subSort(visualizer, leftPtr, middlePtr);
        subSort(visualizer, middlePtr + 1, end);
    }
}
