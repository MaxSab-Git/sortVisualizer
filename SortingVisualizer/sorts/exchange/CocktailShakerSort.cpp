#include "CocktailShakerSort.h"

const std::string sv::CocktailShakerSort::name = "Cocktail shaker sort";

sv::CocktailShakerSort::CocktailShakerSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::CocktailShakerSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    bool result;
    for (size_t i = 0; i < (visualizer.getArrayLength() >> 1); i++)
    {
        for (size_t j = i; j < visualizer.getArrayLength() - i - 1; j++)
        {
            if (!visualizer.visualizerCompare(j, j + 1, result, SortVisualizer::greater)) return;
            if (result)
            {
                visualizer.visualizerSwap(j, j + 1);
            }
        }

        for (size_t j = visualizer.getArrayLength() - i - 2; j > i; j--)
        {
            if (!visualizer.visualizerCompare(j, j - 1, result, SortVisualizer::less)) return;
            if (result)
            {
                visualizer.visualizerSwap(j, j - 1);
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::CocktailShakerSort::getAlgorithmName() const
{
    return name;
}
