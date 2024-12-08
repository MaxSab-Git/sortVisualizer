#include "CombSort.h"

const std::string sv::CombSort::name = "Comb sort";

sv::CombSort::CombSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::CombSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / std::powf(static_cast<float>(visualizer.getArrayLength()), 1.2f);
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(0.f);

    const double reductionFactor = 1.3;
    size_t gap = visualizer.getArrayLength();
    bool unsorted = true;

    bool result;
    while (gap > 1 || unsorted)
    {
        unsorted = false;
        double newGap = static_cast<double>(gap) / reductionFactor;
        gap = (newGap <= 1.0) ? 1 : static_cast<size_t>(newGap);

        for (size_t j = 0; j < visualizer.getArrayLength() - gap; j++)
        {
            if (!visualizer.visualizerCompare(j, j + gap, result, SortVisualizer::greater)) return;
            if (result)
            {
                visualizer.visualizerSwap(j, j + gap);
                unsorted = true;
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::CombSort::getAlgorithmName() const
{
    return name;
}
