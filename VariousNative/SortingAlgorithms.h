#pragma once
#include <utility>
#include <numeric>
#include <stack>
#include <bit>

namespace SortingAlgorithms
{
    template<typename T>
    T* binarySearchForInsertionSort(T* beg, T* end, const T& elem)
    {
        T* middle;
        while (end > beg)
        {
            middle = beg + ((end - beg) >> 1);
            if (*middle <= elem) beg = middle + 1;
            else end = middle;
        }

        return beg;
    }

    template<typename T>
    T* binarySearch(T* beg, T* end, const T& elem)
    {
        T* middle;
        while (end > beg)
        {
            middle = beg + ((end - beg) >> 1);
            if (*middle < elem) beg = middle + 1;
            else end = middle;
        }

        return beg;
    }

    template<typename T>
    T& findMedian(T& a, T& b, T& c)
    {
        if ((a < b && b < c) || (c < b && b < a))
            return b;

        else if ((b < a && a < c) || (c < a && a < b))
            return a;

        else
            return c;
    }

    size_t RoundToNextPowerOf2(size_t val)
    {
        val--;
        val |= (val >> 1);
        val |= (val >> 2);
        val |= (val >> 4);
        val |= (val >> 8);
        val |= (val >> 16);
        val++;
        return val;
    }

    size_t roundToPreviousPowerOf2(size_t val)
    {
        val = val | (val >> 1);
        val = val | (val >> 2);
        val = val | (val >> 4);
        val = val | (val >> 8);
        val = val | (val >> 16);
        return val - (val >> 1);
    }

    size_t getLeftChildNodeIndex(size_t i)
    {
        return 2 * i + 1;
    }

    size_t getRightChildNodeIndex(size_t i)
    {
        return 2 * i + 2;
    }

    size_t getParentNodeIndex(size_t i)
    {
        return (i - 1) / 2;
    }

    template<typename T>
    void shiftDown(T* beg, size_t i, T* end)
    {
        size_t child = getLeftChildNodeIndex(i);
        while (beg + child < end)
        {
            if (child + 1 + beg < end && beg[child + 1] > beg[child])
            {
                child += 1;
            }

            if (beg[child] > beg[i])
            {
                std::swap(beg[child], beg[i]);
                i = child;
            }
            else break;

            child = getLeftChildNodeIndex(i);
        }
    }

    template<typename T>
    void maxHeapify(T* beg, T* end)
    {
        for (T* i = beg + (end - beg) / 2; i > beg; i--)
        {
            shiftDown(beg, i - 1 - beg, end);
        }
    }

    template<typename T>
    void mergeArray(T* beg, T* middle, T* end, T* auxArr)
    {
        T *leftPtr = beg, *rightPtr = middle;
        for (T* i = auxArr; i < auxArr + (end - beg); i++)
        {
            if (leftPtr < middle && (rightPtr >= end || *leftPtr <= *rightPtr))
            {
                *i = *leftPtr;
                leftPtr++;
            }
            else
            {
                *i = *rightPtr;
                rightPtr++;
            }
        }
    }

