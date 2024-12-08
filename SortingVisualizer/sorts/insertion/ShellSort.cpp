#include "ShellSort.h"

const std::string sv::ShellSort::name = "Shell sort";

sv::ShellSort::ShellSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::ShellSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / std::powf(static_cast<float>(visualizer.getArrayLength()), 1.2f);
    visualizer.setSwapDelay(0.f);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    const double reductionFactor = 2.3;
    size_t gap = visualizer.getArrayLength();

    bool result;
    while (gap > 1)
    {
        double newGap = (double)gap / reductionFactor;
        gap = (newGap <= 1.0) ? 1 : (size_t)newGap;

        for (size_t k = 0; k < gap; k++)
        {
            for (size_t i = k; i < visualizer.getArrayLength() - gap; i += gap)
            {
                if (!visualizer.visualizerCompare(i, i + gap, result, SortVisualizer::greater)) return;
                if (result)
                {
                    unsigned int tmp = visualizer.getValue(i + gap);
                    size_t insertIndex = k;
                    for (size_t j = i + gap; j > k; j -= gap)
                    {
                        if (!visualizer.visualizerExternCompare(j - gap, tmp, result, SortVisualizer::greater)) return;
                        if (result)
                        {
                            visualizer.visualizerCopy(j, j - gap);
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
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::ShellSort::getAlgorithmName() const
{
    return name;
}
