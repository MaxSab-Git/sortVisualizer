#include "OddEvenMergeSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::OddEvenMergeSort::name = "Odd even merge sort";

sv::OddEvenMergeSort::OddEvenMergeSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::OddEvenMergeSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::powf(std::logf(static_cast<float>(visualizer.getArrayLength())), 2));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    size_t realSize = sv::roundToNextPowerOf2(visualizer.getArrayLength());
    bool result;

    for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
    {
        for (size_t k = 0; k < realSize / mergerSize; k++)
        {
            size_t a = k * mergerSize;
            for (size_t i = 0; i < (mergerSize >> 1); i++)
            {
                size_t left = i + a, right = i + a + (mergerSize >> 1);
                if (right < visualizer.getArrayLength())
                {
                    if (!visualizer.visualizerCompare(left, right, result, SortVisualizer::greater)) return;
                    if (result) visualizer.visualizerSwap(left, right);
                }
            }
        }

        for (size_t j = (mergerSize >> 1); j > 1; j >>= 1)
        {
            size_t diff = (j >> 1);
            for (size_t r = 0; r < realSize / mergerSize; r++)
            {
                for (size_t k = 0; k < mergerSize / j - 1; k++)
                {
                    size_t a = k * j + r * mergerSize + diff;
                    for (size_t i = 0; i < (j >> 1); i++)
                    {
                        size_t left = i + a, right = i + a + (j >> 1);
                        if (right < visualizer.getArrayLength())
                        {
                            if (!visualizer.visualizerCompare(left, right, result, SortVisualizer::greater)) return;
                            if (result) visualizer.visualizerSwap(left, right);
                        }
                    }
                }
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::OddEvenMergeSort::getAlgorithmName() const
{
	return name;
}