    template<typename T>
    void timMerge(T* beg, T* middle, T* end, T* auxArr)
    {
        if (end < middle || middle < beg) return;

        if (middle - beg <= end - middle)
        {
            for (size_t i = 0; i < middle - beg; i++)
            {
                auxArr[i] = beg[i];
            }

            size_t leftPtr = 0, rightPtr = 0;
            for (T* i = beg; i < end; i++)
            {
                if (leftPtr < middle - beg && (rightPtr >= end - middle || auxArr[leftPtr] <= middle[rightPtr]))
                {
                    *i = auxArr[leftPtr];
                    leftPtr++;
                }
                else
                {
                    *i = middle[rightPtr];
                    rightPtr++;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < end - middle; i++)
            {
                auxArr[i] = middle[i];
            }

            size_t leftPtr = 0, rightPtr = 0;
            for (T* i = end; i > beg; i--)
            {
                if (leftPtr < middle - beg && (rightPtr >= end - middle || *(middle - leftPtr - 1) > auxArr[(end - middle) - rightPtr - 1]))
                {
                    i[-1] = *(middle - leftPtr - 1);
                    leftPtr++;
                }
                else
                {
                    i[-1] = auxArr[(end - middle) - rightPtr - 1];
                    rightPtr++;
                }
            }
        }
    }

    template<typename T>
    void reverse(T* beg, T* end)
    {
        end--;
        while (beg < end)
        {
           std::swap(*beg, *end);
           beg++;
           end--;
        }
    }

    template<typename T>
    void rotateReverse(T* beg, T* end, T* middle)
    {
        reverse(beg, middle);
        reverse(middle, end);
        reverse(beg, end);
    }

    template<typename T>
    void rotate(T* arr, size_t len, size_t gap)
    {
        size_t iteration = std::gcd(gap, len);
        size_t index, precIndex;
        for (size_t i = 0; i < iteration; i++)
        {
            precIndex = i;
            T tmp = arr[i];

            while (true)
            {
                index = precIndex + gap;
                if (index >= len) index -= len;
                if (index == i) break;
                arr[precIndex] = arr[index];
                precIndex = index;
            }

            arr[precIndex] = tmp;
        }
    }

    template<typename T>
    void rotateMergeArray(T* beg, T* middle, T* end)
    {
        if (end <= middle || middle <= beg) return;
        T* middleLeftArray = beg + (middle - beg) / 2;

        T* insertIndex = binarySearch(middle, end, *middleLeftArray);

        rotateReverse(middleLeftArray, insertIndex, middle);
        rotateMergeArray(beg, middleLeftArray, middleLeftArray + (insertIndex - middle));
        rotateMergeArray(middleLeftArray + (insertIndex - middle) + 1, insertIndex, end);
    }

    template<typename T>
    void inPlaceMergeArray(T* beg, T* middle, T* end)
    {
        T* leftPtr = beg, *rightPtr = middle;
        while (leftPtr < rightPtr && rightPtr < end)
        {
            if (*leftPtr <= *rightPtr)
            {
                leftPtr++;
            }
            else
            {
                T tmp = *rightPtr;
                for (T* j = rightPtr; j > leftPtr; j--)
                {
                    *j = *(j - 1);
                }
                *leftPtr = tmp;

                leftPtr++;
                rightPtr++;
            }
        }
    }

    template<typename T>
    bool checkSorted(T* arr, size_t len)
    {
        for (size_t i = 0; i < len - 1; i++)
        {
            if (arr[i] > arr[i + 1]) return false;
        }
        return true;
    }

    template<typename T>
    bool sameArray(T* arr, T* other, size_t len)
    {
        for (size_t i = 0; i < len; i++)
        {
            if (arr[i] != other[i]) return false;
        }
        return true;
    }

    template<typename T>
    void bubbleSort(T* beg, T* end)
    {
        for (T* i = end - 1; i > beg; i--)
        {
            for (T* j = beg; j < i; j++)
            {
                if (j[0] > j[1])
                {
                    std::swap(j[0], j[1]);
                }
            }
        }
    }

    template<typename T>
    void optimizedBubbleSort(T* beg, T* end)
    {
        T* sorted = end - 1;
        T* newSorted;
        while (sorted > beg)
        {
            newSorted = beg;
            for (T* i = beg; i < sorted; i++)
            {
                if (i[0] > i[1])
                {
                    std::swap(i[0], i[1]);
                    newSorted = i;
                }
            }
            sorted = newSorted;
        }
    }

    template<typename T>
    void cocktailShakerSort(T* beg, T* end)
    {
        size_t len = end - beg;
        for (T* i = beg; i < beg + (len >> 1); i++)
        {
            for (T* j = i; j < beg + (end - i) - 1; j++)
            {
                if (j[0] > j[1])
                {
                    std::swap(j[0], j[1]);
                }
            }

            for (T* j = beg + (end - i) - 1; j > i; j--)
            {
                if (j[0] < j[-1])
                {
                    std::swap(j[0], j[-1]);
                }
            }
        }
    }

    template<typename T>
    void optimizedCocktailShakerSort(T* beg, T* end)
    {
        T* leftSorted = end - 1, *rightSorted = beg;
        T* sorted;
        while (leftSorted > rightSorted)
        {
            sorted = rightSorted;
            for (T* j = rightSorted; j < leftSorted; j++)
            {
                if (j[0] > j[1])
                {
                    std::swap(j[0], j[1]);
                    sorted = j;
                }
            }
            leftSorted = sorted;

            if (leftSorted <= rightSorted) break;

            sorted = leftSorted;
            for (T* j = leftSorted; j > rightSorted; j--)
            {
                if (j[0] < j[-1])
                {
                    std::swap(j[0], j[-1]);
                    sorted = j;
                }
            }
            rightSorted = sorted;
        }
    }

    template<typename T>
    void oddEvenSort(T* beg, T* end)
    {
        bool unsorted = true;
        while (unsorted)
        {
            unsorted = false;
            for (size_t k = 0; k < 2; k++)
            {
                for (T* i = beg + k; i < end - 1; i += 2)
                {
                    if (i[0] > i[1])
                    {
                        std::swap(i[0], i[1]);
                        unsorted = true;
                    }
                }
            }
        }
    }

    template<typename T>
    void combSort(T* beg, T* end)
    {
        const double reductionFactor = 1.3;
        size_t gap = end - beg;
        bool unsorted = true;

        while (gap > 1 || unsorted)
        {
            unsorted = false;
            double newGap = static_cast<double>(gap) / reductionFactor;
            gap = (newGap <= 1.0) ? 1 : static_cast<size_t>(newGap);

            for (T* j = beg; j < end - gap; j++)
            {
                if (j[0] > j[gap])
                {
                    std::swap(j[0], j[gap]);
                    unsorted = true;
                }
            }
        }
    }

    template<typename T>
    void selectionSort(T* beg, T* end)
    {
        T* minIndex;
        for (; beg < end - 1; beg++)
        {
            minIndex = beg;
            for (T* j = beg + 1; j < end; j++)
            {
                if (*j < *minIndex)
                {
                    minIndex = j;
                }
            }
            std::swap(*beg, *minIndex);
        }
    }

    template<typename T>
    void doubleSelectionSort(T* beg, T* end)
    {
        T *minIndex, *maxIndex;
        for (T* istart = beg, *iend = end; istart < beg + (end - beg) / 2; istart++, iend--)
        {
            minIndex = istart;
            maxIndex = iend - 1;
            for (T* j = istart; j < iend; j++)
            {
                if (*j < *minIndex)
                {
                    minIndex = j;
                }
                else if (*j >= *maxIndex)
                {
                    maxIndex = j;
                }
            }
            std::swap(*istart, *minIndex);
            if (maxIndex == istart) maxIndex = minIndex;
            std::swap(*(iend - 1), *maxIndex);
        }
    }

    template<typename T>
    void exchangeSort(T* beg, T* end)
    {
        for (T* i = beg; i < end - 1; i++)
        {
            for (T* j = end - 1; j > i; j--)
            {
                if (*j < *i)
                {
                    std::swap(*i, *j);
                }
            }
        }
    }

    template<typename T>
    void cycleSort(T* beg, T* end)
    {
        T* lessCounter;
        for (T* i = beg; i < end; i++)
        {
            lessCounter = beg;
            for (T* j = beg; j < i; j++)
            {
                if (*j <= *i)
                {
                    lessCounter++;
                }
            }

            for (T* j = i + 1; j < end; j++)
            {
                if (*j <= *i)
                {
                    lessCounter++;
                }
            }

            while (i != lessCounter && *(lessCounter - 1) == *i) lessCounter--;
            if (i == lessCounter) continue;

            T tmp = *lessCounter;
            *lessCounter = *i;
            while (true)
            {
                lessCounter = beg;
                for (T* j = beg; j < i; j++)
                {
                    if (*j <= tmp)
                    {
                        lessCounter++;
                    }
                }

                for (T* j = i + 1; j < end; j++)
                {
                    if (*j <= tmp)
                    {
                        lessCounter++;
                    }
                }

                while (i != lessCounter && *lessCounter == tmp) lessCounter--;
                if (i == lessCounter) break;

                std::swap(tmp, *lessCounter);
            }

            *i = tmp;
        }
    }

    template<typename T>
    void heapSort(T* beg, T* end)
    {
        maxHeapify(beg, end);

        for (T* i = end - 1; i > beg; i--)
        {
            std::swap(beg[0], i[0]);
            shiftDown(beg, 0, i);
        }
    }

    template<typename T>
    void circleSort(T* beg, T* end)
    {
        bool unsorted = true;
        size_t firstCircleSize = RoundToNextPowerOf2(end - beg);
        while (unsorted)
        {
            unsorted = false;

            for (size_t circleSize = firstCircleSize; circleSize > 1; circleSize >>= 1)
            {
                for (T* k = beg; k < end; k += circleSize)
                {
                    for (T* i = k, *oppositeIndex = circleSize - 1 + k; i < oppositeIndex; i++, oppositeIndex--)
                    {
                        if (oppositeIndex < end && *i > *oppositeIndex)
                        {
                            std::swap(*i, *oppositeIndex);
                            unsorted = true;
                        }
                    }
                }
            }
        }
    }

    template<typename T>
    void insertionSort(T* beg, T* end)
    {
        for (T* i = beg + 1; i < end; i++)
        {
            if (i[-1] > i[0])
            {
                T tmp = i[0];
                T* insertIndex = beg;
                for (T* j = i; j > beg; j--)
                {
                    if (j[-1] > tmp)
                    {
                        j[0] = j[-1];
                    }
                    else
                    {
                        insertIndex = j;
                        break;
                    }
                }
                *insertIndex = tmp;
            }
        }
    }

    template<typename T>
    void reverseInsertionSort(T* beg, T* end)
    {
        for (T* i = beg + 1; i < end; i++)
        {
            if (i[-1] <= i[0])
            {
                T tmp = i[0];
                T* insertIndex = beg;
                for (T* j = i; j > beg; j--)
                {
                    if (j[-1] <= tmp)
                    {
                        j[0] = j[-1];
                    }
                    else
                    {
                        insertIndex = j;
                        break;
                    }
                }
                *insertIndex = tmp;
            }
        }
    }

    template<typename T>
    void binaryInsertionSort(T* beg, T* end)
    {
        for (T* i = beg + 1; i < end; i++)
        {
            if (i[-1] > i[0])
            {
                T tmp = i[0];
                T* insertIndex = binarySearchForInsertionSort(beg, i, tmp);
                for (T* j = i; j > insertIndex; j--)
                {
                    j[0] = j[-1];
                }
                *insertIndex = tmp;
            }
        }
    }

    template<typename T>
    void shellSort(T* beg, T* end)
    {
        const double reductionFactor = 2.3;
        size_t gap = end - beg;
        while (gap > 1)
        {
            double newGap = (double)gap / reductionFactor;
            gap = (newGap <= 1.0) ? 1 : (size_t)newGap;

            for (T* k = beg; k < beg + gap; k++)
            {
                for (T* i = k; i < end - gap; i += gap)
                {
                    if (i[0] > i[gap])
                    {
                        T tmp = i[gap];
                        T* insertIndex = k;
                        for (T* j = i + gap; j > k; j -= gap)
                        {
                            if (*(j - gap) > tmp)
                            {
                                j[0] = *(j - gap);
                            }
                            else
                            {
                                insertIndex = j;
                                break;
                            }
                        }
                        *insertIndex = tmp;
                    }
                }
            }
        }
    }

    template<typename T>
    void hybridCombSort(T* beg, T* end)
    {
        const double reductionFactor = 1.3;
        size_t gap = end - beg;
        bool unsorted = true;

        while (gap > 1)
        {
            unsorted = false;
            double newGap = static_cast<double>(gap) / reductionFactor;
            gap = (newGap <= 1.0) ? 1 : static_cast<size_t>(newGap);

            for (T* j = beg; j < end - gap; j++)
            {
                if (j[0] > j[gap])
                {
                    std::swap(j[0], j[gap]);
                    unsorted = true;
                }
            }
        }

        insertionSort(beg, end);
    }

    template<typename T>
    void quickSort(T* beg, T* end)
    {
        if (end - beg < 2) return;
        else
        {
            T *leftPtr = beg, *rightPtr = end;
            T& pivot = beg[0];

            while (true)
            {
                ++leftPtr;
                while (leftPtr < rightPtr && *leftPtr <= pivot) ++leftPtr;
                --rightPtr;
                while (leftPtr < rightPtr && *rightPtr > pivot) --rightPtr;

                if (leftPtr >= rightPtr) break;

                std::swap(*leftPtr, *rightPtr);
            }

            std::swap(pivot, *(leftPtr - 1));

            quickSort<T>(beg, leftPtr - 1);
            quickSort<T>(leftPtr, end);
        }
    }

    template<typename T>
    void quickSortLL(T* beg, T* end)
    {
        if (end - beg < 2) return;
        else
        {
            T* ptr = beg + 1;
            T& pivot = beg[0];

            for (T* i = ptr; i < end; i++)
            {
                if (*i <= pivot) std::swap(*(ptr++), *i);
            }

            std::swap(pivot, *(ptr - 1));

            quickSortLL<T>(beg, ptr - 1);
            quickSortLL<T>(ptr, end);
        }
    }

    template<typename T>
    void dualPivotQuickSort(T* beg, T* end)
    {
        if (end - beg < 2) return;
        else
        {
            T* leftPtr = beg + 1, *middlePtr = beg + 1, *rightPtr = end - 2;
            T& leftPivot = beg[0], &rightPivot = end[-1];

            if (leftPivot > rightPivot) std::swap(leftPivot, rightPivot);

            while (middlePtr <= rightPtr)
            {
                if (*middlePtr <= leftPivot)
                {
                    std::swap(*leftPtr, *middlePtr);
                    leftPtr++;
                    middlePtr++;
                }
                else if (*middlePtr > rightPivot)
                {
                    std::swap(*middlePtr, *rightPtr);
                    rightPtr--;
                }
                else
                {
                    middlePtr++;
                }
            }

            std::swap(leftPtr[-1], leftPivot);
            std::swap(middlePtr[0], rightPivot);

            dualPivotQuickSort(beg, leftPtr - 1);
            dualPivotQuickSort(leftPtr, middlePtr);
            dualPivotQuickSort(middlePtr + 1, end);
        }
    }

    template<typename T>
    void proportionExtendSort(T* beg, T* end)
    {
        if (end - beg < 2) return;
        else
        {
            size_t subListLen = (end - beg) >> 4;

            if (subListLen < 16) insertionSort(beg, beg + subListLen);
            else heapSort(beg, beg + subListLen);

            T* leftPtr = beg + (subListLen >> 1), *rightPtr = end;
            T& pivot = *leftPtr;

            while (true)
            {
                ++leftPtr;
                while (leftPtr < rightPtr && *leftPtr <= pivot) ++leftPtr;
                --rightPtr;
                while (leftPtr < rightPtr && *rightPtr > pivot) --rightPtr;

                if (leftPtr >= rightPtr) break;

                std::swap(*leftPtr, *rightPtr);
            }

            std::swap(pivot, *(leftPtr - 1));

            proportionExtendSort<T>(beg, leftPtr - 1);
            proportionExtendSort<T>(leftPtr, end);
        }
    }

    template<typename T>
    void introSort(T* beg, T* end, size_t recursions)
    {
        if (end - beg < 16) return;
        else if (recursions == 0)
        {
            heapSort(beg, end);
        }
        else
        {
            T* leftPtr = beg, * rightPtr = end;
            std::swap(beg[0], beg[(end - beg) / 2]);
            T& pivot = beg[0];

            while (true)
            {
                ++leftPtr;
                while (leftPtr < rightPtr && *leftPtr <= pivot) ++leftPtr;
                --rightPtr;
                while (leftPtr < rightPtr && *rightPtr > pivot) --rightPtr;

                if (leftPtr >= rightPtr) break;

                std::swap(*leftPtr, *rightPtr);
            }

            std::swap(pivot, *(leftPtr - 1));

            introSort<T>(beg, leftPtr - 1, recursions - 1);
            introSort<T>(leftPtr, end, recursions - 1);
        }
    }

    template<typename T>
    void introSort(T* beg, T* end)
    {
        introSort<T>(beg, end, static_cast<size_t>(2 * std::log2(end - beg)));
        insertionSort(beg, end);
    }

    template<typename T>
    void patternDefeatingQuickSort(T* beg, T* end, size_t recursions)
    {
        if (end - beg < 16) return;
        else if (recursions == 0)
        {
            heapSort(beg, end);
        }
        else
        {
            T* leftPtr = beg, * rightPtr = end;
            T* pivot = nullptr;
            {
                T* first = beg, *middle = beg + (end - beg) / 2, *last = end - 1;

                if (end - beg >= 128)
                {
                    pivot = &findMedian(findMedian(first[0], first[1], first[2]), findMedian(middle[-1], middle[0], middle[1]), findMedian(last[-2], last[-1], last[0]));
                }
                else pivot = &findMedian(*first, *middle, *last);
            }

            std::swap(beg[0], *pivot);
            pivot = beg;

            bool noSwap = true;
            while (true)
            {
                ++leftPtr;
                while (leftPtr < rightPtr && *leftPtr <= *pivot) ++leftPtr;
                --rightPtr;
                while (leftPtr < rightPtr && *rightPtr > *pivot) --rightPtr;

                if (leftPtr >= rightPtr) break;

                std::swap(*leftPtr, *rightPtr);
                noSwap = false;
            }

            std::swap(*pivot, *(leftPtr - 1));

            if (noSwap)
            {
                size_t swapCounter = 0;
                for (T* i = beg + 1; i < end; i++)
                {
                    if (i[-1] > i[0])
                    {
                        T tmp = i[0];
                        T* insertIndex = beg;
                        for (T* j = i; j > beg; j--)
                        {
                            if (j[-1] > tmp)
                            {
                                j[0] = j[-1];
                                if (++swapCounter >= 8)
                                {
                                    insertIndex = j - 1;
                                    break;
                                }
                            }
                            else
                            {
                                insertIndex = j;
                                break;
                            }
                        }
                        *insertIndex = tmp;
                    }
                    if (swapCounter >= 8) break;
                }
                if (swapCounter < 8) return;
            }

            patternDefeatingQuickSort<T>(beg, leftPtr - 1, recursions - 1);
            patternDefeatingQuickSort<T>(leftPtr, end, recursions - 1);
        }
    }

    template<typename T>
    void patternDefeatingQuickSort(T* beg, T* end)
    {
        patternDefeatingQuickSort(beg, end, static_cast<size_t>(2 * std::log2(end - beg)));
        insertionSort(beg, end);
    }

    template<typename T>
    void mergeSort(T* beg, T* end, T* auxArr)
    {
        if (end - beg < 2) return;
        else
        {
            size_t middle = (end - beg) / 2;

            mergeSort(auxArr, auxArr + middle, beg);
            mergeSort(auxArr + middle, auxArr + (end - beg), beg + middle);

            mergeArray(beg, beg + middle, end, auxArr);
        }
    }

    template<typename T>
    void mergeSort(T* beg, T* end)
    {
        T* auxArr = new T[end - beg];
        for (size_t i = 0; i < end - beg; i++)
        {
            auxArr[i] = beg[i];
        }
        mergeSort<T>(auxArr, auxArr + (end - beg), beg);
        delete[] auxArr;
    }

    template<typename T>
    void iterativeMergeSort(T* beg, T* end)
    {
        T* auxArr = new T[end - beg];
        T* const toDelete = auxArr;
        const double doubleLen = static_cast<double>(end - beg);
        const double scaleFactor = doubleLen / static_cast<double>(RoundToNextPowerOf2((end - beg) / 2));

        double partitionSize = scaleFactor * 0.5;
        while (partitionSize < doubleLen)
        {
            for (size_t j = 0; j < static_cast<size_t>(doubleLen / partitionSize); j += 2)
            {
                double jIndex = static_cast<double>(j) * partitionSize;
                size_t start = static_cast<size_t>(jIndex), middle = static_cast<size_t>(jIndex + partitionSize), end = static_cast<size_t>(jIndex + partitionSize * 2.0);

                mergeArray(beg + start, beg + middle, beg + end, auxArr + start);
            }

            std::swap(beg, auxArr);
            partitionSize *= 2.0;
        }

        if (toDelete != auxArr)
        {
            std::swap(beg, auxArr);
            for (size_t i = 0; i < end - beg; i++)
            {
                beg[i] = auxArr[i];
            }
        }

        delete[] toDelete;
    }

    template<typename T>
    void rotateMergeSort(T* beg, T* end)
    {
        const double doubleLen = static_cast<double>(end - beg);
        const double scaleFactor = doubleLen / static_cast<double>(RoundToNextPowerOf2((end - beg) / 2));

        double partitionSize = scaleFactor * 0.5;
        while (partitionSize < doubleLen)
        {
            for (size_t j = 0; j < static_cast<size_t>(doubleLen / partitionSize); j += 2)
            {
                double jIndex = static_cast<double>(j) * partitionSize;
                size_t start = static_cast<size_t>(jIndex), middle = static_cast<size_t>(jIndex + partitionSize), end = static_cast<size_t>(jIndex + partitionSize * 2.0);

                rotateMergeArray(beg + start, beg + middle, beg + end);
            }

            partitionSize *= 2.0;
        }
    }

    template<typename T>
    void inPlaceMergeSort(T* beg, T* end)
    {
        const double doubleLen = static_cast<double>(end - beg);
        const double scaleFactor = doubleLen / static_cast<double>(RoundToNextPowerOf2((end - beg) / 2));

        double partitionSize = scaleFactor * 0.5;
        while (partitionSize < doubleLen)
        {
            for (size_t j = 0; j < static_cast<size_t>(doubleLen / partitionSize); j += 2)
            {
                double jIndex = static_cast<double>(j) * partitionSize;
                size_t start = static_cast<size_t>(jIndex), middle = static_cast<size_t>(jIndex + partitionSize), end = static_cast<size_t>(jIndex + partitionSize * 2.0);

                inPlaceMergeArray(beg + start, beg + middle, beg + end);
            }

            partitionSize *= 2.0;
        }
    }

    template<typename T>
    void weaveSort(T* beg, T* end)
    {
        const double doubleLen = static_cast<double>(end - beg);
        const double scaleFactor = doubleLen / static_cast<double>(RoundToNextPowerOf2((end - beg) / 2));

        double partitionSize = scaleFactor * 0.5;
        while (partitionSize < doubleLen)
        {
            for (size_t j = 0; j < static_cast<size_t>(doubleLen / partitionSize); j += 2)
            {
                double jIndex = static_cast<double>(j) * partitionSize;
                size_t start = static_cast<size_t>(jIndex), middle = static_cast<size_t>(jIndex + partitionSize), end = static_cast<size_t>(jIndex + partitionSize * 2.0);

                T* leftPtr = beg + start + 1, * rightPtr = beg + middle;
                while (leftPtr < rightPtr)
                {
                    T tmp = *rightPtr;
                    for (T* i = rightPtr; i > leftPtr; i--)
                    {
                        *i = *(i - 1);
                    }
                    *leftPtr = tmp;

                    leftPtr += 2;
                    rightPtr++;
                }
                insertionSort(beg + start, beg + end);
            }

            partitionSize *= 2.0;
        }
    }

    template<typename T>
    void strandSort(T* beg, T* end)
    {
        T* solutionList = new T[end - beg];
        T* auxArr = new T[end - beg];

        size_t solutionSorted = 0;
        size_t solutionLen = 0;

        while (solutionLen < end - beg)
        {
            solutionList[solutionLen++] = beg[0];
            std::swap(beg[0], beg[end - beg - solutionLen]);

            for (size_t i = 0; i < end - beg - solutionLen;)
            {
                if (solutionList[solutionLen - 1] <= beg[i])
                {
                    solutionList[solutionLen++] = beg[i];
                    std::swap(beg[i], beg[end - beg - solutionLen]);
                }
                else i++;
            }

            mergeArray(solutionList, solutionList + solutionSorted, solutionList + solutionLen, auxArr);

            std::swap(auxArr, solutionList);
            solutionSorted = solutionLen;
        }

        for (size_t i = 0; i < end - beg; i++)
        {
            beg[i] = solutionList[i];
        }

        delete[] solutionList;
        delete[] auxArr;
    }

    template<typename T>
    void timSort(T* beg, T* end)
    {
        if (end - beg < 2) return;
        else
        {
            const size_t len = end - beg;
            size_t minRun = 32;
            T* auxArr = new T[len / 2];

            size_t less = roundToPreviousPowerOf2(len / minRun);
            for (minRun = 32; minRun < 64; minRun++)
            {
                size_t n = len / minRun;
                if (n < less) break;
            }

            std::stack<T*> subList;
            subList.push(beg);

            T* i = beg;
            while (i < end - 1)
            {
                T* runEnd = std::min(i + minRun, end);
                if (i[0] <= i[1])
                {
                    while (i < end - 1 && i[0] <= i[1])
                    {
                        i++;
                    }

                    if (i <= runEnd)
                    {
                        insertionSort(subList.top(), runEnd);
                        i = runEnd;
                    }
                }
                else
                {
                    while (i < end - 1 && i[0] >= i[1])
                    {
                        i++;
                    }

                    if (i <= runEnd)
                    {
                        reverseInsertionSort(subList.top(), runEnd);
                        i = runEnd;
                    }
                    reverse(subList.top(), i);
                }

                subList.push(i);

                while (subList.size() > 2)
                {
                    T* x, * y, * z;
                    T* endStack = subList.top();
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
                            timMerge(y, x, endStack, auxArr);
                            subList.push(y);
                            subList.push(endStack);
                        }
                        else
                        {
                            timMerge(z, y, x, auxArr);
                            subList.push(x);
                            subList.push(endStack);
                        }
                    }
                }
            }

            if (i < end)
            {
                subList.push(i + 1);
            }

            while (subList.size() > 2)
            {
                T* x, *y;
                T* endStack = subList.top();
                subList.pop();
                x = subList.top();
                subList.pop();
                y = subList.top();

                timMerge(y, x, endStack, auxArr);
                subList.push(endStack);
            }

            delete[] auxArr;
        }
    }

    template<typename T>
    void quadSort(T* beg, T* end)
    {
        const size_t len = end - beg;
        if (len >= 32)
        {
            T* auxArr = new T[len / 8];

            for (T* i = beg; i < end; i += 8)
            {
                insertionSort(i, std::min(i + 8, end));
            }

            for (size_t subLen = 8; subLen < len; subLen <<= 1)
            {
                if (4 * subLen <= len / 8)
                {
                    for (T* i = beg; i < end; i += subLen * 4)
                    {
                        mergeArray(i, i + subLen, i + 2 * subLen, auxArr);
                        mergeArray(i + 2 * subLen, i + 3 * subLen, i + 4 * subLen, auxArr + 2 * subLen);

                        mergeArray(auxArr, auxArr + 2 * subLen, auxArr + 4 * subLen, i);
                    }
                }
                else if (2 * subLen <= len / 8)
                {
                    for (T* i = beg; i < end; i += subLen * 2)
                    {
                        mergeArray(i, i + subLen, i + 2 * subLen, auxArr);
                        for (size_t j = 0; j < subLen * 2; j++)
                        {
                            i[j] = auxArr[j];
                        }
                    }
                }
                else if (subLen <= len / 8)
                {
                    for (T* i = beg; i < end; i += subLen * 2)
                    {
                        for (size_t j = 0; j < subLen; j++)
                        {
                            auxArr[j] = i[j];
                        }
                        //timMerge(i, i + subLen, i + subLen * 2, auxArr, auxArr + subLen);
                    }
                }
                else if (subLen <= len / 4)
                {
                    for (T* i = beg; i < end; i += subLen * 2)
                    {
                        T* insertIndex = binarySearch(i + subLen, i + subLen * 2, i[subLen / 2]);
                        rotateReverse(i + subLen / 2, insertIndex, i + subLen);
                        for (size_t j = 0; j < subLen / 2; j++)
                        {
                            auxArr[j] = i[j];
                        }
                        //timMerge(i, i + subLen / 2, subLen / 2 + (insertIndex - subLen), auxArr, auxArr + subLen / 2);

                        for (size_t j = 0; j < subLen / 2; j++)
                        {
                            auxArr[j] = i[j + subLen];
                        }
                        //timMerge(subLen / 2 + (insertIndex - subLen), insertIndex, i + subLen + subLen, auxArr, auxArr + subLen / 2);
                    }
                }
                else if (subLen <= len / 2)
                {
                    
                }
            }
        }
        else
        {
            insertionSort(beg, end);
        }
    }

    template<typename T>
    void bitonicSort(T* beg, T* end)
    {
        size_t realSize = RoundToNextPowerOf2(end - beg);

        for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
        {
            for (T* k = beg; k < beg + realSize; k += mergerSize)
            {
                for (size_t i = 0; i < (mergerSize >> 1); i++)
                {
                    T* left = k + i, *right = k + mergerSize - i - 1;
                    if (right < end && *left > *right) std::swap(*left, *right);
                }
            }

            for (size_t j = (mergerSize >> 1); j > 1; j >>= 1)
            {
                for (T* k = beg; k < beg + realSize; k += j)
                {
                    for (size_t i = 0; i < (j >> 1); i++)
                    {
                        T* left = k + i, *right = k + i + (j >> 1);
                        if (right < end && *left > *right) std::swap(*left, *right);
                    }
                }
            }
        }
    }

    template<typename T>
    void oddEvenMergeSort(T* beg, T* end)
    {
        size_t realSize = RoundToNextPowerOf2(end - beg);

        for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
        {
            for (size_t k = 0; k < realSize / mergerSize; k++)
            {
                T* a = beg + k * mergerSize;
                for (size_t i = 0; i < (mergerSize >> 1); i++)
                {
                    T* left = i + a, *right = i + a + (mergerSize >> 1);
                    if (right < end && *left > *right) std::swap(*left, *right);
                }
            }

            for (size_t j = (mergerSize >> 1); j > 1; j >>= 1)
            {
                size_t diff = (j >> 1);
                for (size_t r = 0; r < realSize / mergerSize; r++)
                {
                    for (size_t k = 0; k < mergerSize / j - 1; k++)
                    {
                        T* a = beg + k * j + r * mergerSize + diff;
                        for (size_t i = 0; i < (j >> 1); i++)
                        {
                            T* left = i + a, * right = i + a + (j >> 1);
                            if (right < end && *left > *right) std::swap(*left, *right);
                        }
                    }
                }
            }
        }
    }

    template<typename T>
    void pairwiseSortingNetwork(T* beg, T* end)
    {
        size_t len = end - beg;
        size_t realSize = RoundToNextPowerOf2(len);

        bool result;
        for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
        {
            for (size_t j = 0; j < len; j += mergerSize)
            {
                for (size_t i = 0; i < mergerSize >> 1; i++)
                {
                    T* left = beg + i + j, *right = beg + i + j + (mergerSize >> 1);
                    if (right < end && *left > *right)
                    {
                        std::swap(*left, *right);
                    }
                }
            }
        }

        for (size_t mergerSize = realSize >> 1, lIter = 0; mergerSize > 1; mergerSize >>= 1, lIter++)
        {
            const size_t diff = (mergerSize >> 1);
            for (size_t l = 0; l < lIter; l++)
            {
                const size_t subblocks = ((mergerSize << (l + 1)) / diff - 1) * (realSize >> 1) / (mergerSize << (l + 1));
                for (size_t k = 0; k < subblocks; k++)
                {
                    T* a = beg + diff + k * mergerSize;
                    for (size_t i = 0; i < diff; i++)
                    {
                        T* left = i + a, *right = i + a + (realSize >> (l + 1)) - diff;
                        if (right < end && *left > *right)
                        {
                            std::swap(*left, *right);
                        }
                    }
                }
            }

            for (size_t k = 0; k < realSize - mergerSize; k += mergerSize)
            {
                T* a = beg + diff + k;
                for (size_t i = 0; i < diff; i++)
                {
                    T* left = i + a, *right = i + a + diff;
                    if (right < end && *left > *right)
                    {
                        std::swap(*left, *right);
                    }
                }
            }
        }
    }

    template<typename T, size_t bits>
    void LSDRadixSort(T* beg, T* end)
    {
        static_assert(std::is_integral<T>::value && bits < sizeof(T) * 8 && (sizeof(T) * 8) % bits == 0);

        T* bucketArr = new T[end - beg];

        const size_t cursorsLen = static_cast<size_t>(1) << bits;
        size_t* cursors = new size_t[cursorsLen];
        size_t* counter = new size_t[cursorsLen];

        size_t len = end - beg;

        for (size_t k = 0; k < sizeof(T) * 8; k += bits)
        {
            typename std::make_unsigned<T>::type mask = (cursorsLen - 1) << k;

            for (size_t i = 0; i < cursorsLen; i++)
            {
                cursors[i] = 0;
                counter[i] = 0;
            }

            for (size_t i = 0; i < len; i++)
            {
                counter[(reinterpret_cast<std::make_unsigned<T>::type&>(beg[i]) & mask) >> k]++;
            }

            std::swap(beg, bucketArr);

            for (size_t i = 0; i < cursorsLen - 1; i++)
            {
                cursors[i + 1] = counter[i] + cursors[i];
            }

            for (size_t i = 0; i < len; i++)
            {
                size_t cursorIndex = (reinterpret_cast<std::make_unsigned<T>::type&>(bucketArr[i]) & mask) >> k;
                beg[cursors[cursorIndex]++] = bucketArr[i];
            }
        }

        delete[] bucketArr;
        delete[] cursors;
        delete[] counter;
    }

    template<typename T, size_t bits>
    void MSDRadixSort(T* beg, T* end, T* bucketArr, size_t* counter, size_t round)
    {
        static_assert(std::is_integral<T>::value && bits < sizeof(T) * 8 && (sizeof(T) * 8) % bits == 0);

        if ((end - beg) < 2 || round >= 32) return;

        const size_t cursorsLen = static_cast<size_t>(1) << bits;
        size_t* cursors = new size_t[cursorsLen];

        typename std::make_unsigned<T>::type mask = ((cursorsLen - 1) << (32 - bits - round));

        for (size_t i = 0; i < cursorsLen; i++)
        {
            cursors[i] = 0;
            counter[i] = 0;
        }

        for (size_t i = 0; i < (end - beg); i++)
        {
            counter[(reinterpret_cast<std::make_unsigned<T>::type&>(beg[i]) & mask) >> (32 - bits - round)]++;
        }

        for (size_t i = 0; i < (end - beg); i++)
        {
            bucketArr[i] = beg[i];
        }

        for (size_t i = 0; i < cursorsLen - 1; i++)
        {
            cursors[i + 1] = counter[i] + cursors[i];
        }

        for (size_t i = 0; i < (end - beg); i++)
        {
            size_t cursorIndex = (reinterpret_cast<std::make_unsigned<T>::type&>(bucketArr[i]) & mask) >> (32 - bits - round);
            beg[cursors[cursorIndex]++] = bucketArr[i];
        }

        size_t start = 0;
        for (size_t i = 0; i < cursorsLen; i++)
        {
            MSDRadixSort<T, bits>(beg + start, beg + cursors[i], bucketArr, counter, round + bits);
            start = cursors[i];
        }

        delete[] cursors;
    }

    template<typename T, size_t bits>
    void MSDRadixSort(T* beg, T* end)
    {
        static_assert(std::is_integral<T>::value && bits < sizeof(T) * 8 && (sizeof(T) * 8) % bits == 0);

        const size_t cursorsLen = static_cast<size_t>(1) << bits;

        T* bucketArr = new T[(end - beg)];
        size_t* counter = new size_t[cursorsLen];

        MSDRadixSort<T, bits>(beg, end, bucketArr, counter, 0);

        delete[] bucketArr;
        delete[] counter;
    }

    template<typename T, size_t bucketSize>
    void bucketSort(T* beg, T* end)
    {
        static_assert(std::is_integral<T>::value && bucketSize > 0);

        T max, min;
        T* maxIndex = beg, *minIndex = beg;
        for (T* i = beg; i < end; i++)
        {
            if (*maxIndex < *i)
            {
                maxIndex = i;
            }
            if (*minIndex > *i)
            {
                minIndex = i;
            }
        }

        if (minIndex == maxIndex) return;

        max = *maxIndex;
        min = *minIndex;

        T* bucketArr = new T[(end - beg)];

        const size_t cursorsLen = (end - beg) / bucketSize + 1;
        size_t* cursors = new size_t[cursorsLen];
        size_t* counter = new size_t[cursorsLen];

        size_t bucketSize = (max - min) / cursorsLen + 1;

        for (size_t i = 0; i < cursorsLen; i++)
        {
            cursors[i] = 0;
            counter[i] = 0;
        }

        for (size_t i = 0; i < (end - beg); i++)
        {
            counter[(beg[i] - min) / bucketSize]++;
        }

        for (size_t i = 0; i < cursorsLen - 1; i++)
        {
            cursors[i + 1] = counter[i] + cursors[i];
        }

        for (size_t i = 0; i < (end - beg); i++)
        {
            size_t cursorIndex = static_cast<size_t>((beg[i] - min) / bucketSize);
            bucketArr[cursors[cursorIndex]++] = beg[i];
        }

        insertionSort(bucketArr, bucketArr + counter[0]);
        for (size_t i = 1; i < cursorsLen; i++)
        {
            insertionSort(bucketArr + cursors[i - 1], bucketArr + cursors[i]);
        }

        for (size_t i = 0; i < (end - beg); i++)
        {
            beg[i] = bucketArr[i];
        }

        delete[] bucketArr;
        delete[] cursors;
        delete[] counter;
    }

    template<typename T>
    void binaryQuickSort(T* beg, T* end, size_t pivot)
    {
        if (pivot >= sizeof(T) * 8 || (end - beg) < 2) return;

        T* leftCursor = beg, *rightCursor = end;
        typename std::make_unsigned<T>::type mask = (1 << (sizeof(T) * 8 - 1 - pivot));

        while (true)
        {
            while (leftCursor < rightCursor && !(*leftCursor & mask)) ++leftCursor;
            --rightCursor;
            while (leftCursor < rightCursor && (*rightCursor & mask)) --rightCursor;

            if (leftCursor >= rightCursor) break;

            std::swap(*leftCursor, *rightCursor);
            ++leftCursor;
        }

        binaryQuickSort(beg, leftCursor, pivot + 1);
        binaryQuickSort(leftCursor, end, pivot + 1);
    }

    template<typename T>
    void binaryQuickSort(T* beg, T* end)
    {
        binaryQuickSort(beg, end, 0);
    }

    template<typename T>
    void pancakeSort(T* beg, T* end)
    {
        T* minIndex;
        for (; beg < end - 1; beg++)
        {
            minIndex = beg;
            for (T* j = beg + 1; j < end; j++)
            {
                if (*j < *minIndex)
                {
                    minIndex = j;
                }
            }
            reverse(minIndex, end);
            reverse(beg, end);
        }
    }

    template<typename T>
    void stoogeSort(T* beg, T* end)
    {
        if ((end - beg) > 2)
        {
            size_t splitSize = (end - beg) / 3;

            stoogeSort(beg, end - splitSize);
            stoogeSort(beg + splitSize, end);
            stoogeSort(beg, end - splitSize);
        }
        else
        {
            if (beg[0] > beg[1]) std::swap(beg[0], beg[1]);
        }
    }

    template<typename T>
    void slowSort(T* beg, T* end)
    {
        T* middle = beg + (end - beg) / 2;

        if (end - beg > 2)
        {
            slowSort(beg, middle);
            slowSort(middle, end);
        }

        if (end - beg >= 2)
        {
            if (middle[-1] > end[-1]) std::swap(middle[-1], end[-1]);
            slowSort(beg, end - 1);
        }
    }
}

namespace CUDASortingAlgorithms
{
    __declspec(dllimport) bool initCuda();
    __declspec(dllimport) void* allocCUDAArray(size_t size);
    __declspec(dllimport) void freeCUDAArray(void* devArr);
    __declspec(dllimport) void*& getDevArrForSorting();

    template<typename T>
    __declspec(dllimport) void parallelBitonicSort(T* beg, T* end);

    template<typename T>
    __declspec(dllimport) void parallelOddEvenMergeSort(T* beg, T* end);

    template<typename T>
    __declspec(dllimport) void parallelPairwiseSortingNetwork(T* beg, T* end);

    template void parallelBitonicSort<int>(int* beg, int* end);
    template void parallelBitonicSort<unsigned int>(unsigned int* beg, unsigned int* end);

    template void parallelOddEvenMergeSort<int>(int* beg, int* end);
    template void parallelOddEvenMergeSort<unsigned int>(unsigned int* beg, unsigned int* end);

    template void parallelPairwiseSortingNetwork<int>(int* arr, int* end);
    template void parallelPairwiseSortingNetwork<unsigned int>(unsigned int* arr, unsigned int* end);
}