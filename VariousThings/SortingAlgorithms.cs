using System.Reflection;
using System.Security.Cryptography.X509Certificates;

namespace VariousThings
{
    public static class SortingAlgorithms
    {
        public static int MaxRecursiveCall { set; get; } = 1000;
        private static int recursiveCall = 0;

        private static int radixBitShift = 2;
        public static int RadixBitShift
        {
            set
            {
                radixBitShift = RoundToNextPowerOf2(value - 1);
            }
            get
            {
                return radixBitShift;
            }
        }

        private static int bucketRatio = 10;

        private static void IncrementRecursiveCall()
        {
            if (++recursiveCall > MaxRecursiveCall)
            {
                recursiveCall = 0;
                throw new Exception("Too much recursive call.");
            }
        }

        private static int GetLeftChildNodeIndex(int i)
        {
            return 2 * i + 1;
        }

        private static int GetRightChildNodeIndex(int i)
        {
            return 2 * i + 2;
        }

        private static int GetParentNodeIndex(int i)
        {
            return (i - 1) / 2;
        }

        private static void MaxHeapify<T>(T[] arr) where T : IComparable<T>
        {
            for (int i = arr.Length / 2; i > 0; i--)
            {
                ShiftDown(arr, i - 1, arr.Length);
            }
        }

        private static void ShiftDown<T>(T[] arr, int i, int end) where T : IComparable<T>
        {
            int child = GetLeftChildNodeIndex(i);
            while (child < end)
            {
                if (child + 1 < end && arr[child + 1].CompareTo(arr[child]) > 0)
                {
                    child += 1;
                }

                if (arr[child].CompareTo(arr[i]) > 0)
                {
                    Swap(ref arr[child], ref arr[i]);
                    i = child;
                }
                else break;
                
                child = GetLeftChildNodeIndex(i);
            }
        }

        private static int RoundToNextPowerOf2(int val)
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

        private static int GCD(int a, int b)
        {
            int shift, tmp;

            if (a == 0) return b;
            if (b == 0) return a;

            // Find power of two divisor
            for (shift = 0; ((a | b) & 1) == 0; shift++) { a >>= 1; b >>= 1; }

            // Remove remaining factors of two from a - they are not common
            while ((a & 1) == 0) a >>= 1;

            do
            {
                // Remove remaining factors of two from b - they are not common
                while ((b & 1) == 0) b >>= 1;

                if (a > b) { tmp = a; a = b; b = tmp; } // swap a,b
                b = b - a;
            }
            while (b != 0);

            return a << shift;
        }

        private static void Reverse<T>(T[] arr, int start, int end)
        {
            int mid = (end - start) >> 1;
            for (int i = 0; i < mid; i++)
            {
                Swap(ref arr[start + i], ref arr[end - 1 - i]);
            }
        }

        private static void Rotate<T>(T[] arr, int start, int mid, int end)
        {
            Reverse(arr, start, mid);
            Reverse(arr, mid, end);
            Reverse(arr, start, end);

            //int gap = mid - start;
            //int length = end - start;
            //if (gap == 0 || length < 2) return;

            //int iteration = GCD(gap, length);
            //for (int i = 0; i < iteration; i++)
            //{
            //    int cycleStart = start + i;
            //    int index;
            //    int precIndex = i;
            //    T tmp = arr[cycleStart];

            //    while (true)
            //    {
            //        index = (precIndex + gap) % length;
            //        if (index == i) break;
            //        arr[start + precIndex] = arr[start + index];
            //        precIndex = index;
            //    }

            //    arr[start + precIndex] = tmp;
            //}
        }

        private static void MergeArray<T>(T[] arr, int start, int middle, int end, T[] auxArr) where T : IComparable<T>
        {
            int leftIndex = start, rightIndex = middle;
            for (int i = start; i < end; i++)
            {
                if (leftIndex < middle && (rightIndex >= end || arr[leftIndex].CompareTo(arr[rightIndex]) <= 0))
                {
                    auxArr[i] = arr[leftIndex++];
                }
                else
                {
                    auxArr[i] = arr[rightIndex++];
                }
            }
        }

        private static void RotateMergeArray<T>(T[] arr, int start, int middle, int end) where T :IComparable<T>
        {
            if (end <= middle || middle <= start) return;
            int middleLeftArray = (middle + start) >> 1;

            int insertIndex = BinarySearch(arr, middle, end, arr[middleLeftArray]);

            Rotate(arr, middleLeftArray, middle, insertIndex);
            RotateMergeArray(arr, start, middleLeftArray, middleLeftArray + insertIndex - middle);
            RotateMergeArray(arr, middleLeftArray + insertIndex + 1 - middle, insertIndex, end);
        }

        private static void InPlaceMergeArray<T>(T[] arr, int start, int middle, int end) where T : IComparable<T>
        {
            int leftIndex = start, rightIndex = middle;
            while (leftIndex < rightIndex && rightIndex < end)
            {
                if (leftIndex < middle && (rightIndex >= end || arr[leftIndex].CompareTo(arr[rightIndex]) < 0))
                {
                    leftIndex++;
                }
                else
                {
                    T tmp = arr[rightIndex];
                    int insertIndex = start;
                    for (int j = rightIndex; j > start; j--)
                    {
                        if (arr[j - 1].CompareTo(tmp) > 0)
                        {
                            arr[j] = arr[j - 1];
                        }
                        else
                        {
                            insertIndex = j;
                            break;
                        }
                    }
                    arr[insertIndex] = tmp;

                    leftIndex++;
                    rightIndex++;
                }
            }
        }
        private static void BlockSwap<T>(T[] arr, int ABlock, int BBlock, int blockSize) where T : IComparable<T>
        {
            for (int i = 0; i < blockSize; i++)
            {
                Swap(ref arr[ABlock + i], ref arr[BBlock + i]);
            }
        }

