#include "RotateMergeSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::RotateMergeSort::name = "Rotate merge sort";

sv::RotateMergeSort::RotateMergeSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::RotateMergeSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::powf(std::logf(static_cast<float>(visualizer.getArrayLength())), 2));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    const double doubleLen = static_cast<double>(visualizer.getArrayLength());
    const double scaleFactor = doubleLen / static_cast<double>(sv::roundToNextPowerOf2(visualizer.getArrayLength() / 2));

    double partitionSize = scaleFactor * 0.5;
    while (partitionSize < doubleLen)
    {
        for (size_t j = 0; j < static_cast<size_t>(doubleLen / partitionSize); j += 2)
        {
            if (visualizer.isAborted()) return;

            double jIndex = static_cast<double>(j) * partitionSize;
            size_t start = static_cast<size_t>(jIndex), middle = static_cast<size_t>(jIndex + partitionSize), end = static_cast<size_t>(jIndex + partitionSize * 2.0);

            sv::rotateMerge(visualizer, start, middle, end);
        }

        partitionSize *= 2.0;
    }

    visualizer.visualizerDone();
}

const std::string& sv::RotateMergeSort::getAlgorithmName() const
{
	return name;
}
