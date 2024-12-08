#include "ExchangeSort.h"

const std::string sv::ExchangeSort::name = "Exchange sort";

sv::ExchangeSort::ExchangeSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::ExchangeSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(0.f);

    bool result;
    for (size_t i = 0; i < visualizer.getArrayLength() - 1; i++)
    {
        for (size_t j = i + 1; j < visualizer.getArrayLength(); j++)
        {
            if (!visualizer.visualizerCompare(j, i, result, SortVisualizer::less)) return;
            if (result)
            {
                visualizer.visualizerSwap(j, i);
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::ExchangeSort::getAlgorithmName() const
{
    return name;
}