        private static void RotateBlockMerge<T>(T[] arr, int start, int end, int blockSize) where T : IComparable<T>
        {
            while (blockSize > 0)
            {
                int insertIndex = BinarySearch(arr, start + blockSize, end, arr[start]);
                Rotate(arr, start, start + blockSize, insertIndex);
                blockSize--;
                start = insertIndex - blockSize;
            }
        }

        private static void OriginalBlockMerge<T>(T[] arr, int start, int middle, int end) where T : IComparable<T>
        {
            int blockSize = (int)Math.Sqrt(middle - start);
            int fullABlocks = (middle - start) / blockSize, fullBBlocks = (end - middle) / blockSize;
            int fullBlocks = fullABlocks + fullBBlocks;
            int s1 = start, s2 = start + fullBlocks;

            for (; s2 < middle - fullBlocks; s2++)
            {
                if (arr[s1 + fullBlocks - 1].CompareTo(arr[s2]) != 0) break;
            }

            int buffer = s2 + fullBlocks;

            {
                int bufferIndex = buffer;
                for (int i = buffer + 1; i < middle; i++)
                {
                    if (arr[i].CompareTo(arr[bufferIndex]) > 0)
                    {
                        Rotate(arr, ++bufferIndex, i, i + 1);
                        if (bufferIndex >= buffer + blockSize - 1) break;
                    }
                }
            }

            for (int i = 0; i < fullABlocks; i++)
            {
                Swap(ref arr[s1 + i], ref arr[s2 + i]);
            }

            int undersizedABlockLength = (middle - start) % fullABlocks;

            {
                int left = 0, right = fullABlocks;
                while (left < right)
                {
                    int leftBlock = undersizedABlockLength + left * blockSize + start, rightBlock = undersizedABlockLength + right * blockSize + start;

                    int leftBlockValue = leftBlock, rightBlockValue = rightBlock + blockSize - 1;
                    if (leftBlock < buffer + blockSize)
                    {
                        left++;
                        continue;
                    }

                    for (int i = left + 1; i < right; i++)
                    {
                        if (arr[leftBlockValue].CompareTo(arr[undersizedABlockLength + i * blockSize + start]) > 0)
                        {
                            leftBlockValue = undersizedABlockLength + i * blockSize + start;
                        }
                    }

                    if (rightBlockValue < end && arr[leftBlockValue].CompareTo(arr[rightBlockValue]) > 0)
                    {
                        BlockSwap(arr, leftBlock, rightBlock, blockSize);
                        Swap(ref arr[s1 + left], ref arr[s2 + left]);
                        Swap(ref arr[s1 + right], ref arr[s2 + right]);
                        right++;
                        left++;
                    }
                    else
                    {
                        BlockSwap(arr, leftBlockValue, leftBlock, blockSize);
                        left++;
                    }
                }
            }

            //if (arr is int[] arrInt)
            //{
            //    int maxValue = 0;
            //    for (int i = 0; i < fullBlocks; i++)
            //    {
            //        int block = undersizedABlockLength + i * blockSize + start;
            //        if (block < buffer + blockSize) continue;

            //        if (arr[s1 + i].CompareTo(arr[s2 + i]) < 0)
            //        {
            //            block += blockSize - 1;
            //        }

            //        if (arrInt[block] < maxValue)
            //        {
            //            Console.Write("Wrong order : ");
            //            for (int j = 0; j < fullBlocks; j++)
            //            {
            //                int jblock = undersizedABlockLength + j * blockSize + start;
            //                if (jblock < buffer + blockSize) continue;

            //                if (arr[s1 + j].CompareTo(arr[s2 + j]) < 0)
            //                {
            //                    Console.Write("[B:" + arr[jblock + blockSize - 1] + "]");
            //                }
            //                else
            //                {
            //                    Console.Write("[A:" + arr[jblock] + "]");
            //                }
            //            }
            //            break;
            //        }
            //        maxValue = arrInt[block];
            //    }
            //}

            int lastA = end;
            int undersizedFirstABlockLength = undersizedABlockLength;
            int inverseI;
            for (int i = 0; i < fullBlocks; i++)
            {
                inverseI = fullBlocks - i - 1;
                int block = undersizedABlockLength + inverseI * blockSize + start;
                if (block < buffer + blockSize)
                {
                    undersizedFirstABlockLength = blockSize - (buffer + blockSize - block);
                    break;
                }

                if (arr[s1 + inverseI].CompareTo(arr[s2 + inverseI]) > 0)
                {
                    BlockSwap(arr, block, buffer, blockSize);

                    int left = buffer, right = block + blockSize;
                    while (left < buffer + blockSize)
                    {
                        if (right >= end || arr[left].CompareTo(arr[right]) <= 0)
                        {
                            if (left == buffer) lastA = right - (buffer + blockSize - left);
                            Swap(ref arr[right - (buffer + blockSize - left)], ref arr[left]);
                            left++;
                        }
                        else
                        {
                            Swap(ref arr[right - (buffer + blockSize - left)], ref arr[right]);
                            right++;
                        }
                    }
                }
            }

            for (int i = 0; i < fullBlocks; i++)
            {
                if (arr[s1 + i].CompareTo(arr[s2 + i]) > 0)
                {
                    Swap(ref arr[s1 + i], ref arr[s2 + i]);
                }
            }

            InsertionSort<T>(arr, buffer, buffer + blockSize);

            Span<int> v = stackalloc int[3];

            v[0] = buffer + blockSize + undersizedFirstABlockLength;
            v[1] = lastA;
            for (int i = v[0]; i < lastA; i++)
            {
                if (arr[s1 + fullBlocks - 1].CompareTo(arr[i]) <= 0)
                {
                    v[1] = i;
                    break;
                }
            }
            v[2] = lastA;
            for (int i = v[1]; i < lastA; i++)
            {
                if (arr[s2 + fullBlocks - 1].CompareTo(arr[i]) <= 0)
                {
                    v[2] = i;
                    break;
                }
            }

            Rotate(arr, buffer, v[0], v[2]);
            Rotate(arr, s1 + fullBlocks, s2 + fullBlocks, v[1] - v[0] + s2 + fullBlocks);

            int newBuffer = v[2] - undersizedFirstABlockLength - blockSize;

            RotateBlockMerge(arr, newBuffer + blockSize, lastA, undersizedFirstABlockLength);

            RotateBlockMerge(arr, newBuffer, end, blockSize);
            RotateBlockMerge(arr, start, v[1] - v[0] + start + fullBlocks, fullBlocks);
            RotateBlockMerge(arr, s2 + v[1] - v[0], newBuffer, fullBlocks);

            //if (!CheckSorted(arr, start, end)) 
            //    Console.WriteLine("Unsorted");
        }

