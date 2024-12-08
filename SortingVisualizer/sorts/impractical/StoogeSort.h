#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class StoogeSort : public SortingAlgorithm
	{
	public:
		StoogeSort(float timeMultiplier = 10.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		void subSort(SortVisualizer& visualizer, size_t start, size_t end);
	};
}