#include "BitonicSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::BitonicSort::name = "Bitonic sort";

sv::BitonicSort::BitonicSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::BitonicSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::powf(std::logf(static_cast<float>(visualizer.getArrayLength())), 2));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    size_t realSize = sv::roundToNextPowerOf2(visualizer.getArrayLength());

    bool result;
    for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
    {
        for (size_t k = 0; k < realSize; k += mergerSize)
        {
            for (size_t i = 0; i < (mergerSize >> 1); i++)
            {
                size_t left = k + i, right = k + mergerSize - i - 1;
                if (right < visualizer.getArrayLength())
                {
                    if (!visualizer.visualizerCompare(left, right, result, SortVisualizer::greater)) return;
                    if (result) visualizer.visualizerSwap(left, right);
                }
            }
        }

        for (size_t j = (mergerSize >> 1); j > 1; j >>= 1)
        {
            for (size_t k = 0; k < realSize; k += j)
            {
                for (size_t i = 0; i < (j >> 1); i++)
                {
                    size_t left = k + i, right = k + i + (j >> 1);
                    if (right < visualizer.getArrayLength())
                    {
                        if (!visualizer.visualizerCompare(left, right, result, SortVisualizer::greater)) return;
                        if (result) visualizer.visualizerSwap(left, right);
                    }
                }
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::BitonicSort::getAlgorithmName() const
{
	return name;
}
