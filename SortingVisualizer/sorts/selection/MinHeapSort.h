#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class MinHeapSort : public SortingAlgorithm
	{
	public:
		MinHeapSort(float timeMultiplier = 5.5f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		void minHeapify(SortVisualizer& visualizer);
		void shiftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t end);
		size_t getLeftChildNodeIndex(size_t i);
	};
}