        public static int BinarySearch<T>(T[] arr, int start, int end, in T elem) where T : IComparable<T>
        {
            int middle;
            while (end > start)
            {
                middle = (start + end) >> 1;
                if (arr[middle].CompareTo(elem) < 0) start = middle + 1;
                else end = middle;
            } 

            return start;
        }

        public static int GetPurged<T>(T[] arr) where T : IComparable<T>
        {
            for (int i = 0; i < arr.Length - 1; i++)
            {
                if (arr[i].CompareTo(arr[i + 1]) > 0)
                {
                    return arr.Length - i - 1;
                }
            }

            return 0;
        }

        private static void Swap<T>(ref T left, ref T right)
        {
            T tmp = left;
            left = right;
            right = tmp;
        }

        public static bool CheckSorted<T>(in T[] arr) where T : IComparable<T>
        {
            for (int i = 0; i < arr.Length - 1; i++)
            {
                if (arr[i].CompareTo(arr[i + 1]) > 0)
                {
                    return false;
                }
            }
            return true;
        }

        public static bool CheckSorted<T>(in T[] arr, int start, int end) where T : IComparable<T>
        {
            for (int i = start; i < end - 1; i++)
            {
                if (arr[i].CompareTo(arr[i + 1]) > 0)
                {
                    return false;
                }
            }
            return true;
        }

        public static bool SameArray<T>(in T[] a, in T[] b) where T : IComparable<T>
        {
            if (a.Length != b.Length) return false;
            for (int i = 0; i < a.Length; i++)
            {
                if (a[i].CompareTo(b[i]) != 0)
                {
                    return false;
                }
            }
            return true;
        }

        public static void BubbleSort<T>(T[] arr) where T : IComparable<T>
        {
            for (int i = arr.Length - 1; i > 0; i--)
            {
                for (int j = 0; j < i; j++)
                {
                    if (arr[j].CompareTo(arr[j + 1]) > 0)
                    {
                        Swap(ref arr[j], ref arr[j + 1]);
                    }
                }
            }
        }

        public static void OptimizedBubbleSort<T>(T[] arr) where T : IComparable<T>
        {
            int sorted = 0;
            int newSorted;
            while (sorted < arr.Length)
            {
                newSorted = 1;
                for (int i = 0; i < arr.Length - sorted - 1; i++)
                {
                    if (arr[i].CompareTo(arr[i + 1]) > 0)
                    {
                        Swap(ref arr[i], ref arr[i + 1]);
                        newSorted = 1;
                    }
                    else
                    {
                        newSorted++;
                    }
                }
                sorted += newSorted;
            }
        }

        public static void CocktailShakerSort<T>(T[] arr) where T : IComparable<T>
        {
            for (int i = 0; i < arr.Length / 2; i++)
            {
                for (int j = i; j < arr.Length - i - 1; j++)
                {
                    if (arr[j].CompareTo(arr[j + 1]) > 0)
                    {
                        Swap(ref arr[j], ref arr[j + 1]);
                    }
                }

                for (int j = arr.Length - i - 1; j > i; j--)
                {
                    if (arr[j].CompareTo(arr[j - 1]) < 0)
                    {
                        Swap(ref arr[j], ref arr[j - 1]);
                    }
                }
            }
        }

        public static void OptimizedCocktailShakerSort<T>(T[] arr) where T : IComparable<T>
        {
            int leftSorted = 0, rightSorted = 0;
            int sorted;
            while (leftSorted + rightSorted < arr.Length)
            {
                sorted = 1; 
                for (int j = rightSorted; j < arr.Length - leftSorted - 1; j++)
                {
                    if (arr[j].CompareTo(arr[j + 1]) > 0)
                    {
                        Swap(ref arr[j], ref arr[j + 1]);
                        sorted = 1;
                    }
                    else
                    {
                        sorted++;
                    }
                }
                leftSorted += sorted;

                sorted = 1;
                for (int j = arr.Length - leftSorted - 1; j > rightSorted; j--)
                {
                    if (arr[j].CompareTo(arr[j - 1]) < 0)
                    {
                        Swap(ref arr[j], ref arr[j - 1]);
                        sorted = 1;
                    }
                    else
                    {
                        sorted++;
                    }
                }
                rightSorted += sorted;
            }
        }

