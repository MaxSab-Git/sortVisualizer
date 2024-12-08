#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sorts/misc/SampleArray.h"
#include "SortVisualizer.h"
#include "sorts/misc/SortingHelpers.h"
#include "sorts/AllSortingAlgorithms.h"
#include "shuffle/FullShuffle.h"
#include "shuffle/LuaShuffleAlgorithm.h"
#include <thread>
#include <condition_variable>
#include <filesystem>
#include <lua.hpp>
#include <iostream>

class VisualizerScreen
{
public:
	VisualizerScreen();
	~VisualizerScreen();

	void run();

private:
	enum class TestStep
	{
		Shuffle,
		Sort,
		CheckSorted,
		End
	};

	class SortingTask : sf::NonCopyable
	{
	public:
		SortingTask(sv::SortingAlgorithm& algorithm, size_t arraySize);

		sv::SortingAlgorithm& getSortingAlgorithm() const;
		size_t getArraySize() const;

	private:
		sv::SortingAlgorithm* m_algorithm;
		size_t m_arraySize;
	};

	void applyDefaultTextTemplate(sf::Text& text);
	void updateOffset();
	void resetStatDisplay();
	void updateStatDisplay();

	void processParametersEvent(const sf::Event& ev);
	void processTaskFileEvent(const sf::Event& ev);
	void startNextTask();

	void testSort(sv::SortingAlgorithm& sort, sv::ShuffleAlgorithm& shuffle);
	void pushTask(const std::vector<sv::SortingAlgorithm*>& algorithms, const std::string& name, size_t arraySize);
	bool processFile(const std::vector<sv::SortingAlgorithm*>& algorithms, const std::string& luaFile);

	bool isReadyToGo() const;

	sf::RenderWindow m_window;
	sv::SortVisualizer m_visualizer;
	size_t m_arraySize;
	TestStep m_testStep;
	lua_State* m_L;

	std::vector<sv::ShuffleAlgorithm*> m_shuffleAlgorithms;
	size_t m_shuffleIndex;

	std::vector<sv::SortingAlgorithm*> m_sortingAlgorithms;
	std::vector<sv::SortingAlgorithm*> m_additionalSortingAlgorithms;
	std::vector<sv::SortingAlgorithm*> *m_sortingAlgorithmList;
	size_t m_sortIndex;
	bool m_sorted;

	bool m_stepByStepMode;
	sf::Clock m_visualTimeClock;

	std::thread m_testSortThread;
	std::condition_variable m_cond;
	std::mutex m_condMutex;

	std::filesystem::path m_taskPath;
	std::filesystem::path m_additionalSortPath;
	std::vector<std::filesystem::path> m_taskFiles;
	size_t m_taskIndex;
	size_t m_totalNumTask;

	bool m_selectParameters;
	std::queue<SortingTask> m_taskQueue;
	float m_taskDelay;
	sv::LuaShuffleAlgorithm m_shuffleAlgorithm;
	std::filesystem::path m_shufflePath;

	sf::Font m_arial;

	sf::Text m_hello;
	sf::Text m_numTaskDisplay;
	sf::Text m_sortingAlgorithmDisplay;
	sf::Text m_arraySizeDisplay;
	sf::Text m_auxArrayShowDisplay;
	sf::Text m_visualTimeDisplay;
	sf::Text m_comparisonsDisplay;
	sf::Text m_writesMainDisplay;
	sf::Text m_writesAuxDisplay;

	sf::RectangleShape m_maskRect;
	sf::Text m_fileSelectedDisplay;
};

