#include "IterativeMergeSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::IterativeMergeSort::name = "Iterative merge sort";

sv::IterativeMergeSort::IterativeMergeSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::IterativeMergeSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    visualizer.createAuxiliaryArray(visualizer.getArrayLength());

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

            sv::merge(visualizer, start, middle, end);
        }

        partitionSize *= 2.0;
    }

    visualizer.visualizerDone();
}

const std::string& sv::IterativeMergeSort::getAlgorithmName() const
{
	return name;
}
