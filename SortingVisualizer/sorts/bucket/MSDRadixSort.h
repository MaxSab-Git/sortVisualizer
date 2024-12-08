#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class MSDRadixSort : public SortingAlgorithm
	{
	public:
		MSDRadixSort(size_t baseBitOffset, float timeMultiplier = 1.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		size_t m_bits;

		void subSort(SortVisualizer& visualizer, size_t start, size_t end, size_t* counter, unsigned int round);
	};
}