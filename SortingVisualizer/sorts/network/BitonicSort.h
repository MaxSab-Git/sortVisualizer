#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class BitonicSort : public SortingAlgorithm
	{
	public:
		BitonicSort(float timeMultiplier = 10.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;
	};
}