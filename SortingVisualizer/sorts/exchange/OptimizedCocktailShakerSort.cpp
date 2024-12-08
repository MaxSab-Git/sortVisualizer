#include "OptimizedCocktailShakerSort.h"


const std::string sv::OptimizedCocktailShakerSort::name = "Optimized cocktail shaker sort";

sv::OptimizedCocktailShakerSort::OptimizedCocktailShakerSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::OptimizedCocktailShakerSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    size_t leftSorted = visualizer.getArrayLength() - 1, rightSorted = 0;
    size_t sorted;
    bool result;
    while (leftSorted > rightSorted)
    {
        sorted = rightSorted;
        for (size_t i = rightSorted; i < leftSorted; i++)
        {
            if (!visualizer.visualizerCompare(i, i + 1, result, SortVisualizer::greater)) return;
            if (result)
            {
                visualizer.visualizerSwap(i, i + 1);
                sorted = i;
            }
        }
        leftSorted = sorted;

        if (leftSorted <= rightSorted) break;

        sorted = leftSorted;
        for (size_t i = leftSorted; i > rightSorted; i--)
        {
            if (!visualizer.visualizerCompare(i, i - 1, result, SortVisualizer::less)) return;
            if (result)
            {
                visualizer.visualizerSwap(i, i - 1);
                sorted = i;
            }
        }
        rightSorted = sorted;
    }

    visualizer.visualizerDone();
}

const std::string& sv::OptimizedCocktailShakerSort::getAlgorithmName() const
{
	return name;
}
