#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include "SortingAlgorithms.h"
#include "SampleArray.h"
#include <Windows.h>

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
using SortingAlgorithmDelegate = void (*)(T*, T*);

using ElemType = unsigned int;

struct SortAlgorithm
{
	enum class Complexity
	{
		NSquared,
		LessThanNSquared,
		LogNSquared,
		NLogN,
		NLogNSquared,
		Linear,
		TooHigh
	};

	SortAlgorithm(const char* name, SortingAlgorithmDelegate<ElemType> sortAlgorithm, Complexity meanComplexity, bool needCuda = false) : 
		name(name),
		sortAlgorithm(sortAlgorithm),
		meanComplexity(meanComplexity),
		needCuda(needCuda)
	{}
	
	const char* name;
	SortingAlgorithmDelegate<ElemType> sortAlgorithm;
	Complexity meanComplexity;
	bool needCuda;
};

int main()
{
	bool CUDAEnabled = false;
	std::cout << "CUDA init : ";
	if (!CUDASortingAlgorithms::initCuda())
	{
		std::cout << "failed. Parallel algorithms won't be tested." << std::endl;
	}
	else
	{
		std::cout << "Succed" << std::endl;
		CUDAEnabled = true;
	}
	CUDASortingAlgorithms::getDevArrForSorting() = nullptr;

	const size_t arrLen = 1 << 19;

	std::default_random_engine eng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count() & 0xffffffff));
	std::uniform_int_distribution<int> random(2, arrLen);

	std::vector<ElemType> list = SampleArray::createShuffledArray<ElemType>(arrLen);
	std::vector<ElemType> unsortedList;
	std::vector<ElemType> checkList = SampleArray::createIncrementalArray<ElemType>(list.size());

	if (CUDAEnabled)
	{
		std::cout << "Preallocation on device... ";
		CUDASortingAlgorithms::getDevArrForSorting() = CUDASortingAlgorithms::allocCUDAArray(sizeof(ElemType) * list.size());
		if (!CUDASortingAlgorithms::getDevArrForSorting())
		{
			std::cout << "failed. Parallel algorithms won't be tested.\n";
			CUDAEnabled = false;
		}
		else std::cout << "done !\n";
	}
	std::cout << std::endl;

	std::cout << "Array size : " << list.size() << "\n" << std::endl;
	std::cout.precision(13);

	std::vector<SortAlgorithm> sortAlgorithms({
		//Exchange sort
		SortAlgorithm("Bubble sort", SortingAlgorithms::bubbleSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Bubble sort (optimized)", SortingAlgorithms::optimizedBubbleSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Cocktail shaker sort", SortingAlgorithms::cocktailShakerSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Cocktail shaker sort (optimized)", SortingAlgorithms::optimizedCocktailShakerSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Odd even sort", SortingAlgorithms::oddEvenSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Circle sort", SortingAlgorithms::circleSort, SortAlgorithm::Complexity::NLogNSquared),
		SortAlgorithm("Comb sort", SortingAlgorithms::combSort, SortAlgorithm::Complexity::LessThanNSquared),
		SortAlgorithm("Hybrid comb sort", SortingAlgorithms::hybridCombSort, SortAlgorithm::Complexity::LessThanNSquared),

		//Selection sort
		SortAlgorithm("Selection sort", SortingAlgorithms::selectionSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Double selection sort", SortingAlgorithms::doubleSelectionSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Exchange sort", SortingAlgorithms::exchangeSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Heap sort", SortingAlgorithms::heapSort, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Cycle sort", SortingAlgorithms::cycleSort, SortAlgorithm::Complexity::NSquared),

		//Insertion sort
		SortAlgorithm("Insertion sort", SortingAlgorithms::insertionSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Binary insertion sort", SortingAlgorithms::binaryInsertionSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Shell sort", SortingAlgorithms::shellSort, SortAlgorithm::Complexity::LessThanNSquared),

		//Quick sort
		SortAlgorithm("Quick sort (left/right pointers)", SortingAlgorithms::quickSort, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Quick sort (left/left pointers)", SortingAlgorithms::quickSortLL, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Dual pivot quick sort", SortingAlgorithms::dualPivotQuickSort, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Proportion extend sort", SortingAlgorithms::proportionExtendSort, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Intro sort", SortingAlgorithms::introSort, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Pattern defeating quick sort", SortingAlgorithms::patternDefeatingQuickSort, SortAlgorithm::Complexity::NLogN),

		//Merge sort
		SortAlgorithm("Merge sort", SortingAlgorithms::mergeSort, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Iterative merge sort", SortingAlgorithms::iterativeMergeSort, SortAlgorithm::Complexity::NLogN),
		SortAlgorithm("Rotate merge sort", SortingAlgorithms::rotateMergeSort, SortAlgorithm::Complexity::NLogNSquared),
		SortAlgorithm("In place merge sort", SortingAlgorithms::inPlaceMergeSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Weave sort", SortingAlgorithms::weaveSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Strand sort", SortingAlgorithms::strandSort, SortAlgorithm::Complexity::NLogNSquared),
		SortAlgorithm("Tim sort", SortingAlgorithms::timSort, SortAlgorithm::Complexity::NLogN),

		//Sorting network
		SortAlgorithm("Bitonic sort", SortingAlgorithms::bitonicSort, SortAlgorithm::Complexity::NLogNSquared),
		SortAlgorithm("Odd even merge sort", SortingAlgorithms::oddEvenMergeSort, SortAlgorithm::Complexity::NLogNSquared),
		SortAlgorithm("Pairwise sorting network", SortingAlgorithms::pairwiseSortingNetwork, SortAlgorithm::Complexity::NLogNSquared),
		SortAlgorithm("Bitonic sort (parallel)", CUDASortingAlgorithms::parallelBitonicSort, SortAlgorithm::Complexity::LogNSquared, true),
		SortAlgorithm("Odd even merge sort (parallel)", CUDASortingAlgorithms::parallelOddEvenMergeSort, SortAlgorithm::Complexity::LogNSquared, true),
		SortAlgorithm("Pairwise sorting network (parallel)", CUDASortingAlgorithms::parallelPairwiseSortingNetwork, SortAlgorithm::Complexity::LogNSquared, true),

		//Bucket sort
		SortAlgorithm("LSD radix sort (b = 16)", SortingAlgorithms::LSDRadixSort<ElemType, 4>, SortAlgorithm::Complexity::Linear),
		SortAlgorithm("MSD radix sort (b = 16)", SortingAlgorithms::MSDRadixSort<ElemType, 4>, SortAlgorithm::Complexity::Linear),
		SortAlgorithm("Bucket sort (b = n/16)", SortingAlgorithms::bucketSort<ElemType, 16>, SortAlgorithm::Complexity::Linear),
		SortAlgorithm("Binary quick sort", SortingAlgorithms::binaryQuickSort<ElemType>, SortAlgorithm::Complexity::NLogN),

		//Impractical/Joke sort
		SortAlgorithm("Pancake sort", SortingAlgorithms::pancakeSort, SortAlgorithm::Complexity::NSquared),
		SortAlgorithm("Stooge sort", SortingAlgorithms::stoogeSort, SortAlgorithm::Complexity::TooHigh),
		SortAlgorithm("Slow sort", SortingAlgorithms::slowSort, SortAlgorithm::Complexity::TooHigh)
	});

	for (size_t i = 0; i < sortAlgorithms.size(); i++)
	{
		if ((sortAlgorithms[i].meanComplexity == SortAlgorithm::Complexity::NSquared && list.size() > static_cast<size_t>(1) << 15)
			|| (sortAlgorithms[i].sortAlgorithm == SortingAlgorithms::strandSort<ElemType> && list.size() > static_cast<size_t>(1) << 18)
			|| (sortAlgorithms[i].sortAlgorithm == SortingAlgorithms::slowSort<ElemType> && list.size() > static_cast<size_t>(1) << 8)
			|| (sortAlgorithms[i].meanComplexity == SortAlgorithm::Complexity::TooHigh && list.size() > static_cast<size_t>(1) << 11)
			|| (sortAlgorithms[i].needCuda && (!CUDAEnabled))) continue;

		if (sortAlgorithms[i].needCuda) sortAlgorithms[i].sortAlgorithm(unsortedList.data(), unsortedList.data() + unsortedList.size()); //warm up

		std::cout << sortAlgorithms[i].name << " : ";

		unsortedList = list;
		auto start = std::chrono::steady_clock::now();
		sortAlgorithms[i].sortAlgorithm(unsortedList.data(), unsortedList.data() + unsortedList.size());
		auto benchmarkTime = std::chrono::steady_clock::now() - start;

		if (benchmarkTime.count() / 1000000 > 1000) std::cout << ((double)benchmarkTime.count() / 1000000000.0) << " s";
		else std::cout << ((double)benchmarkTime.count() / 1000000.0) << " ms";

		if (!SortingAlgorithms::checkSorted(unsortedList.data(), list.size()))
		{
			SortingAlgorithms::heapSort(unsortedList.data(), unsortedList.data() + unsortedList.size());

			if (SortingAlgorithms::sameArray(unsortedList.data(), checkList.data(), list.size())) std::cout << " UNSORTED" << std::endl;
			else std::cout << " BROKEN" << std::endl;
		}
		else if (!SortingAlgorithms::sameArray(unsortedList.data(), checkList.data(), list.size()))
		{
			std::cout << " BROKEN" << std::endl;
		}
		else
		{
			std::cout << std::endl;
		}
		if (list.size() <= 32) printList(unsortedList.data(), unsortedList.size());
	}

	if (CUDAEnabled)
	{
		CUDASortingAlgorithms::freeCUDAArray(CUDASortingAlgorithms::getDevArrForSorting());
		CUDASortingAlgorithms::getDevArrForSorting() = nullptr;
	}

	std::cout << "\nAll sorting algorithms have been tested. Press ENTER to exit...";
	std::cin.get();

	return 0;
}