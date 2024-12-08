#include "MSDRadixSort.h"

const std::string sv::MSDRadixSort::name = "MSD radix sort";

sv::MSDRadixSort::MSDRadixSort(size_t baseBitOffset, float timeMultiplier) : SortingAlgorithm(timeMultiplier), m_bits(baseBitOffset)
{
}

void sv::MSDRadixSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / visualizer.getArrayLength();
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    const size_t cursorsLen = static_cast<size_t>(1) << m_bits;

    unsigned int maxValue = 0;
    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        if (visualizer.isAborted()) return;
        maxValue = std::max(maxValue, visualizer.getValue(i));
    }

    unsigned int round = 0;

    for (; round + m_bits <= 32; round += m_bits)
    {
        typename std::make_unsigned<unsigned int>::type mask = ((cursorsLen - 1) << (sizeof(unsigned int) * 8 - m_bits - round));
        if (maxValue & mask) break;
    }

    if (round + m_bits > 32) return;

    visualizer.createAuxiliaryArray(visualizer.getArrayLength());
    size_t* counter = new size_t[cursorsLen];

    subSort(visualizer, 0, visualizer.getArrayLength(), counter, round);

    visualizer.visualizerDone();

    delete[] counter;
}

const std::string& sv::MSDRadixSort::getAlgorithmName() const
{
	return name;
}

void sv::MSDRadixSort::subSort(SortVisualizer& visualizer, size_t start, size_t end, size_t* counter, unsigned int round)
{
    if ((end - start) < 2 || round + m_bits > sizeof(unsigned int) * 8 || visualizer.isAborted()) return;

    const size_t cursorsLen = static_cast<size_t>(1) << m_bits;
    size_t* cursors = new size_t[cursorsLen];

    typename std::make_unsigned<unsigned int>::type mask = ((cursorsLen - 1) << (sizeof(unsigned int) * 8 - m_bits - round));

    unsigned int result;

    for (size_t i = 0; i < cursorsLen; i++)
    {
        cursors[i] = 0;
        counter[i] = 0;
    }

    for (size_t i = start; i < end; i++)
    {
        if (!visualizer.visualizerBitMask(i, mask, result, VisualOperation::ArrayAccess::Main)) goto freeBuckets;
        visualizer.visualizerCopy(i, i, VisualOperation::ArrayAccess::AuxMain);
        counter[result >> (sizeof(unsigned int) * 8 - m_bits - round)]++;
    }

    for (size_t i = 0; i < cursorsLen - 1; i++)
    {
        cursors[i + 1] = counter[i] + cursors[i];
    }

    for (size_t i = start; i < end; i++)
    {
        if (!visualizer.visualizerBitMask(i, mask, result, VisualOperation::ArrayAccess::Aux)) goto freeBuckets;
        size_t cursorIndex = result >> (sizeof(unsigned int) * 8 - m_bits - round);
        visualizer.visualizerCopy(start + cursors[cursorIndex]++, i, VisualOperation::ArrayAccess::MainAux);
    }

    {
        size_t newStart = start;
        for (size_t i = 0; i < cursorsLen; i++)
        {
            subSort(visualizer, newStart, start + cursors[i], counter, round + m_bits);
            newStart = start + cursors[i];
        }
    }

freeBuckets:

    delete[] cursors;
}