        public static void OddEvenSort<T>(T[] arr) where T : IComparable<T>
        {
            bool unsorted = true;
            while (unsorted)
            {
                unsorted = false;
                for (int k = 0; k < 2; k++)
                {
                    for (int i = k; i < arr.Length - 1; i += 2)
                    {
                        if (arr[i].CompareTo(arr[i + 1]) > 0)
                        {
                            Swap(ref arr[i], ref arr[i + 1]);
                            unsorted = true;
                        }
                    }
                }
            }
        }

        public static void CombSort<T>(T[] arr) where T : IComparable<T>
        {
            const double reductionFactor = 1.3;
            int gap = arr.Length;
            bool unsorted = true;

            while (gap > 1 || unsorted)
            {
                unsorted = false;
                double newGap = (double)gap / reductionFactor;
                gap = (newGap <= 1.0) ? 1 : (int)newGap;

                for (int j = 0; j < arr.Length - gap; j++)
                {
                    if (arr[j].CompareTo(arr[j + gap]) > 0)
                    {
                        Swap(ref arr[j], ref arr[j + gap]);
                        unsorted = true;
                    }
                }
            }
        }

        public static void SelectionSort<T>(T[] arr) where T : IComparable<T>
        {
            int minIndex;
            for (int i = 0; i < arr.Length - 1; i++)
            {
                minIndex = i;
                for (int j = i; j < arr.Length; j++)
                {
                    if (arr[j].CompareTo(arr[minIndex]) < 0)
                    {
                        minIndex = j;
                    }
                }
                Swap(ref arr[i], ref arr[minIndex]);
            }
        }

        public static void DoubleSelectionSort<T>(T[] arr) where T : IComparable<T>
        {
            int minIndex, maxIndex;
            for (int i = 0; i < arr.Length >> 1; i++)
            {
                minIndex = i;
                maxIndex = arr.Length - i - 1;
                for (int j = i; j < arr.Length - i; j++)
                {
                    if (arr[j].CompareTo(arr[minIndex]) < 0)
                    {
                        minIndex = j;
                    }
                    else if (arr[j].CompareTo(arr[maxIndex]) >= 0)
                    {
                        maxIndex = j;
                    }
                }
                Swap(ref arr[i], ref arr[minIndex]);
                if (maxIndex == i) maxIndex = minIndex;
                Swap(ref arr[arr.Length - i - 1], ref arr[maxIndex]);
            }
        }

        public static void ExchangeSort<T>(T[] arr) where T : IComparable<T>
        {
            for (int i = arr.Length - 1; i > 0; i--)
            {
                for (int j = 0; j < i; j++)
                {
                    if (arr[j].CompareTo(arr[i]) >= 0)
                    {
                        Swap(ref arr[i], ref arr[j]);
                    }
                }
            }
        }

        public static void CycleSort<T>(T[] arr) where T : IComparable<T>
        {
            int lessCounter;
            for (int i = 0; i < arr.Length; i++)
            {
                ref T current = ref arr[i];
                lessCounter = 0;
                for (int j = 0; j < i; j++)
                {
                    if (arr[j].CompareTo(current) < 0)
                    {
                        lessCounter++;
                    }
                }

                for (int j = i + 1; j < arr.Length; j++)
                {
                    if (arr[j].CompareTo(current) < 0)
                    {
                        lessCounter++;
                    }
                }

                if (i == lessCounter) continue;

                T tmp = arr[lessCounter];
                arr[lessCounter] = current;
                while (true)
                {
                    lessCounter = 0;
                    for (int j = 0; j < i; j++)
                    {
                        if (arr[j].CompareTo(tmp) < 0)
                        {
                            lessCounter++;
                        }
                    }

                    for (int j = i + 1; j < arr.Length; j++)
                    {
                        if (arr[j].CompareTo(tmp) < 0)
                        {
                            lessCounter++;
                        }
                    }

                    if (i == lessCounter) break;
                    Swap(ref tmp, ref arr[lessCounter]);
                } 

                arr[i] = tmp;
            }
        }

        public static void HeapSort<T>(T[] arr) where T : IComparable<T>
        {
            MaxHeapify(arr);

            for (int i = 1; i < arr.Length; i++)
            {
                Swap(ref arr[0], ref arr[^i]);
                ShiftDown(arr, 0, arr.Length - i);
            }
        }

        public static void CircleSort<T>(T[] arr) where T : IComparable<T>
        {
            bool unsorted = true;
            int firstCircleSize = RoundToNextPowerOf2(arr.Length);
            while (unsorted)
            {
                unsorted = false;

                for (int circleSize = firstCircleSize; circleSize > 1; circleSize >>= 1)
                {
                    for (int k = 0; k < arr.Length; k += circleSize)
                    {
                        for (int i = 0, oppositeIndex = circleSize - 1 + k; i < circleSize >> 1; i++, oppositeIndex--)
                        {
                            if (oppositeIndex < arr.Length && arr[i + k].CompareTo(arr[oppositeIndex]) > 0)
                            {
                                Swap(ref arr[i + k], ref arr[oppositeIndex]);
                                unsorted = true;
                            }
                        }
                    }
                }
            }
        }

        public static void InsertionSort<T>(T[] arr) where T : IComparable<T>
        {
            for (int i = 0; i < arr.Length - 1; i++)
            {
                if (arr[i].CompareTo(arr[i + 1]) > 0)
                {
                    T tmp = arr[i + 1];
                    int insertIndex = 0;
                    for (int j = i + 1; j > 0; j--)
                    {
                        if (arr[j - 1].CompareTo(tmp) > 0)
                        {
                            arr[j] = arr[j - 1];
                        }
                        else
                        {
                            insertIndex = j;
                            break;
                        }
                    }
                    arr[insertIndex] = tmp;
                }
            }
        }

