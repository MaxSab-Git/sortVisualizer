#pragma once
#include <vector>
#include <chrono>
#include <random>

namespace SampleArray
{
	template<typename T>
	void shuffle(T* arr, size_t len)
	{
		std::default_random_engine eng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count() & 0xffffffff));
		std::uniform_int_distribution<size_t> random(0, static_cast<size_t>(len - 1));

		for (size_t i = 0; i < len; i++)
		{
			std::swap(arr[i], arr[random(eng)]);
		}
	}

	template<typename T>
	std::vector<T> createIncrementalArray(size_t len, size_t uniques = 0)
	{
		static_assert(std::is_integral<T>::value, "T is not integral");

		if (uniques == 0 || uniques > len) uniques = len;
		size_t miss = len % uniques;
		size_t iter = len / uniques;

		if (miss != 0) iter += 1;

		std::vector<T> arr(len);
		for (size_t i = 0; i < uniques; i++)
		{
			for (size_t j = 0; j < iter; j++)
			{
				if (i * iter + j >= len) break;
				arr[i * iter + j] = static_cast<T>(i) + 1;
			}
		}
		
		return arr;
	}

	template<typename T>
	std::vector<T> createShuffledArray(size_t len, size_t uniques = 0)
	{
		std::vector<T> arr = createIncrementalArray<T>(len, uniques);
		shuffle(arr.data(), len);
		return arr;
	}
}