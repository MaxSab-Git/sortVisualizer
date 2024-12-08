#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class LSDRadixSort : public SortingAlgorithm
	{
	public:
		LSDRadixSort(size_t baseBitOffset, float timeMultiplier = 1.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		size_t m_bits;
	};
}