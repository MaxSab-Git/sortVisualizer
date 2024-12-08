#include "PairwiseSortingNetwork.h"
#include "../misc/SortingHelpers.h"

const std::string sv::PairwiseSortingNetwork::name = "Pairwise sorting network";

sv::PairwiseSortingNetwork::PairwiseSortingNetwork(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::PairwiseSortingNetwork::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::powf(std::logf(static_cast<float>(visualizer.getArrayLength())), 2));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    size_t realSize = sv::roundToNextPowerOf2(visualizer.getArrayLength());

    bool result;
    for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
    {
        for (size_t j = 0; j < visualizer.getArrayLength(); j += mergerSize)
        {
            for (size_t i = 0; i < mergerSize >> 1; i++)
            {
                size_t left = i + j, right = i + j + (mergerSize >> 1);
                if (right < visualizer.getArrayLength())
                {
                    if (!visualizer.visualizerCompare(left, right, result, SortVisualizer::greater)) return;
                    if (result) visualizer.visualizerSwap(left, right);
                }
            }
        }
    }

    for (size_t mergerSize = realSize / 2, lIter = 0; mergerSize > 1; mergerSize >>= 1, lIter++)
    {
        const size_t diff = (mergerSize >> 1);
        for (size_t l = 0; l < lIter; l++)
        {
            const size_t subblocks = ((mergerSize << (l + 1)) / diff - 1) * (realSize >> 1) / (mergerSize << (l + 1));
            for (size_t k = 0; k < subblocks; k++)
            {
                const size_t a = diff + k * mergerSize;
                for (size_t i = 0; i < mergerSize / 2; i++)
                {
                    size_t left = i + a;
                    size_t right = i + a + (realSize >> (l + 1)) - diff;
                    if (right < visualizer.getArrayLength())
                    {
                        if (!visualizer.visualizerCompare(left, right, result, SortVisualizer::greater)) return;
                        if (result) visualizer.visualizerSwap(left, right);
                    }
                }
            }
        }

        for (size_t k = 0; k < realSize - mergerSize; k += mergerSize)
        {
            const size_t a = diff + k;
            for (size_t i = 0; i < mergerSize / 2; i++)
            {
                size_t left = i + a, right = i + a + (mergerSize >> 1);
                if (right < visualizer.getArrayLength())
                {
                    if (!visualizer.visualizerCompare(left, right, result, SortVisualizer::greater)) return;
                    if (result) visualizer.visualizerSwap(left, right);
                }
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::PairwiseSortingNetwork::getAlgorithmName() const
{
    return name;
}
