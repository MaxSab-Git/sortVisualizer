#include "BubbleSort.h"

const std::string sv::BubbleSort::name = "Bubble sort";

sv::BubbleSort::BubbleSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::BubbleSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    bool result;
    for (size_t i = visualizer.getArrayLength() - 1; i > 0; i--)
    {
        for (size_t j = 0; j < i; j++)
        {
            if (!visualizer.visualizerCompare(j, j + 1, result, SortVisualizer::greater)) return;
            if (result)
            {
                visualizer.visualizerSwap(j, j + 1);
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::BubbleSort::getAlgorithmName() const
{
    return name;
}
