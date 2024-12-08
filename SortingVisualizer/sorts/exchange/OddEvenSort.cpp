#include "OddEvenSort.h"

const std::string sv::OddEvenSort::name = "Odd even sort";

sv::OddEvenSort::OddEvenSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::OddEvenSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(0.f);

    bool result;

    bool unsorted = true;
    while (unsorted)
    {
        unsorted = false;
        for (size_t k = 0; k < 2; k++)
        {
            for (size_t i = k; i < visualizer.getArrayLength() - 1; i += 2)
            {
                if (!visualizer.visualizerCompare(i, i + 1, result, SortVisualizer::greater)) return;
                if (result)
                {
                    visualizer.visualizerSwap(i, i + 1);
                    unsorted = true;
                }
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::OddEvenSort::getAlgorithmName() const
{
    return name;
}
