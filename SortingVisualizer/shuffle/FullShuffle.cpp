#include "FullShuffle.h"
#include <random>

sv::FullShuffle::FullShuffle(float timeMultiplier) : ShuffleAlgorithm(timeMultiplier)
{
}

void sv::FullShuffle::shuffle(SortVisualizer& visualizer)
{
	float timer = getTimeMultiplier() / visualizer.getArrayLength();
	visualizer.setCompareDelay(timer);
	visualizer.setWriteDelay(timer);
	visualizer.setSwapDelay(timer);

	if (visualizer.getArrayLength() == 0) return;

	std::default_random_engine eng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count() & 0xffffffff));
	std::uniform_int_distribution<size_t> random(0, static_cast<size_t>(visualizer.getArrayLength() - 1));

	for (size_t i = 0; i < visualizer.getArrayLength(); i++)
	{
		if (!visualizer.visualizerSwap(i, random(eng))) return;
	}

	visualizer.visualizerDone();
}
