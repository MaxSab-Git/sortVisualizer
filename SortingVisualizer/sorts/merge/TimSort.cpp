#include "TimSort.h"
#include "../misc/SortingHelpers.h"
#include <stack>

const std::string sv::TimSort::name = "Tim sort";

sv::TimSort::TimSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::TimSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    if (visualizer.getArrayLength() < 2) return;
    else
    {
        const size_t len = visualizer.getArrayLength();
        size_t minRun = 32;
        visualizer.createAuxiliaryArray(visualizer.getArrayLength() / 2);

        size_t less = sv::roundToPreviousPowerOf2(len / minRun);
        for (minRun = 32; minRun < 64; minRun++)
        {
            size_t n = len / minRun;
            if (n < less) break;
        }

        std::stack<size_t> subList;
        subList.push(0);

        bool result;
        size_t i = 0;
        while (i < len - 1)
        {
            size_t runEnd = std::min(i + minRun, len);
            if (!visualizer.visualizerCompare(i, i + 1, result, SortVisualizer::lessEquals)) return;
            if (result)
            {
                i++;
                while (i < len)
                {
                    if (!visualizer.visualizerCompare(i - 1, i, result, SortVisualizer::lessEquals)) return;
                    if (result) i++;
                    else break;
                }

                if (i < runEnd)
                {
                    insertionSort(visualizer, subList.top(), runEnd);
                    i = runEnd;
                }
            }
            else
            {
                i++;
                while (i < len)
                {
                    if (!visualizer.visualizerCompare(i - 1, i, result, SortVisualizer::greaterEquals)) return;
                    if (result) i++;
                    else break;
                }

                if (i < runEnd)
                {
                    reverseInsertionSort(visualizer, subList.top(), runEnd);
                    i = runEnd;
                }
                visualizer.visualizerReverse(subList.top(), i);
            }

            subList.push(i);

            while (subList.size() > 2 && !visualizer.isAborted())
            {
                size_t x, y, z;
                size_t endStack = subList.top();
                subList.pop();
                x = subList.top();
                subList.pop();
                y = subList.top();

                if (subList.size() == 1)
                {
                    z = y;
                    subList.pop();
                }
                else
                {
                    subList.pop();
                    z = subList.top();
                }

                const size_t lenX = endStack - x, lenY = x - y, lenZ = y - z;

                if (lenY > lenX && (lenZ == 0 || lenZ > lenY + lenX))
                {
                    subList.push(y);
                    subList.push(x);
                    subList.push(endStack);
                    break;
                }
                else
                {
                    if (lenZ == 0 || lenZ > lenX)
                    {
                        sv::timMerge(visualizer, y, x, endStack);
                        subList.push(y);
                        subList.push(endStack);
                    }
                    else
                    {
                        sv::timMerge(visualizer, z, y, x);
                        subList.push(x);
                        subList.push(endStack);
                    }
                }
            }
        }

        if (i < len)
        {
            subList.push(i + 1);
        }

        while (subList.size() > 2 && !visualizer.isAborted())
        {
            size_t x, y;
            size_t endStack = subList.top();
            subList.pop();
            x = subList.top();
            subList.pop();
            y = subList.top();

            sv::timMerge(visualizer, y, x, endStack);
            subList.push(endStack);
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::TimSort::getAlgorithmName() const
{
	return name;
}
