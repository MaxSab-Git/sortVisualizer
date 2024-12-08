#include "HeapSort.h"

const std::string sv::HeapSort::name = "Heap sort";

sv::HeapSort::HeapSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::HeapSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    maxHeapify(visualizer);

    for (size_t i = visualizer.getArrayLength() - 1; i > 0; i--)
    {
        if (!visualizer.visualizerSwap(0, i)) return;
        shiftDown(visualizer, 0, 0, i);
    }

    visualizer.visualizerDone();
}

const std::string& sv::HeapSort::getAlgorithmName() const
{
	return name;
}

void sv::HeapSort::maxHeapify(SortVisualizer& visualizer)
{
    for (size_t i = visualizer.getArrayLength() / 2; i > 0; i--)
    {
        shiftDown(visualizer, 0, i - 1, visualizer.getArrayLength());
        if (visualizer.isAborted()) return;
    }
}

void sv::HeapSort::shiftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t end)
{
    bool result;

    size_t child = getLeftChildNodeIndex(i);
    while (start + child < end)
    {
        if (start + child + 1 < end)
        {
            if (!visualizer.visualizerCompare(start + child + 1, start + child, result, SortVisualizer::greater)) return;
            if (result) child += 1;
        }

        visualizer.visualizerCompare(start + child, start + i, result, SortVisualizer::greater);
        if (result)
        {
            visualizer.visualizerSwap(start + child, start + i);
            i = child;
        }
        else break;

        child = getLeftChildNodeIndex(i);
    }
}

size_t sv::HeapSort::getLeftChildNodeIndex(size_t i)
{
    return 2 * i + 1;
}
