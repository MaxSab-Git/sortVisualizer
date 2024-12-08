#include "LSDRadixSort.h"
#include <assert.h>

const std::string sv::LSDRadixSort::name = "LSD radix sort";

sv::LSDRadixSort::LSDRadixSort(size_t baseBitOffset, float timeMultiplier) : SortingAlgorithm(timeMultiplier), m_bits(baseBitOffset)
{
    assert(m_bits < sizeof(unsigned int) * 8 && (sizeof(unsigned int) * 8) % m_bits == 0 && m_bits > 0);
}

void sv::LSDRadixSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / visualizer.getArrayLength();
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    unsigned int maxValue = 0;
    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        if (visualizer.isAborted()) return;
        maxValue = std::max(maxValue, visualizer.getValue(i));
    }

    visualizer.createAuxiliaryArray(visualizer.getArrayLength());

    const size_t cursorsLen = static_cast<size_t>(1) << m_bits;
    size_t* cursors = new size_t[cursorsLen];
    size_t* counter = new size_t[cursorsLen];

    size_t len = visualizer.getArrayLength();

    unsigned int result;
    for (size_t k = 0; k < sizeof(unsigned int) * 8; k += m_bits)
    {
        typename std::make_unsigned<unsigned int>::type mask = (cursorsLen - 1) << k;

        for (size_t i = 0; i < cursorsLen; i++)
        {
            cursors[i] = 0;
            counter[i] = 0;
        }

        for (size_t i = 0; i < len; i++)
        {
            if (!visualizer.visualizerBitMask(i, mask, result, VisualOperation::ArrayAccess::Main)) goto freeBuckets;
            visualizer.visualizerCopy(i, i, VisualOperation::ArrayAccess::AuxMain);
            counter[result >> k]++;
        }

        for (size_t i = 0; i < cursorsLen - 1; i++)
        {
            cursors[i + 1] = counter[i] + cursors[i];
        }

        for (size_t i = 0; i < len; i++)
        {
            if (!visualizer.visualizerBitMask(i, mask, result, VisualOperation::ArrayAccess::Aux)) goto freeBuckets;
            size_t cursorIndex = result >> k;
            visualizer.visualizerCopy(cursors[cursorIndex]++, i, VisualOperation::ArrayAccess::MainAux);
        }

        if (maxValue <= mask) break;
    }

    visualizer.visualizerDone();

freeBuckets:

    delete[] cursors;
    delete[] counter;
}

const std::string& sv::LSDRadixSort::getAlgorithmName() const
{
    return name;
}
