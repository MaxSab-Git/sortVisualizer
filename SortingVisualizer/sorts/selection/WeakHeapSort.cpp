#include "WeakHeapSort.h"

const std::string sv::WeakHeapSort::name = "Weak heap sort";

sv::WeakHeapSort::WeakHeapSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::WeakHeapSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    const size_t arrayLength = visualizer.getArrayLength();
    bool* flipped = new bool[arrayLength];

    for (size_t i = 0; i < arrayLength; i++)
    {
        flipped[i] = false;
    }

    weakHeapify(visualizer, flipped);

    for (size_t i = arrayLength - 1; i > 0; i--)
    {
        if (!visualizer.visualizerSwap(0, i)) goto freeFlipped;
        siftDown(visualizer, 0, 0, i, flipped);
    }

freeFlipped:

    delete[] flipped;

    visualizer.visualizerDone();
}

const std::string& sv::WeakHeapSort::getAlgorithmName() const
{
	return name;
}

void sv::WeakHeapSort::weakHeapify(SortVisualizer& visualizer, bool* flipped)
{
    for (size_t i = visualizer.getArrayLength() - 1; i > 0; i--)
    {
        join(visualizer, 0, i, flipped);
        if (visualizer.isAborted()) return;
    }
}

void sv::WeakHeapSort::siftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t end, bool* flipped)
{
    size_t child = getRightChildNodeIndex(i, flipped);
    while (start + child < end)
    {
        i = child;
        child = getLeftChildNodeIndex(i, flipped);
    }

    while (i > 0)
    {
        if (visualizer.isAborted()) return;
        join(visualizer, start, i, flipped);
        i >>= 1;
    } 
}

size_t sv::WeakHeapSort::findDistinguishedAncestor(SortVisualizer& visualizer, size_t start, size_t i, bool* flipped)
{
    size_t parent = i >> 1;
    while ((flipped[parent] && (i & 1) == 1) || (!flipped[parent] && (i & 1) == 0))
    {
        i = parent;
        parent >>= 1;
    }
    return parent;
}

void sv::WeakHeapSort::join(SortVisualizer& visualizer, size_t start, size_t i, bool* flipped)
{
    size_t da = findDistinguishedAncestor(visualizer, start, i, flipped);
    bool result;
    visualizer.visualizerCompare(start + da, start + i, result, SortVisualizer::less);
    if (result)
    {
        flipped[i] = !flipped[i]; 
        visualizer.visualizerSwap(start + da, start + i);
    }
}

size_t sv::WeakHeapSort::getLeftChildNodeIndex(size_t i, bool* flipped)
{
    if (flipped[i]) return 2 * i + 1;
    else return 2 * i;
}

size_t sv::WeakHeapSort::getRightChildNodeIndex(size_t i, bool* flipped)
{
    if (flipped[i]) return 2 * i;
    else return 2 * i + 1;
}
