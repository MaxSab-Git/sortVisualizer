#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class WeakHeapSort : public SortingAlgorithm
	{
	public:
		WeakHeapSort(float timeMultiplier = 5.5f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		void weakHeapify(SortVisualizer& visualizer, bool* flipped);
		void siftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t end, bool* flipped);
		size_t findDistinguishedAncestor(SortVisualizer& visualizer, size_t start, size_t i, bool* flipped);
		void join(SortVisualizer& visualizer, size_t start, size_t i, bool* flipped);
		size_t getLeftChildNodeIndex(size_t i, bool* flipped);
		size_t getRightChildNodeIndex(size_t i, bool* flipped);
	};
}