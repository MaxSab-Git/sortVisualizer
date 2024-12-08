using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace VariousThings
{
    public abstract class SortingNetwork
    {
        protected (int, int)[]? swapList;
        public SortingNetwork()
        {
            swapList = null;
        }

        public int GetSwapListLength()
        {
            return (swapList is null) ? 0 : swapList.Length;
        }

        public SortingNetwork(int size)
        {
            Create(size);
        }

        public abstract void Create(int size);
        public void Release()
        {
            swapList = null;
        }

        public void Sort<T>(T[] arr) where T : IComparable<T>
        {
            if (swapList is null) return;

            for (int i = 0; i < swapList.Length; i++)
            {
                ref var swap = ref swapList[i];

                if (arr[swap.Item1].CompareTo(arr[swap.Item2]) > 0)
                {
                    T tmp = arr[swap.Item1];
                    arr[swap.Item1] = arr[swap.Item2];
                    arr[swap.Item2] = tmp;
                }
            }
        }
    }
}
