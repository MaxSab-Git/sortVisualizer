#include "CycleSort.h"

const std::string sv::CycleSort::name = "Cycle sort";

sv::CycleSort::CycleSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::CycleSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * visualizer.getArrayLength());
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    size_t lessCounter;
    bool result;
    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        lessCounter = i;

        for (size_t j = i + 1; j < visualizer.getArrayLength(); j++)
        {
            if (!visualizer.visualizerCompare(j, i, result, SortVisualizer::lessEquals)) return;
            if (result)
            {
                lessCounter++;
            }
        }

        while (i != lessCounter)
        {
            if (!visualizer.visualizerCompare(lessCounter, i, result, SortVisualizer::equals)) return;
            if (result) lessCounter--;
            else break;
        }

        if (i == lessCounter) continue;

        unsigned int tmp = visualizer.getValue(lessCounter);
        visualizer.visualizerCopy(lessCounter, i);
        while (true)
        {
            lessCounter = i;

            for (size_t j = i + 1; j < visualizer.getArrayLength(); j++)
            {
                if (!visualizer.visualizerExternCompare(j, tmp, result, SortVisualizer::lessEquals)) return;
                if (result)
                {
                    lessCounter++;
                }
            }

            while (i != lessCounter)
            {
                if (!visualizer.visualizerExternCompare(lessCounter, tmp, result, SortVisualizer::equals)) return;
                if (result) lessCounter--;
                else break;
            }
            if (i == lessCounter) break;

            unsigned int tmptmp = tmp;
            tmp = visualizer.getValue(lessCounter);
            visualizer.visualizerWrite(lessCounter, tmptmp);
        }

        visualizer.visualizerWrite(i, tmp);
    }

    visualizer.visualizerDone();
}

const std::string& sv::CycleSort::getAlgorithmName() const
{
	return name;
}
