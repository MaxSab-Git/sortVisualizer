#include "StoogeSort.h"

const std::string sv::StoogeSort::name = "Stooge sort";

sv::StoogeSort::StoogeSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::StoogeSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::powf(static_cast<float>(visualizer.getArrayLength()), 2.71f / 2.15f));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength());
    visualizer.visualizerDone();
}

const std::string& sv::StoogeSort::getAlgorithmName() const
{
    return name;
}

void sv::StoogeSort::subSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if ((end - start) > 2)
    {
        if (visualizer.isAborted()) return;

        size_t splitSize = (end - start) / 3;

        subSort(visualizer, start, end - splitSize);
        subSort(visualizer, start + splitSize, end);
        subSort(visualizer, start, end - splitSize);
    }
    else
    {
        bool result;
        if (!visualizer.visualizerCompare(start, end - 1, result, SortVisualizer::greater)) return;
        if (result) visualizer.visualizerSwap(start, end - 1);
    }
}
