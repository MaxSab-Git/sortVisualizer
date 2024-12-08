#include "BinaryInsertionSort.h"
#include "../misc/SortingHelpers.h"

const std::string sv::BinaryInsertionSort::name = "Binary insertion sort";

sv::BinaryInsertionSort::BinaryInsertionSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::BinaryInsertionSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    for (size_t i = 1; i < visualizer.getArrayLength(); i++)
    {
        unsigned int tmp = visualizer.getValue(i);
        size_t insertIndex = sv::binarySearchForInsertionSort(visualizer, 0, i, i);
        for (size_t j = i; j > insertIndex; j--)
        {
            visualizer.visualizerCopy(j, j - 1);
        }
        visualizer.visualizerWrite(insertIndex, tmp);
    }

    visualizer.visualizerDone();
}

const std::string& sv::BinaryInsertionSort::getAlgorithmName() const
{
	return name;
}
