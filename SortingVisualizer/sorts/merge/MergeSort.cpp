#include "MergeSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::MergeSort::name = "Merge sort";

sv::MergeSort::MergeSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::MergeSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    visualizer.createAuxiliaryArray(visualizer.getArrayLength());
    subSort(visualizer, 0, visualizer.getArrayLength());
    visualizer.visualizerDone();
}

const std::string& sv::MergeSort::getAlgorithmName() const
{
    return name;
}

void sv::MergeSort::subSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if (end - start < 2 || visualizer.isAborted()) return;
    else
    {
        size_t middle = (end + start) / 2;

        subSort(visualizer, start, middle);
        subSort(visualizer, middle, end);

        sv::merge(visualizer, start, middle, end);
    }
}
