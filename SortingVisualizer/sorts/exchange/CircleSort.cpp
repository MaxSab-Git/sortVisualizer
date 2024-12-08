#include "CircleSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::CircleSort::name = "Circle sort";

sv::CircleSort::CircleSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::CircleSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::powf(std::logf(static_cast<float>(visualizer.getArrayLength())), 2));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    bool result;
    bool unsorted = true;
    size_t firstCircleSize = sv::roundToNextPowerOf2(visualizer.getArrayLength());
    while (unsorted)
    {
        unsorted = false;

        for (size_t circleSize = firstCircleSize; circleSize > 1; circleSize >>= 1)
        {
            for (size_t k = 0; k < visualizer.getArrayLength(); k += circleSize)
            {
                for (size_t i = k, oppositeIndex = circleSize - 1 + k; i < oppositeIndex; i++, oppositeIndex--)
                {
                    if (oppositeIndex < visualizer.getArrayLength())
                    {
                        if (!visualizer.visualizerCompare(i, oppositeIndex, result, SortVisualizer::greater)) return;
                        if (result)
                        {
                            visualizer.visualizerSwap(i, oppositeIndex);
                            unsorted = true;
                        }
                    }
                }
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::CircleSort::getAlgorithmName() const
{
	return name;
}
