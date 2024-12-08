#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>


namespace CUDASortingAlgorithms
{
    __declspec(dllexport) bool initCuda();
    __declspec(dllexport) void* allocCUDAArray(size_t size);
    __declspec(dllexport) void freeCUDAArray(void* devArr);

    __declspec(dllexport) void*& getDevArrForSorting();
    static void* devArrForSorting = nullptr;

    size_t RoundToNextPowerOf2(size_t val);

    __global__ void warm_up_gpu();

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
    __device__ inline void swap(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = tmp;
    }

    template<typename T>
    __global__ void parallelBitonicSortThreadOppositeSwap(T* arr, size_t len, size_t mergerSize)
    {
        unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

        size_t offset = i % (mergerSize >> 1);
        size_t k = (i / (mergerSize >> 1)) * mergerSize;
        size_t left = k + offset, right = k + mergerSize - 1 - offset;
        if (right < len && arr[left] > arr[right])
        {
            swap(arr[left], arr[right]);
        }
    }

    template<typename T>
    __global__ void parallelBitonicSortThreadExchangeSwap(T* arr, size_t len, size_t j)
    {
        unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

        const size_t diff = (j >> 1);
        size_t offset = i & (diff - 1);
        size_t k = (i / diff) * j;
        size_t left = k + offset, right = left + diff;
        if (right < len && arr[left] > arr[right])
        {
            swap(arr[left], arr[right]);
        }
    }

    template<typename T>
    __global__ void parallelOddEvenMergeSortThreadExchangeSwap(T* arr, size_t len, size_t mergerSize, size_t j)
    {
        unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

        const size_t diff = (j >> 1);
        size_t offset = i & (diff - 1);
        size_t k = (i / diff);
        const size_t a = ((mergerSize / j) - 1);
        size_t left = (k % a) * j + (k / a) * mergerSize + offset + diff, right = left + diff;
        if (right < len && arr[left] > arr[right])
        {
            swap(arr[left], arr[right]);
        }
    }

    template<typename T>
    __global__ void parallelPairwiseSortingNetworkThreadExchangeSwap(T* arr, size_t len, size_t mergerSize)
    {
        unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

        size_t diff = mergerSize >> 1;
        size_t k = (i / diff) * mergerSize;
        size_t offset = i & (diff - 1);
        size_t a = diff + k;
        size_t left = a + offset, right = left + diff;
        if (right < len && arr[left] > arr[right])
        {
            swap(arr[left], arr[right]);
        }
    }

    template<typename T>
    __global__ void parallelPairwiseSortingNetworkThreadPairwiseSwap(T* arr, size_t len, size_t mergerSize, size_t realSizeL)
    {
        unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

        size_t diff = mergerSize >> 1;
        size_t k = (i / diff) * mergerSize;
        size_t offset = i & (diff - 1);
        size_t a = k + diff;
        size_t left = a + offset, right = left + realSizeL - diff;
        if (right < len && arr[left] > arr[right])
        {
            swap(arr[left], arr[right]);
        }
    }

    template<typename T>
    void parallelBitonicSort(T* beg, T* end)
    {
        int device = 0;
        if (cudaGetDevice(&device) != cudaError::cudaSuccess) return;

        size_t realSize = RoundToNextPowerOf2(end - beg);
        size_t len2 = (realSize / 2);
        size_t grids = (len2 / 1024) + 1;
        size_t len3 = len2 / grids;
        if (len3 % grids > 0) len3 += 1;

        bool noPreAlloc = false;
        T* devArr = reinterpret_cast<T*>(devArrForSorting);
        if (!devArr)
        {
            if (cudaMalloc(&devArr, (end - beg) * sizeof(T)) != cudaError::cudaSuccess) return;
            noPreAlloc = true;
        }
        cudaMemcpy(devArr, beg, (end - beg) * sizeof(T), cudaMemcpyHostToDevice);

        for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
        {
            parallelBitonicSortThreadOppositeSwap<T> << <grids, len3 >> > (devArr, (end - beg), mergerSize);
            for (size_t j = (mergerSize >> 1); j > 1; j >>= 1)
            {
                parallelBitonicSortThreadExchangeSwap<T> << <grids, len3 >> > (devArr, (end - beg), j);
            }
        }

        cudaDeviceSynchronize();

        cudaMemcpy(beg, devArr, (end - beg) * sizeof(T), cudaMemcpyDeviceToHost);
        if (noPreAlloc) cudaFree(devArr);
    }