        public static void BinaryInsertionSort<T>(T[] arr) where T : IComparable<T>
        {
            for (int i = 0; i < arr.Length - 1; i++)
            {
                if (arr[i].CompareTo(arr[i + 1]) > 0)
                {
                    T tmp = arr[i + 1];
                    int insertIndex = BinarySearch(arr, 0, i + 1, tmp);
                    for (int j = i + 1; j > insertIndex; j--)
                    {
                        arr[j] = arr[j - 1];
                    }

                    arr[insertIndex] = tmp;
                }
            }
        }

        public static void ShellSort<T>(T[] arr) where T : IComparable<T>
        {
            const double reductionFactor = 2.3;
            int gap = arr.Length;
            while (gap > 1)
            {
                double newGap = (double)gap / reductionFactor;
                gap = (newGap <= 1.0) ? 1 : (int)newGap;

                for (int k = 0; k < gap; k++)
                {
                    for (int i = k; i < arr.Length - gap; i += gap)
                    {
                        if (arr[i].CompareTo(arr[i + gap]) > 0)
                        {
                            T tmp = arr[i + gap];
                            for (int j = i + gap; j > k; j -= gap)
                            {
                                if (arr[j - gap].CompareTo(tmp) > 0)
                                {
                                    arr[j] = arr[j - gap];
                                    if (j == k + gap) arr[j - gap] = tmp;
                                }
                                else
                                {
                                    arr[j] = tmp;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        private static void InsertionSort<T>(T[] arr, int start, int end) where T : IComparable<T>
        {
            for (int i = start; i < end - 1; i++)
            {
                if (arr[i].CompareTo(arr[i + 1]) > 0)
                {
                    T tmp = arr[i + 1];
                    for (int j = i + 1; j > start; j--)
                    {
                        if (arr[j - 1].CompareTo(tmp) > 0)
                        {
                            arr[j] = arr[j - 1];
                            if (j == start + 1) arr[start] = tmp;
                        }
                        else
                        {
                            arr[j] = tmp;
                            break;
                        }
                    }
                }
            }
        }

        public static void QuickSort<T>(T[] arr) where T : IComparable<T>
        {
            QuickSort(arr, 0, arr.Length);
        }

        private static void QuickSort<T>(T[] arr, int start, int end) where T : IComparable<T>
        {
            //IncrementRecursiveCall();
            if (end - start <= 15) InsertionSort(arr, start, end);
            else
            {
                int leftCursor = start + 1, rightCursor = end - 1;
                ref T pivot = ref arr[start];

                while (leftCursor <= rightCursor)
                {
                    if (arr[rightCursor].CompareTo(pivot) > 0) rightCursor--;
                    else Swap(ref arr[leftCursor++], ref arr[rightCursor]);
                }
                
                Swap(ref pivot, ref arr[leftCursor - 1]);

                QuickSort<T>(arr, start, leftCursor);
                QuickSort<T>(arr, leftCursor, end);
            }
            //recursiveCall--;
        }

        public static void QuickSortLL<T>(T[] arr) where T : IComparable<T>
        {
            QuickSortLL(arr, 0, arr.Length);
        }

        private static void QuickSortLL<T>(T[] arr, int start, int end) where T : IComparable<T>
        {
            //IncrementRecursiveCall();
            if (end - start <= 15) InsertionSort(arr, start, end);
            else
            {
                int cursor = start + 1;
                ref T pivot = ref arr[start];

                for (int i = start + 1; i < end; i++)
                {
                    if (arr[i].CompareTo(pivot) <= 0) Swap(ref arr[cursor++], ref arr[i]);
                }

                Swap(ref pivot, ref arr[cursor - 1]);

                QuickSortLL<T>(arr, start, cursor - 1);
                QuickSortLL<T>(arr, cursor, end);
            }
            //recursiveCall--;
        }

        public static void MergeSort<T>(T[] arr) where T : IComparable<T>
        {
            T[] auxArr = (T[])arr.Clone();
            MergeSort(auxArr, 0, arr.Length, arr);
        }

        private static void MergeSort<T>(T[] arr, int start, int end, T[] auxArr) where T : IComparable<T>
        {
            //IncrementRecursiveCall();
            if (end - start <= 15) InsertionSort(auxArr, start, end);
            else
            {
                int middle = (end + start) >>> 1;

                MergeSort(auxArr, start, middle, arr);
                MergeSort(auxArr, middle, end, arr);

                MergeArray(arr, start, middle, end, auxArr);
            }
            //recursiveCall--;
        }

        public static void IterativeMergeSort<T>(T[] arr) where T : IComparable<T>
        {
            T[] auxArr = new T[arr.Length];
            double scaleFactor = (double)arr.Length / (double)RoundToNextPowerOf2(arr.Length >> 1);

            double partitionSize = scaleFactor * 0.5;
            int round = 0;
            while (partitionSize < (double)arr.Length)
            {
                for (int j = 0; j < (int)((double)arr.Length / partitionSize); j += 2)
                {
                    double jIndex = (double)j * partitionSize;
                    int start = (int)(jIndex), middle = (int)(jIndex + partitionSize), end = (int)(jIndex + partitionSize * 2.0);

                    MergeArray(arr, start, middle, end, auxArr);
                }

                Swap(ref arr, ref auxArr);
                partitionSize = scaleFactor * (double)(1 << round++);
            }

            if ((round & 1) == 1)
            {
                arr.CopyTo(auxArr, 0);
            }
        }

        public static void RotateMergeSort<T>(T[] arr) where T : IComparable<T>
        {
            double scaleFactor = (double)arr.Length / (double)RoundToNextPowerOf2(arr.Length >> 1);

            double partitionSize = scaleFactor * 0.5;
            int round = 0;
            while (partitionSize < (double)arr.Length)
            {
                for (int j = 0; j < (int)((double)arr.Length / partitionSize); j += 2)
                {
                    double jIndex = (double)j * partitionSize;
                    int start = (int)(jIndex), middle = (int)(jIndex + partitionSize), end = (int)(jIndex + partitionSize * 2.0);

                    RotateMergeArray(arr, start, middle, end);
                }

                partitionSize = scaleFactor * (double)(1 << round++);
            }
        }

        public static void InPlaceMergeSort<T>(T[] arr) where T : IComparable<T>
        {
            double scaleFactor = (double)arr.Length / (double)RoundToNextPowerOf2(arr.Length >> 1);

            double partitionSize = scaleFactor * 0.5;
            int round = 0;
            while (partitionSize < (double)arr.Length)
            {
                for (int j = 0; j < (int)((double)arr.Length / partitionSize); j += 2)
                {
                    double jIndex = (double)j * partitionSize;
                    int start = (int)(jIndex), middle = (int)(jIndex + partitionSize), end = (int)(jIndex + partitionSize * 2.0);

                    InPlaceMergeArray(arr, start, middle, end);
                }
                partitionSize = scaleFactor * (double)(1 << round++);
            }
        }

        public static void WeaveSort<T>(T[] arr) where T : IComparable<T>
        {
            double scaleFactor = (double)arr.Length / (double)RoundToNextPowerOf2(arr.Length >> 1);

            double partitionSize = scaleFactor * 0.5;
            int round = 0;
            while (partitionSize < (double)arr.Length)
            {
                for (int j = 0; j < (int)((double)arr.Length / partitionSize); j += 2)
                {
                    double jIndex = (double)j * partitionSize;
                    int start = (int)(jIndex), middle = (int)(jIndex + partitionSize), end = (int)(jIndex + partitionSize * 2.0);

                    int leftIndex = start, rightIndex = middle;
                    while (leftIndex < rightIndex)
                    {
                        T tmp = arr[rightIndex];
                        int insertIndex = ++leftIndex;
                        for (int i = rightIndex; i > insertIndex; i--)
                        {
                            arr[i] = arr[i - 1];
                        }
                        arr[insertIndex] = tmp;

                        leftIndex++;
                        rightIndex++;
                    }
                    InsertionSort(arr, start, end);
                }

                partitionSize = scaleFactor * (double)(1 << round++);
            }
        }

        public static void StrandSort<T>(T[] arr) where T : IComparable<T>
        {
            T[] solutionList = new T[arr.Length];
            T[] auxArr = new T[arr.Length];

            int solutionLength = 0;
            int solutionSorted = 0;
            while (solutionLength < arr.Length)
            {
                solutionList[solutionLength++] = arr[0];
                Swap(ref arr[0], ref arr[arr.Length - solutionLength]);
                
                for (int i = 0; i < arr.Length - solutionLength; i++)
                {
                    if (solutionList[solutionLength - 1].CompareTo(arr[i]) <= 0)
                    {
                        solutionList[solutionLength++] = arr[i];
                        Swap(ref arr[i], ref arr[arr.Length - solutionLength]);
                        i--;
                    }
                }

                MergeArray(solutionList, 0, solutionSorted, solutionLength, auxArr);

                Swap(ref solutionList, ref auxArr);
                solutionSorted = solutionLength;
            }

            solutionList.CopyTo(arr, 0);
        }

        public static void BitonicSort<T>(T[] arr) where T : IComparable<T>
        {
            int realSize = RoundToNextPowerOf2(arr.Length);

            for (int mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
            {
                for (int k = 0; k < realSize; k += mergerSize)
                {
                    for (int i = 0; i < (mergerSize >> 1); i++)
                    {
                        int left = k + i, right = k + mergerSize - i - 1;
                        if (right < arr.Length && arr[left].CompareTo(arr[right]) > 0) Swap(ref arr[left], ref arr[right]);
                    }
                }

                for (int j = (mergerSize >> 1); j > 1; j >>= 1)
                {
                    for (int k = 0; k < realSize; k += j)
                    {
                        for (int i = 0; i < (j >> 1); i++)
                        {
                            int left = k + i, right = k + i + (j >> 1);
                            if (right < arr.Length && arr[left].CompareTo(arr[right]) > 0) Swap(ref arr[left], ref arr[right]);
                        }
                    }
                }
            }
        }

        public static void OddEvenMergeSort<T>(T[] arr) where T : IComparable<T>
        {
            int realSize = RoundToNextPowerOf2(arr.Length);

            for (int mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
            {
                for (int k = 0; k < realSize / mergerSize; k++)
                {
                    int a = k * mergerSize;
                    for (int i = 0; i < (mergerSize >> 1); i++)
                    {
                        int left = i + a, right = i + a + (mergerSize >> 1);
                        if (right < arr.Length && arr[left].CompareTo(arr[right]) > 0) Swap(ref arr[left], ref arr[right]);
                    }
                }

                for (int j = (mergerSize >> 1); j > 1; j >>= 1)
                {
                    int diff = (j >> 1);
                    for (int r = 0; r < realSize / mergerSize; r++)
                    {
                        for (int k = 0; k < mergerSize / j - 1; k++)
                        {
                            int a = k * j + r * mergerSize + diff;
                            for (int i = 0; i < (j >> 1); i++)
                            {
                                int left = i + a, right = i + a + (j >> 1);
                                if (right < arr.Length && arr[left].CompareTo(arr[right]) > 0) Swap(ref arr[left], ref arr[right]);
                            }
                        }
                    }
                }
            }
        }

        public static void RadixLSDSort(int[] arr)
        {
            int bits = (radixBitShift > 16) ? 16 : radixBitShift;
            int[] bucketArr = new int[arr.Length];
            Span<int> cursors = stackalloc int[1 << bits];
            Span<int> counter = stackalloc int[1 << bits];

            for (int k = 0; k < 32; k += bits)
            {
                int mask = ((1 << bits) - 1) << k;

                cursors.Fill(0);
                counter.Fill(0);
                for (int i = 0; i < arr.Length; i++)
                {
                    counter[(arr[i] & mask) >> k]++;
                }

                Swap(ref arr, ref bucketArr);

                for (int i = 0; i < cursors.Length - 1; i++)
                {
                    cursors[i + 1] = counter[i] + cursors[i];
                }

                for (int i = 0; i < bucketArr.Length; i++)
                {
                    int cursorIndex = (bucketArr[i] & mask) >> k;
                    arr[cursors[cursorIndex]++] = bucketArr[i];
                }
            }
        }

        public static void RadixLSDSort(long[] arr)
        {
            int bits = (radixBitShift > 32) ? 32 : radixBitShift;
            long[] bucketArr = new long[arr.Length];
            Span<int> cursors = stackalloc int[1 << bits];
            Span<int> counter = stackalloc int[1 << bits];

            for (int k = 0; k < 64; k += bits)
            {
                long mask = ((1 << bits) - 1) << k;

                cursors.Fill(0);
                counter.Fill(0);
                for (int i = 0; i < arr.Length; i++)
                {
                    counter[(int)((arr[i] & mask) >> k)]++;
                }

                Swap(ref arr, ref bucketArr);

                for (int i = 0; i < cursors.Length - 1; i++)
                {
                    cursors[i + 1] += cursors[i];
                }

                for (int i = 0; i < bucketArr.Length; i++)
                {
                    int cursorIndex = (int)((bucketArr[i] & mask) >> k);
                    arr[cursors[cursorIndex]++] = bucketArr[i];
                }
            }
        }
        public static void RadixMSDSort(int[] arr)
        {
            int bits = (radixBitShift > 16) ? 16 : radixBitShift;
            int[] bucketArr = new int[arr.Length];
            int[] counter = new int[1 << bits];

            RadixMSDSort(arr, bucketArr, counter, bits, 0, arr.Length, 0);
        }

        public static void RadixMSDSort(int[] arr, int[] bucketArr, int[] counter, int bits, int start, int end, int round)
        {
            if (end - start < 2 || round >= 31) return;

            Span<int> cursors = stackalloc int[1 << bits];

            int mask = (((1 << bits) - 1) << (32 - bits - round));

            Array.Fill(counter, 0);
            for (int i = start; i < end; i++)
            {
                counter[(arr[i] & mask) >> (32 - bits - round)]++;
            }

            for (int i = start; i < end; i++)
            {
                bucketArr[i] = arr[i];
            }

            for (int i = 0; i < cursors.Length - 1; i++)
            {
                cursors[i + 1] = counter[i] + cursors[i];
            }

            for (int i = start; i < end; i++)
            {
                int cursorIndex = (bucketArr[i] & mask) >> (32 - bits - round);
                arr[start + cursors[cursorIndex]++] = bucketArr[i];
            }

            int nextStart = start;
            for (int i = 0; i < cursors.Length; i++)
            {
                RadixMSDSort(arr, bucketArr, counter, bits, nextStart, start + cursors[i], round + bits);
                nextStart = start + cursors[i];
            }
        }

        public static void BucketSort(int[] arr)
        {
            if (arr.Length < 2) return;

            int[] bucketArr = new int[arr.Length];
            int[] cursors = new int[bucketArr.Length / bucketRatio + 1];
            int[] counter = new int[bucketArr.Length / bucketRatio + 1];
            int min = arr.Min();

            for (int i = 0; i < arr.Length; i++)
            {
                counter[(arr[i] - min) / bucketRatio]++;
            }

            for (int i = 0; i < cursors.Length - 1; i++)
            {
                cursors[i + 1] = counter[i] + cursors[i];
            }

            for (int i = 0; i < bucketArr.Length; i++)
            {
                bucketArr[cursors[(arr[i] - min) / bucketRatio]++] = arr[i];
            }

            InsertionSort(bucketArr, 0, cursors[0]);
            for (int i = 0; i < cursors.Length - 1; i++)
            {
                InsertionSort(bucketArr, cursors[i], cursors[i + 1]);
            }

            bucketArr.CopyTo(arr, 0);
        }

        public static void BinaryQuickSort(int[] arr)
        {
            BinaryQuickSort(arr, 0, arr.Length, 0);
        }

        private static void BinaryQuickSort(int[] arr, int start, int end, int pivot)
        {
            //IncrementRecursiveCall();
            if (pivot >= 32 || end <= start) return;

            int leftCursor = start, rightCursor = end - 1;
            int mask = (1 << 31 - pivot);

            while (leftCursor <= rightCursor)
            {
                if ((arr[leftCursor] & mask) == 0) leftCursor++;
                else Swap(ref arr[leftCursor], ref arr[rightCursor--]);
            }

            BinaryQuickSort(arr, start, leftCursor, pivot + 1);
            BinaryQuickSort(arr, leftCursor, end, pivot + 1);
            //recursiveCall--;
        }
        public static void BlockSort<T>(T[] arr) where T : IComparable<T>
        {
            double scaleFactor = (double)(arr.Length) / (double)RoundToNextPowerOf2(arr.Length >> 1);

            double partitionSize = scaleFactor * 0.5;
            int round = 0;
            while (partitionSize < (double)arr.Length)
            {
                for (int j = 0; j < (int)((double)arr.Length / partitionSize); j += 2)
                {
                    double jIndex = (double)j * partitionSize;
                    int start = (int)(jIndex), middle = (int)(jIndex + partitionSize), end = (int)(jIndex + partitionSize * 2.0);

                    if (end - start < 100) RotateBlockMerge(arr, start, end, middle - start);
                    else
                    {
                        OriginalBlockMerge(arr, start, middle, end);
                    }
                }

                partitionSize = scaleFactor * (double)(1 << round++);
            }
        }

        public static void StalinSort<T>(T[] arr) where T : IComparable<T>
        {
            int safetyIndex = 0;
            for (int i = 1; i < arr.Length; i++)
            {
                if (arr[safetyIndex].CompareTo(arr[i]) <= 0)
                {
                    Swap(ref arr[++safetyIndex], ref arr[i]);
                }
                else
                {
                    arr[i] = arr[0];
                }
            }
        }

        public static void PancakeSort<T>(T[] arr) where T : IComparable<T>
        {
            int minIndex;
            for (int i = 0; i < arr.Length - 1; i++)
            {
                minIndex = i;
                for (int j = i; j < arr.Length; j++)
                {
                    if (arr[j].CompareTo(arr[minIndex]) < 0)
                    {
                        minIndex = j;
                    }
                }
                Reverse(arr, minIndex, arr.Length);
                Reverse(arr, i, arr.Length);
            }
        }

        public static void StoogeSort<T>(T[] arr) where T : IComparable<T>
        {
            StoogeSort(arr, 0, arr.Length);
        }

        private static void StoogeSort<T>(T[] arr, int start, int end) where T : IComparable<T>
        {
            if (end - start > 2)
            {
                int splitSize = (end - start) / 3;

                StoogeSort(arr, start, end - splitSize);
                StoogeSort(arr, start + splitSize, end);
                StoogeSort(arr, start, end - splitSize);
            }
            else
            {
                if (arr[start].CompareTo(arr[start + 1]) > 0) Swap(ref arr[start], ref arr[start + 1]);
            }
        }

        public static void SlowSort<T>(T[] arr) where T : IComparable<T>
        {
            SlowSort(arr, 0, arr.Length);
        }

        private static void SlowSort<T>(T[] arr, int start, int end) where T : IComparable<T>
        {
            int middle = (start + end) >> 1;

            if (end - start > 2)
            {
                SlowSort(arr, start, middle);
                SlowSort(arr, middle, end);
            }

            if (end - start >= 2)
            {
                if (arr[middle - 1].CompareTo(arr[end - 1]) > 0) Swap(ref arr[middle - 1], ref arr[end - 1]);
                SlowSort(arr, start, end - 1);
            }
        }

        public class BubbleSortNetwork : SortingNetwork
        {
            public override void Create(int size)
            {
                swapList = new (int, int)[((size - 1) * size) / 2];
                int index = 0;

                for (int i = 1; i < size; i++)
                    for (int j = 0; j < size - i; j++)
                        swapList[index++] = (j, j + 1);
            }
        }

        public class BitonicNetwork : SortingNetwork
        {
            public override void Create(int size)
            {
                List<(int, int)> swapList = new List<(int, int)>();
                swapList.Capacity = size;
                int realSize = RoundToNextPowerOf2(size);

                int mergerSize = 2;

                while (mergerSize <= realSize)
                {
                    for (int k = 0; k < realSize; k += mergerSize)
                    {
                        for (int i = 0; i < mergerSize / 2; i++)
                        {
                            int left = k + i, right = k + mergerSize - i - 1;
                            if (left < size && right < size) swapList.Add((left, right));
                        }
                    }

                    for (int j = mergerSize / 2; j > 1; j /= 2)
                    {
                        for (int k = 0; k < realSize; k += j)
                        {
                            for (int i = 0; i < j / 2; i++)
                            {
                                int left = k + i, right = k + i + j / 2;
                                if (left < size && right < size) swapList.Add((left, right));
                            }
                        }
                    }

                    mergerSize *= 2;
                }

                this.swapList = swapList.ToArray();
            }
        }

        public class OddEvenMergeNetwork : SortingNetwork
        {
            public override void Create(int size)
            {
                List<(int, int)> swapList = new List<(int, int)>();
                swapList.Capacity = size;
                int realSize = RoundToNextPowerOf2(size);

                int mergerSize = 2;

                while (mergerSize <= realSize)
                {
                    for (int k = 0; k < realSize / mergerSize; k++)
                    {
                        for (int i = 0; i < mergerSize / 2; i++)
                        {
                            int left = i + k * mergerSize, right = i + k * mergerSize + mergerSize / 2;
                            if (left < size && right < size) swapList.Add((left, right));
                        }
                    }

                    for (int j = mergerSize / 2; j > 1; j /= 2)
                    {
                        for (int r = 0; r < realSize / mergerSize; r++)
                        {
                            for (int k = 0; k < mergerSize / j - 1; k++)
                            {
                                for (int i = 0; i < j / 2; i++)
                                {
                                    int diff = j / 2;
                                    int left = i + k * j + (r * mergerSize) + diff, right = i + k * j + (r * mergerSize) + diff + j / 2;
                                    if (left < size && right < size) swapList.Add((left, right));
                                }
                            }
                        }
                    }

                    mergerSize *= 2;
                }

                this.swapList = swapList.ToArray();
            }
        }
    }
}
