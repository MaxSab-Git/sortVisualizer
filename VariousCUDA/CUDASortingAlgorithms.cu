#include "CUDASortingAlgorithms.cuh"

namespace CUDASortingAlgorithms
{
    __declspec(dllexport) bool initCuda()
    {
        cudaError_t result = cudaSetDevice(0);
        if (result != cudaError::cudaSuccess)
        {
            return false;
        }
        warm_up_gpu << <1, 1024 >> > ();
        return true;
    }

    __declspec(dllexport) void* allocCUDAArray(size_t size)
    {
        void* devArr = nullptr;
        cudaMalloc(&devArr, size);
        return devArr;
    }

    __declspec(dllexport) void freeCUDAArray(void* devArr)
    {
        cudaFree(devArr);
    }

    void*& getDevArrForSorting()
    {
        return devArrForSorting;
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

    __global__ void warm_up_gpu()
    {
        unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
        float ia, ib;
        ia = ib = 0.0f;
        ib += ia + tid;
    }

	template __declspec(dllexport) void parallelBitonicSort<int>(int* arr, int* end);
	template __declspec(dllexport) void parallelBitonicSort<unsigned int>(unsigned int* arr, unsigned int* end);

    template __declspec(dllexport) void parallelOddEvenMergeSort<int>(int* arr, int* end);
    template __declspec(dllexport) void parallelOddEvenMergeSort<unsigned int>(unsigned int* arr, unsigned int* end);

    template __declspec(dllexport) void parallelPairwiseSortingNetwork<int>(int* arr, int* end);
    template __declspec(dllexport) void parallelPairwiseSortingNetwork<unsigned int>(unsigned int* arr, unsigned int* end);
}