    template<typename T>
    void parallelOddEvenMergeSort(T* beg, T* end)
    {
        int device = 0;
        if (cudaGetDevice(&device) != cudaError::cudaSuccess) return;

        size_t realSize = RoundToNextPowerOf2(end - beg);
        size_t len2 = (realSize / 2);
        size_t grids = (len2 / 1024) + 1;
        size_t len3 = len2 / grids;
        if (len3 % grids > 0) len3 += 1;
        
        bool noPreAlloc = false;
        T* devArr = reinterpret_cast<T*>(devArrForSorting);
        if (!devArr)
        {
            if (cudaMalloc(&devArr, (end - beg) * sizeof(T)) != cudaError::cudaSuccess) return;
            noPreAlloc = true;
        }
        cudaMemcpy(devArr, beg, (end - beg) * sizeof(T), cudaMemcpyHostToDevice);

        for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
        {
            parallelBitonicSortThreadExchangeSwap<T> << <grids, len3 >> > (devArr, (end - beg), mergerSize);
            for (size_t j = (mergerSize >> 1); j > 1; j >>= 1)
            {
                size_t len3 = (len2 - len2 / mergerSize / j) / grids;
                if (len3 % grids > 0) len3 += 1;
                parallelOddEvenMergeSortThreadExchangeSwap<T> << <grids, len3 >> > (devArr, (end - beg), mergerSize, j);
            }
        }

        cudaDeviceSynchronize();

        cudaMemcpy(beg, devArr, (end - beg) * sizeof(T), cudaMemcpyDeviceToHost);
        if (noPreAlloc) cudaFree(devArr);
    }

    template<typename T>
    void parallelPairwiseSortingNetwork(T* beg, T* end)
    {
        int device = 0;
        if (cudaGetDevice(&device) != cudaError::cudaSuccess) return;

        size_t realSize = RoundToNextPowerOf2(end - beg);
        size_t len2 = (realSize / 2);
        size_t grids = (len2 / 1024) + 1;
        size_t len3 = len2 / grids;
        if (len3 % grids > 0) len3 += 1;

        bool noPreAlloc = false;
        T* devArr = reinterpret_cast<T*>(devArrForSorting);
        if (!devArr)
        {
            if (cudaMalloc(&devArr, (end - beg) * sizeof(T)) != cudaError::cudaSuccess) return;
            noPreAlloc = true;
        }
        cudaMemcpy(devArr, beg, (end - beg) * sizeof(T), cudaMemcpyHostToDevice);

        for (size_t mergerSize = 2; mergerSize <= realSize; mergerSize <<= 1)
        {
            parallelBitonicSortThreadExchangeSwap<T> << <grids, len3 >> > (devArr, (end - beg), mergerSize);
        }

        for (size_t mergerSize = realSize / 2, lIter = 0; mergerSize > 1; mergerSize >>= 1, lIter++)
        {
            for (size_t l = 0; l < lIter; l++)
            {
                //const size_t subblocks = ((mergerSize << (l + 1)) / (mergerSize >> 1) - 1) * (realSize / 2) / (mergerSize << (l + 1));
                parallelPairwiseSortingNetworkThreadPairwiseSwap << < grids, len3 >> > (devArr, (end - beg), mergerSize, realSize >> (l + 1));
            }

            parallelPairwiseSortingNetworkThreadExchangeSwap << <grids, len3 >> > (devArr, (end - beg), mergerSize);
        }

        cudaDeviceSynchronize();

        cudaMemcpy(beg, devArr, (end - beg) * sizeof(T), cudaMemcpyDeviceToHost);
        if (noPreAlloc) cudaFree(devArr);
    }
}