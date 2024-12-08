using Microsoft.VisualBasic.FileIO;
using System.Diagnostics;

namespace VariousThings
{
    internal class Program
    {
        delegate void SortingAlgorithmDelegate<T>(T[] arr);

        readonly struct SortAlgorithm
        {
            public SortAlgorithm(string name, SortingAlgorithmDelegate<int> sortingAlgorithm, Complexity meanComplexity)
            {
                this.name = name;
                this.sortingAlgorithm = sortingAlgorithm;
                this.sortingNetwork = null;
                this.meanComplexity = meanComplexity;
            }

            public SortAlgorithm(string name, SortingNetwork sortingNetwork, Complexity meanComplexity)
            {
                this.name = name;
                this.sortingAlgorithm = sortingNetwork.Sort;
                this.sortingNetwork = sortingNetwork;
                this.meanComplexity = meanComplexity;
            }

            public enum Complexity
            {
                NSquared,
                LessThanNSquared,
                NLogN,
                NLogNSquared,
                Linear,
                TooHigh
            }

            public readonly string name;
            public readonly SortingAlgorithmDelegate<int> sortingAlgorithm;
            public readonly SortingNetwork? sortingNetwork;
            public readonly Complexity meanComplexity;
        }
        static void Main(string[] args)
        {
            Random random = new Random((int)(DateTime.Now.Ticks & 0xffffffffL));

            int[] list = SampleArray.CreateShuffledArray(1 << 14);
            int[] unsortedList = new int[list.Length];
            int[] checkList = SampleArray.CreateIncrementalArray(list.Length);

            SortingAlgorithms.RadixBitShift = 3;

            Console.WriteLine("Array size : " + list.Length + "\n");
            SortAlgorithm[] sortAlgorithms =
            [
                //Exchange sort
                new SortAlgorithm("Bubble sort", SortingAlgorithms.BubbleSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Bubble sort (optimized)", SortingAlgorithms.OptimizedBubbleSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Cocktail shaker sort", SortingAlgorithms.CocktailShakerSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Cocktail shaker sort (optimized)", SortingAlgorithms.OptimizedCocktailShakerSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Odd even sort", SortingAlgorithms.OddEvenSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Comb sort", SortingAlgorithms.CombSort, SortAlgorithm.Complexity.LessThanNSquared),
                new SortAlgorithm("Circle sort", SortingAlgorithms.CircleSort, SortAlgorithm.Complexity.NLogNSquared),

                //Selection sort
                new SortAlgorithm("Selection sort", SortingAlgorithms.SelectionSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Double selection sort", SortingAlgorithms.DoubleSelectionSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Exchange sort", SortingAlgorithms.ExchangeSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Cycle sort", SortingAlgorithms.CycleSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Heap sort", SortingAlgorithms.HeapSort, SortAlgorithm.Complexity.NLogN),

                //Insertion sort
                new SortAlgorithm("Insertion sort", SortingAlgorithms.InsertionSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Binary insertion sort", SortingAlgorithms.BinaryInsertionSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Shell sort", SortingAlgorithms.ShellSort, SortAlgorithm.Complexity.LessThanNSquared),

                //Quick sort
                new SortAlgorithm("Quick sort (left/right pointers)", SortingAlgorithms.QuickSort, SortAlgorithm.Complexity.NLogN),
                new SortAlgorithm("Quick sort (left/left pointers)", SortingAlgorithms.QuickSortLL, SortAlgorithm.Complexity.NLogN),

                //Merge sort
                new SortAlgorithm("Merge sort", SortingAlgorithms.MergeSort, SortAlgorithm.Complexity.NLogN),
                new SortAlgorithm("Iterative merge sort", SortingAlgorithms.IterativeMergeSort, SortAlgorithm.Complexity.NLogN),
                new SortAlgorithm("Rotate merge sort", SortingAlgorithms.RotateMergeSort, SortAlgorithm.Complexity.NLogNSquared),
                new SortAlgorithm("In place merge sort", SortingAlgorithms.InPlaceMergeSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Weave sort", SortingAlgorithms.WeaveSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Strand sort", SortingAlgorithms.StrandSort, SortAlgorithm.Complexity.LessThanNSquared),

                //Sorting network
                //new SortAlgorithm("Bubble sort (prebuilt network)", new SortingAlgorithms.BubbleSortNetwork(), SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Bitonic sort (prebuilt network)", new SortingAlgorithms.BitonicNetwork(), SortAlgorithm.Complexity.NLogNSquared),
                new SortAlgorithm("Odd even network (prebuilt network)", new SortingAlgorithms.OddEvenMergeNetwork(), SortAlgorithm.Complexity.NLogNSquared),
                new SortAlgorithm("Bitonic sort", SortingAlgorithms.BitonicSort, SortAlgorithm.Complexity.NLogNSquared),
                new SortAlgorithm("Odd even network", SortingAlgorithms.OddEvenMergeSort, SortAlgorithm.Complexity.NLogNSquared),

                //Bucket sort
                new SortAlgorithm("Radix LSD sort", SortingAlgorithms.RadixLSDSort, SortAlgorithm.Complexity.Linear),
                new SortAlgorithm("Radix MSD sort", SortingAlgorithms.RadixMSDSort, SortAlgorithm.Complexity.Linear),
                new SortAlgorithm("Bucket sort", SortingAlgorithms.BucketSort, SortAlgorithm.Complexity.Linear),
                new SortAlgorithm("Binary quick sort", SortingAlgorithms.BinaryQuickSort, SortAlgorithm.Complexity.Linear),

                //Block sort
                new SortAlgorithm("Block sort", SortingAlgorithms.BlockSort, SortAlgorithm.Complexity.NLogN),

                //Impractical/Joke sort
                new SortAlgorithm("Pancake sort", SortingAlgorithms.PancakeSort, SortAlgorithm.Complexity.NSquared),
                new SortAlgorithm("Stooge sort", SortingAlgorithms.StoogeSort, SortAlgorithm.Complexity.TooHigh),
                new SortAlgorithm("Slow sort", SortingAlgorithms.SlowSort, SortAlgorithm.Complexity.TooHigh),
                new SortAlgorithm("Stalin sort", SortingAlgorithms.StalinSort, SortAlgorithm.Complexity.Linear),
            ];

            for (int i = 0; i < sortAlgorithms.Length; i++)
            {
                if ((sortAlgorithms[i].meanComplexity == SortAlgorithm.Complexity.NSquared && list.Length > 1 << 15)
                    || (sortAlgorithms[i].meanComplexity == SortAlgorithm.Complexity.TooHigh && list.Length > 1 << 11)
                    || (sortAlgorithms[i].sortingAlgorithm == SortingAlgorithms.SlowSort && list.Length > 1 << 8)
                    || (sortAlgorithms[i].sortingAlgorithm == SortingAlgorithms.StrandSort && list.Length > 1 << 18)
                    || (sortAlgorithms[i].sortingNetwork is not null && list.Length > 1 << 13)) continue;

                sortAlgorithms[i].sortingNetwork?.Create(unsortedList.Length);

                Console.Write(sortAlgorithms[i].name + " : ");

                const int repeat = 1;
                Stopwatch stopWatch = new Stopwatch();
                for (int j = 0; j < repeat; j++)
                {
                    list.CopyTo(unsortedList, 0);
                    stopWatch.Start();
                    sortAlgorithms[i].sortingAlgorithm(unsortedList);
                    stopWatch.Stop();
                }

                TimeSpan benchmarkTime = stopWatch.Elapsed / repeat;
                if (benchmarkTime.TotalSeconds > 1.0) Console.Write(benchmarkTime.TotalSeconds + " s");
                else Console.Write(benchmarkTime.TotalMilliseconds + " ms");

                if (sortAlgorithms[i].sortingNetwork is not null) Console.Write(" (network swap list lenght : " + sortAlgorithms[i].sortingNetwork.GetSwapListLength() + ")");

                if (!SortingAlgorithms.CheckSorted(unsortedList))
                {
                    if (sortAlgorithms[i].sortingAlgorithm == SortingAlgorithms.StalinSort)
                    {
                        Console.WriteLine(" PURGED (new length : " + (unsortedList.Length - SortingAlgorithms.GetPurged(unsortedList)) + ")");
                    }
                    else
                    {
                        Console.WriteLine(" UNSORTED");
                    }
                }
                else if (!SortingAlgorithms.SameArray(unsortedList, checkList))
                {
                    Console.WriteLine(" BROKEN");
                }
                else
                {
                    Console.WriteLine();
                }
                if (unsortedList.Length <= 32) PrintList(unsortedList);

                sortAlgorithms[i].sortingNetwork?.Release();
            }
        }

        static void PrintList<T>(in T[] list)
        {
            Console.Write("{ ");
            for (int i = 0; i < list.Length; i++)
            {
                Console.Write(list[i]);
                if (i < list.Length - 1) Console.Write(", ");
            }
            Console.WriteLine(" }");
        }
    }
}
