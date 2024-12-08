#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class CircleSort : public SortingAlgorithm
	{
	public:
		CircleSort(float timeMultiplier = 20.f);

		void sort(SortVisualizer& visualizer);
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;
	};
}