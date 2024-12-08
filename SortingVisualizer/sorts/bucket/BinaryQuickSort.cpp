#include "BinaryQuickSort.h"

const std::string sv::BinaryQuickSort::name = "Binary quick sort";

sv::BinaryQuickSort::BinaryQuickSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::BinaryQuickSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    unsigned int maxValue = 0;
    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        if (visualizer.isAborted()) return;
        maxValue = std::max(maxValue, visualizer.getValue(i));
    }

    unsigned int round = 0;

    for (; round < 32; round += 1)
    {
        typename std::make_unsigned<unsigned int>::type mask = 1 << (sizeof(unsigned int) * 8 - 1 - round);
        if (maxValue & mask) break;
    }

    if (round >= 32) return;

    subSort(visualizer, 0, visualizer.getArrayLength(), round);

    visualizer.visualizerDone();
}

const std::string& sv::BinaryQuickSort::getAlgorithmName() const
{
	return name;
}

void sv::BinaryQuickSort::subSort(SortVisualizer& visualizer, size_t start, size_t end, size_t pivot)
{
    if (pivot >= sizeof(unsigned int) * 8 || end - start < 2 || visualizer.isAborted()) return;
    else
    {
        size_t leftPtr = start, rightPtr = end;
        typename std::make_unsigned<unsigned int>::type mask = 1 << (sizeof(unsigned int) * 8 - 1 - pivot);

        while (true)
        {
            unsigned int result;

            while (leftPtr < rightPtr)
            {
                if (!visualizer.visualizerBitMask(leftPtr, mask, result)) return;
                if (!result) ++leftPtr;
                else break;
            }
            --rightPtr;
            while (leftPtr < rightPtr)
            {
                if (!visualizer.visualizerBitMask(rightPtr, mask, result)) return;
                if (result) --rightPtr;
                else break;
            }

            if (leftPtr >= rightPtr) break;

            visualizer.visualizerSwap(leftPtr, rightPtr);
        }

        subSort(visualizer, start, leftPtr, pivot + 1);
        subSort(visualizer, leftPtr, end, pivot + 1);
    }
}
