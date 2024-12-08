#include "SlowSort.h"

const std::string sv::SlowSort::name = "Slow sort";

sv::SlowSort::SlowSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::SlowSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / std::powf(static_cast<float>(visualizer.getArrayLength()), std::logf(static_cast<float>(visualizer.getArrayLength())) / 1.65f);
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    subSort(visualizer, 0, visualizer.getArrayLength());

    visualizer.visualizerDone();
}

const std::string& sv::SlowSort::getAlgorithmName() const
{
	return name;
}

void sv::SlowSort::subSort(SortVisualizer& visualizer, size_t start, size_t end)
{
    if (visualizer.isAborted()) return;

    size_t middle = (end + start) / 2;

    if (end - start > 2)
    {
        subSort(visualizer, start, middle);
        subSort(visualizer, middle, end);
    }

    if (end - start >= 2)
    {
        bool result;
        if (!visualizer.visualizerCompare(middle - 1, end - 1, result, SortVisualizer::greater)) return;
        if (result) visualizer.visualizerSwap(middle - 1, end - 1);
        subSort(visualizer, start, end - 1);
    }
}
