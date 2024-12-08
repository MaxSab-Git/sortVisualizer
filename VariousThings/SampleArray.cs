using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VariousThings
{
    public static class SampleArray
    {
        public static void Shuffle<T>(T[] arr)
        {
            Random random = new Random((int)(DateTime.Now.Ticks & 0xffffffffL));
            for (int i = 0; i < arr.Length; i++)
            {
                int index = random.Next(arr.Length);
                T tmp = arr[i];
                arr[i] = arr[index];
                arr[index] = tmp;
            }
        }

        public static int[] CreateIncrementalArray(int size) 
        {
            int[] arr = new int[size];
            for (int i = 0; i < size; i++)
            {
                arr[i] = i + 1;
            }
            return arr;
        }

        public static int[] CreateAlmostSortedArray(int size, int unsortedIndex = -1, int unsortedNumber = 1)
        {
            if (unsortedIndex < 0 || unsortedIndex >= size) unsortedIndex = size - 1;
            if (unsortedNumber < 1 || unsortedNumber > size) unsortedNumber = size;

            int[] arr = new int[size];
            arr[unsortedIndex] = unsortedNumber;
            for (int i = 0; i < size - 1; i++)
            {
                int finalIndex = (i >= unsortedIndex) ? i + 1 : i;
                arr[finalIndex] = i + ((i + 1 < unsortedNumber) ? 1 : 2);
            }
            return arr;
        }

        public static int[] CreateShuffledArray(int size)
        {
            int[] arr = CreateIncrementalArray(size);
            Shuffle(arr);
            return arr;
        }
    }
}
