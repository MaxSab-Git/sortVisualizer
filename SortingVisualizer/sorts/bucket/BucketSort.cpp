#include "BucketSort.h"
#include "../misc/SortingHelpers.h"
#include <assert.h>

const std::string sv::BucketSort::name = "Bucket sort";

sv::BucketSort::BucketSort(size_t bucketSize, float timeMultiplier) : SortingAlgorithm(timeMultiplier), m_bucketSize(bucketSize)
{
}

void sv::BucketSort::sort(SortVisualizer& visualizer)
{
    assert(m_bucketSize > 0);

    const float timer = getTimeMultiplier() / visualizer.getArrayLength();
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    unsigned int max, min;
    size_t maxIndex = 0, minIndex = 0;
    bool result;
    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        if (!visualizer.visualizerCompare(maxIndex, i, result, SortVisualizer::less)) return;
        if (result)
        {
            maxIndex = i;
        }
        visualizer.visualizerCompare(minIndex, i, result, SortVisualizer::greater);
        if (result)
        {
            minIndex = i;
        }
    }

    if (minIndex == maxIndex) return;

    max = visualizer.getValue(maxIndex);
    min = visualizer.getValue(minIndex);

    visualizer.createAuxiliaryArray(visualizer.getArrayLength());

    const size_t cursorsLen = visualizer.getArrayLength() / m_bucketSize + 1;
    size_t* cursors = new size_t[cursorsLen];
    size_t* counter = new size_t[cursorsLen];

    size_t bucketSize = (max - min) / cursorsLen + 1;

    for (size_t i = 0; i < cursorsLen; i++)
    {
        cursors[i] = 0;
        counter[i] = 0;
    }

    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        if (visualizer.isAborted()) goto freeBuckets;
        counter[static_cast<size_t>(visualizer.getValue(i) - min) / bucketSize]++;
        visualizer.visualizerCopy(i, i, VisualOperation::ArrayAccess::AuxMain);
    }

    for (size_t i = 0; i < cursorsLen - 1; i++)
    {
        cursors[i + 1] = counter[i] + cursors[i];
    }

    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        size_t cursorIndex = static_cast<size_t>(visualizer.getValue(i, VisualOperation::ArrayAccess::Aux) - min) / bucketSize;
        if (!visualizer.visualizerCopy(cursors[cursorIndex]++, i, sv::VisualOperation::ArrayAccess::MainAux)) goto freeBuckets;
    }

    sv::insertionSort(visualizer, 0, counter[0]);
    for (size_t i = 1; i < cursorsLen; i++)
    {
        if (visualizer.isAborted()) goto freeBuckets;
        sv::insertionSort(visualizer, cursors[i - 1], cursors[i]);
    }

freeBuckets:

    delete[] cursors;
    delete[] counter;

    visualizer.visualizerDone();
}

const std::string& sv::BucketSort::getAlgorithmName() const
{
	return name;
}
