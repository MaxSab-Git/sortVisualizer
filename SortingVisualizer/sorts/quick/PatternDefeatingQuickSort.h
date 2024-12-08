#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class PatternDefeatingQuickSort : public SortingAlgorithm
	{
	public:
		PatternDefeatingQuickSort(float timeMultiplier = 10.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		void subSort(SortVisualizer& visualizer, size_t start, size_t end, size_t recursions);
		size_t findMedian(SortVisualizer& visualizer, size_t first, size_t middle, size_t last);
	};
}