#include "WeaveSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::WeaveSort::name = "Weave sort";

sv::WeaveSort::WeaveSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::WeaveSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
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

            size_t leftPtr = start + 1, rightPtr = middle;
            while (leftPtr < rightPtr)
            {
                unsigned tmp = visualizer.getValue(rightPtr);
                for (size_t i = rightPtr; i > leftPtr; i--)
                {
                    if (!visualizer.visualizerCopy(i, i - 1)) return;
                }
                visualizer.visualizerWrite(leftPtr, tmp);

                leftPtr += 2;
                rightPtr++;
            }
            insertionSort(visualizer, start, end);
        }

        partitionSize *= 2.0;
    }

    visualizer.visualizerDone();
}

const std::string& sv::WeaveSort::getAlgorithmName() const
{
    return name;
}
