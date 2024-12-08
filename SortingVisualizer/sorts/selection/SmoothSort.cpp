#include "SmoothSort.h"

const std::string sv::SmoothSort::name = "Smooth sort";

sv::SmoothSort::SmoothSort(float timeMultiplier) : SortingAlgorithm(timeMultiplier)
{
}

void sv::SmoothSort::sort(SortVisualizer& visualizer)
{
    const float timer = getTimeMultiplier() / (visualizer.getArrayLength() * std::logf(static_cast<float>(visualizer.getArrayLength())));
    visualizer.setSwapDelay(timer);
    visualizer.setCompareDelay(timer);
    visualizer.setWriteDelay(timer);

    sf::Uint64 treeLevelMask = 0;
    size_t maxLevel = leonardoHeapify(visualizer, treeLevelMask);

    for (size_t i = visualizer.getArrayLength() - 1; i > 0; i--)
    {
        if (visualizer.isAborted()) return;
        for (size_t j = 0; (static_cast<sf::Uint64>(1) << j) <= treeLevelMask; j++)
        {
            if (treeLevelMask & (static_cast<sf::Uint64>(1) << j))
            {
                if (j > 1)
                {
                    treeLevelMask ^= (static_cast<sf::Uint64>(0b111) << (j - 2));

                    size_t index = 0;
                    for (size_t level = maxLevel; level > 0; level--)
                    {
                        if (treeLevelMask & (static_cast<sf::Uint64>(1) << (level - 1)))
                        {
                            index += getLevelSize(level - 1);
                            rootInsertion(visualizer, treeLevelMask, index, level - 1);
                        }
                    }
                }
                else
                {
                    treeLevelMask ^= (static_cast<sf::Uint64>(0b1) << j);
                }
                break;
            }
        }
    }

    visualizer.visualizerDone();
}

const std::string& sv::SmoothSort::getAlgorithmName() const
{
    return name;
}

size_t sv::SmoothSort::leonardoHeapify(SortVisualizer& visualizer, sf::Uint64& treeLevelMask)
{
    size_t maxLevel = 0;
    for (size_t i = 0; i < visualizer.getArrayLength(); i++)
    {
        if (!combineTree(treeLevelMask))
        {
            if (treeLevelMask & 0b10) treeLevelMask |= 0b1;
            else treeLevelMask |= 0b10;
        }

        if ((static_cast<sf::Uint64>(1) << maxLevel) < treeLevelMask)
        {
            maxLevel += 1;
        }

        size_t index = 0;
        for (size_t level = maxLevel; level > 0; level--)
        {
            if (treeLevelMask & (static_cast<sf::Uint64>(1) << (level - 1)))
            {
                index += getLevelSize(level - 1);
                rootInsertion(visualizer, treeLevelMask, index, level - 1);
            }
        }
    }

    return maxLevel;
}

bool sv::SmoothSort::combineTree(sf::Uint64& treeLevelMask)
{
    for (sf::Uint64 i = 0b11; i <= treeLevelMask; i <<= 1)
    {
        if ((treeLevelMask & i) == i)
        {
            treeLevelMask ^= i | (i << 1);
            return true;
        }
    }
    return false;
}

void sv::SmoothSort::rootInsertion(SortVisualizer& visualizer, const sf::Uint64& treeLevelMask, size_t index, size_t level)
{
    size_t leftIndex = index;
    size_t rightIndex = index;
    bool result;
    size_t rightLevel = level;
    for (size_t i = level; (static_cast<sf::Uint64>(1) << i) <= treeLevelMask; i++)
    {
        if (treeLevelMask & (static_cast<sf::Uint64>(1) << i))
        {
            size_t levelSize = getLevelSize(i);

            if (leftIndex >= levelSize)
            {
                leftIndex -= levelSize;

                if (!visualizer.visualizerCompare(leftIndex, rightIndex, result, SortVisualizer::greater)) return;
                if (result)
                {
                    if (i > 1)
                    {
                        visualizer.visualizerCompare(leftIndex, getRightChildNodeIndex(rightIndex), result, SortVisualizer::greater);
                        if (result)
                        {
                            visualizer.visualizerCompare(leftIndex, getLeftChildNodeIndex(rightIndex, i), result, SortVisualizer::greater);
                            if (result)
                            {
                                visualizer.visualizerSwap(leftIndex, rightIndex);
                                rightIndex = leftIndex;
                                rightLevel = i;
                            }
                        }
                    }
                    else
                    {
                        visualizer.visualizerSwap(leftIndex, rightIndex);
                        rightIndex = leftIndex;
                        rightLevel = i;
                    }
                }

                if (!result)
                {
                    rightLevel = i;
                    break;
                }
            }
        }
    }

    shiftDown(visualizer, 0, rightIndex, rightLevel);
}

void sv::SmoothSort::shiftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t level)
{
    bool result;

    size_t child = getLeftChildNodeIndex(i, level);
    while (level > 1)
    {
        if (!visualizer.visualizerCompare(start + getRightChildNodeIndex(i), start + child, result, SortVisualizer::greater)) return;
        if (result) child = getRightChildNodeIndex(i), level -= 1;

        visualizer.visualizerCompare(start + child, start + i, result, SortVisualizer::greater);
        if (result)
        {
            visualizer.visualizerSwap(start + child, start + i);
            i = child;
        }
        else break;

        level -= 1;
        child = getLeftChildNodeIndex(i, level);
    }
}

size_t sv::SmoothSort::getLevelSize(size_t level)
{
    if (level < 2) return 1;

    size_t prevLevelSize = 1;
    size_t levelSize = 1;

    for (size_t i = 1; i < level; i++)
    {
        size_t tmp = levelSize;
        levelSize = prevLevelSize + levelSize + 1;
        prevLevelSize = tmp;
    }

    return levelSize;
}

size_t sv::SmoothSort::getLeftChildNodeIndex(size_t i, size_t level)
{
    if (level < 2) return i;
    else return i - getLevelSize(level - 2) - 1;
}

size_t sv::SmoothSort::getRightChildNodeIndex(size_t i)
{
    return i - 1;
}
