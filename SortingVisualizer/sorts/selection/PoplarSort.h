#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class PoplarSort : public SortingAlgorithm
	{
	public:
		PoplarSort(float timeMultiplier = 5.5f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		size_t leonardoHeapify(SortVisualizer& visualizer, sf::Uint64& treeLevelMask);
		bool combineTree(sf::Uint64& treeLevelMask);
		void rootInsertion(SortVisualizer& visualizer, const sf::Uint64& treeLevelMask, size_t index, size_t level);
		void shiftDown(SortVisualizer& visualizer, size_t start, size_t i, size_t level);
		size_t getLevelSize(size_t level);
		size_t getLeftChildNodeIndex(size_t i, size_t level);
		size_t getRightChildNodeIndex(size_t i);
		void extractMax(SortVisualizer& visualizer, const sf::Uint64& treeLevelMask, size_t index, size_t level);
	};
}