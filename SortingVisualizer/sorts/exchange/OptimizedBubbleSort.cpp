#include "OptimizedBubbleSort.h"

const std::string sv::OptimizedBubbleSort::name = "Optimized bubble sort";

sv::OptimizedBubbleSort::OptimizedBubbleSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::OptimizedBubbleSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(0.f);

    size_t sorted = visualizer.getArrayLength() - 1;
    size_t newSorted = 0;
    bool result;
    while (sorted > 0)
    {
        newSorted = 0;
        for (size_t i = 0; i < sorted; i++)
        {
            if (!visualizer.visualizerCompare(i, i + 1, result, SortVisualizer::greater)) return;
            if (result)
            {
                visualizer.visualizerSwap(i, i + 1);
                newSorted = i;
            }
        }
        sorted = newSorted;
    }

    visualizer.visualizerDone();
}

const std::string& sv::OptimizedBubbleSort::getAlgorithmName() const
{
	return name;
}
