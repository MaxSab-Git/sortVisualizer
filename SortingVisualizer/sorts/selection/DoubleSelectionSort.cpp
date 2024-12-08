#include "DoubleSelectionSort.h"

const std::string sv::DoubleSelectionSort::name = "Double selection sort";

sv::DoubleSelectionSort::DoubleSelectionSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{

}

void sv::DoubleSelectionSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    size_t minIndex, maxIndex;
    bool result;
    for (size_t istart = 0, iend = visualizer.getArrayLength(); istart < visualizer.getArrayLength() / 2; istart++, iend--)
    {
        minIndex = istart;
        maxIndex = iend - 1;
        for (size_t j = istart; j < iend; j++)
        {
            if (!visualizer.visualizerCompare(j, minIndex, result, SortVisualizer::less)) return;
            if (result)
            {
                minIndex = j;
            }
            else
            {
                visualizer.visualizerCompare(j, maxIndex, result, SortVisualizer::greaterEquals);
                if (result)
                {
                    maxIndex = j;
                }
            }
        }

        visualizer.visualizerSwap(istart, minIndex);
        if (maxIndex == istart) maxIndex = minIndex;
        visualizer.visualizerSwap(iend - 1, maxIndex);
    }

    visualizer.visualizerDone();
}

const std::string& sv::DoubleSelectionSort::getAlgorithmName() const
{
	return name;
}
