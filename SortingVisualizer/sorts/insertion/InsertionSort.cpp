#include "InsertionSort.h"

const std::string sv::InsertionSort::name = "Insertion sort";

sv::InsertionSort::InsertionSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{

}

void sv::InsertionSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    bool result;
    for (size_t i = 1; i < visualizer.getArrayLength(); i++)
    {
        if (!visualizer.visualizerCompare(i - 1, i, result, SortVisualizer::greater)) return;
        if (result)
        {
            unsigned int tmp = visualizer.getValue(i);
            size_t insertIndex = 0;
            for (size_t j = i; j > 0; j--)
            {
                if (!visualizer.visualizerExternCompare(j - 1, tmp, result, SortVisualizer::greater)) return;
                if (result)
                {
                    visualizer.visualizerCopy(j, j - 1);
                }
                else
                {
                    insertIndex = j;
                    break;
                }
            }
            visualizer.visualizerWrite(insertIndex, tmp);
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::InsertionSort::getAlgorithmName() const
{
    return name;
}
