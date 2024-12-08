#include "PancakeSort.h"

const std::string sv::PancakeSort::name = "Pancake sort";

sv::PancakeSort::PancakeSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::PancakeSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    bool result;
    size_t minIndex;
    for (size_t i = 0; i < visualizer.getArrayLength() - 1; i++)
    {
        minIndex = i;
        for (size_t j = i + 1; j < visualizer.getArrayLength(); j++)
        {
            if (!visualizer.visualizerCompare(j, minIndex, result, SortVisualizer::less)) return;
            if (result)
            {
                minIndex = j;
            }
        }
        visualizer.visualizerReverse(minIndex, visualizer.getArrayLength());
        visualizer.visualizerReverse(i, visualizer.getArrayLength());
    }

    visualizer.visualizerDone();
}

const std::string& sv::PancakeSort::getAlgorithmName() const
{
	return name;
}
