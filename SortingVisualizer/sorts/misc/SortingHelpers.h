#pragma once
#include "../../SortVisualizer.h"

namespace sv
{
    void insertionSort(SortVisualizer& visualizer, size_t start, size_t end);
    void reverseInsertionSort(SortVisualizer& visualizer, size_t start, size_t end);

    void shiftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t end);
    void heapSort(SortVisualizer& visualizer, size_t start, size_t end);

    void merge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end);
    void timMerge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end);
    void rotateMerge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end);
    void inPlaceMerge(SortVisualizer& visualizer, size_t start, size_t middle, size_t end);

    size_t roundToNextPowerOf2(size_t i);
    size_t roundToPreviousPowerOf2(size_t i);

    void rotate(SortVisualizer& visualizer, size_t start, size_t middle, size_t end);
    size_t binarySearch(SortVisualizer& visualizer, size_t start, size_t end, size_t elem);
    size_t binarySearchForInsertionSort(SortVisualizer& visualizer, size_t start, size_t end, size_t elem);
}