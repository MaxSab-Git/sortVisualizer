#include "MinHeapSort.h"

const std::string sv::MinHeapSort::name = "Min heap sort";

sv::MinHeapSort::MinHeapSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::MinHeapSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    minHeapify(visualizer);

    for (size_t i = 0; i < visualizer.getArrayLength() - 1; i++)
    {
        if (!visualizer.visualizerSwap(visualizer.getArrayLength() - 1, i)) return;
        shiftDown(visualizer, i + 1, 0, visualizer.getArrayLength());
    }

    visualizer.visualizerDone();
}

const std::string& sv::MinHeapSort::getAlgorithmName() const
{
    return name;
}

void sv::MinHeapSort::minHeapify(SortVisualizer& visualizer)
{
    for (size_t i = visualizer.getArrayLength() / 2; i > 0; i--)
    {
        shiftDown(visualizer, 0, i - 1, visualizer.getArrayLength());
        if (visualizer.isAborted()) return;
    }
}

void sv::MinHeapSort::shiftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t end)
{
    bool result;

    size_t child = getLeftChildNodeIndex(i);
    while (start + child < end)
    {
        if (start + child + 1 < end)
        {
            if (!visualizer.visualizerCompare(end - (child + 1) - 1, end - child - 1, result, SortVisualizer::less)) return;
            if (result) child += 1;
        }

        visualizer.visualizerCompare(end - child - 1, end - i - 1, result, SortVisualizer::less);
        if (result)
        {
            visualizer.visualizerSwap(end - child - 1, end - i - 1);
            i = child;
        }
        else break;

        child = getLeftChildNodeIndex(i);
    }
}

size_t sv::MinHeapSort::getLeftChildNodeIndex(size_t i)
{
    return 2 * i + 1;
}
