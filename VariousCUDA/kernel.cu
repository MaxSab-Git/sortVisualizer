#ifndef _DLL

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "CUDASortingAlgorithms.cuh"

#include <stdio.h>
#include <vector>
#include <chrono>
#include "SampleArray.h"

template<typename T>
void printList(T* list, size_t len)
{
	std::cout << "{ ";
	for (int i = 0; i < len; i++)
	{
		std::cout << list[i];
		if (i < len - 1) std::cout << ", ";
	}
	std::cout << " }" << std::endl;
}

template<typename T>
using SortingAlgorithmDelegate = void (*)(T*, size_t);

using ElemType = unsigned int;

struct SortAlgorithm
{
	enum class Complexity
	{
		NSquared,
		LessThanNSquared,
		NLogN,
		NLogNSquared,
		Linear,
		TooHigh
	};

	SortAlgorithm(const char* name, SortingAlgorithmDelegate<ElemType> sortAlgorithm, Complexity meanComplexity) : name(name), sortAlgorithm(sortAlgorithm), meanComplexity(meanComplexity) {}

	const char* name;
	SortingAlgorithmDelegate<ElemType> sortAlgorithm;
	Complexity meanComplexity;
};

int main()
{
    CUDASortingAlgorithms::initCuda();
	const size_t arrLen = 29;

	std::vector<ElemType> list = SampleArray::createShuffledArray<ElemType>(arrLen);
	std::vector<ElemType> unsortedList;
	std::vector<ElemType> checkList = SampleArray::createIncrementalArray<ElemType>(list.size());

	std::cout << "Array size : " << list.size() << "\n" << std::endl;

	std::vector<SortAlgorithm> sortAlgorithms({
		SortAlgorithm("Bitonic sort (parallel)", CUDASortingAlgorithms::parallelBitonicSort, SortAlgorithm::Complexity::NLogN),
	});

	for (size_t i = 0; i < sortAlgorithms.size(); i++)
	{
		std::cout << sortAlgorithms[i].name << " : ";

		unsortedList = list;
		auto start = std::chrono::steady_clock::now();
		sortAlgorithms[i].sortAlgorithm(unsortedList.data(), unsortedList.size());
		auto benchmarkTime = std::chrono::steady_clock::now() - start;

		if (benchmarkTime.count() / 1000000 > 1000) std::cout << ((double)benchmarkTime.count() / 1000000000.0) << " s";
		else std::cout << ((double)benchmarkTime.count() / 1000000.0) << " ms";

		if (!CUDASortingAlgorithms::checkSorted(unsortedList.data(), list.size()))
		{
			std::cout << " UNSORTED" << std::endl;
		}
		else if (!CUDASortingAlgorithms::sameArray(unsortedList.data(), checkList.data(), list.size()))
		{
			std::cout << " BROKEN" << std::endl;
		}
		else
		{
			std::cout << std::endl;
		}
		if (list.size() <= 32) printList(unsortedList.data(), unsortedList.size());
	}

    cudaError_t cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

#endif