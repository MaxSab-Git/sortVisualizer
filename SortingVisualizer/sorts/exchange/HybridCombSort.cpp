#include "HybridCombSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::HybridCombSort::name = "Hybrid comb sort";

sv::HybridCombSort::HybridCombSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::HybridCombSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / std::powf(static_cast<float>(visualizer.getArrayLength()), 1.2f);
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    const double reductionFactor = 1.3;
    size_t gap = visualizer.getArrayLength();

    bool result;
    while (true)
    {
        double newGap = static_cast<double>(gap) / reductionFactor;
        gap = static_cast<size_t>(newGap);
        if (gap <= 4) break;

        for (size_t j = 0; j < visualizer.getArrayLength() - gap; j++)
        {
            if (!visualizer.visualizerCompare(j, j + gap, result, SortVisualizer::greater)) return;
            if (result)
            {
                visualizer.visualizerSwap(j, j + gap);
            }
        }
    }

    sv::insertionSort(visualizer, 0, visualizer.getArrayLength());

    visualizer.visualizerDone();
}

const std::string& sv::HybridCombSort::getAlgorithmName() const
{
	return name;
}
