#include "VisualizerScreen.h"
#include "LuaHelper.h"

VisualizerScreen::VisualizerScreen() :
	m_window(sf::VideoMode(1600, 900), "Sorting visualizer", sf::Style::Close),
	m_taskPath("task"),
	m_additionalSortPath("additionalSort"),
	m_taskIndex(0),
	m_totalNumTask(0),
	m_arraySize(1 << 8),
	m_selectParameters(true),
	m_taskDelay(0.f),
	m_sortingAlgorithmList(nullptr),
	m_sortIndex(0),
	m_sorted(true),
	m_stepByStepMode(false),
	m_testStep(TestStep::End),
	m_shuffleIndex(0),
	m_L(luaL_newstate())
{
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);

	luaL_openlibs(m_L);
	sv::LuaHelper::open_visualizerLib(m_L, m_visualizer);

	m_shuffleAlgorithm.setLua(m_L);
	m_shuffleAlgorithm.setLuaShuffleFuncName("shuffle");

	if (std::filesystem::is_directory(m_taskPath))
	{
		for (std::filesystem::recursive_directory_iterator dir(m_taskPath); dir != std::filesystem::recursive_directory_iterator(); ++dir)
		{
			if (std::filesystem::is_regular_file(dir->path()) && dir->path().extension() == ".lua")
			{
				m_taskFiles.push_back(dir->path());
			}
		}
	}

	m_arial.loadFromFile("Ressources/arial.ttf");

	applyDefaultTextTemplate(m_hello);
	m_hello.setString(": Done !");
	m_hello.setPosition(0.f, 0.f);

	applyDefaultTextTemplate(m_numTaskDisplay);
	m_numTaskDisplay.setString("");
	m_numTaskDisplay.setPosition(0.f, 0.f);

	m_visualizer.setVisualizerSize(sf::Vector2f(1580, 750));
	m_visualizer.setPosition(10, 890);
	m_visualizer.setArray(SampleArray::createIncrementalArray<unsigned int>(m_arraySize));

	m_visualizer.setQueueLimit(1 << 19);

	m_sortingAlgorithms =
	{
		new sv::BubbleSort(30.f),
		new sv::OptimizedBubbleSort(30.f),
		new sv::CocktailShakerSort(30.f),
		new sv::OptimizedCocktailShakerSort(30.f),
		new sv::OddEvenSort(30.f),
		new sv::CombSort(4.f),
		new sv::HybridCombSort(4.f),
		new sv::CircleSort(20.f),
		new sv::SelectionSort(30.f),
		new sv::DoubleSelectionSort(30.f),
		new sv::ExchangeSort(30.f),
		new sv::CycleSort(20.f),
		new sv::HeapSort(5.5f),
		new sv::MinHeapSort(5.5f),
		new sv::WeakHeapSort(5.5f),
		new sv::SmoothSort(2.f),
		new sv::PoplarSort(2.f),
		new sv::InsertionSort(30.f),
		new sv::BinaryInsertionSort(60.f),
		new sv::ShellSort(4.f),
		new sv::QuickSort(10.f),
		new sv::QuickSortLL(10.f),
		new sv::DualPivotQuickSort(10.f),
		new sv::ProportionExtendSort(10.f),
		new sv::IntroSort(10.f),
		new sv::PatternDefeatingQuickSort(10.f),
		new sv::MergeSort(10.f),
		new sv::IterativeMergeSort(10.f),
		new sv::RotateMergeSort(80.f),
		new sv::InPlaceMergeSort(160.f),
		new sv::WeaveSort(80.f),
		new sv::TimSort(10.f),
		new sv::BitonicSort(50.f),
		new sv::OddEvenMergeSort(50.f),
		new sv::PairwiseSortingNetwork(50.f),
		new sv::LSDRadixSort(2, 1.f),
		new sv::MSDRadixSort(2, 1.f),
		new sv::BucketSort(10, 2.f),
		new sv::BinaryQuickSort(10.f),
		new sv::PancakeSort(50.f),
		new sv::StoogeSort(10.f),
		new sv::SlowSort(10.f)
	};

	m_shuffleAlgorithms =
	{
		new sv::FullShuffle(2.0f)
	};

	if (std::filesystem::is_directory(m_additionalSortPath))
	{
		for (std::filesystem::recursive_directory_iterator dir(m_additionalSortPath); dir != std::filesystem::recursive_directory_iterator(); ++dir)
		{
			if (std::filesystem::is_regular_file(dir->path()) && dir->path().extension() == ".lua")
			{
				sv::LuaSortingAlgorithm* luaSort = new sv::LuaSortingAlgorithm(m_L);
				luaSort->setLuaSortFuncName(dir->path().string());
				m_additionalSortingAlgorithms.push_back(luaSort);
			}
		}
	}

	m_sortingAlgorithmList = &m_sortingAlgorithms;

	applyDefaultTextTemplate(m_sortingAlgorithmDisplay);
	m_sortingAlgorithmDisplay.setString((*m_sortingAlgorithmList)[m_sortIndex]->getAlgorithmName());
	m_sortingAlgorithmDisplay.setPosition(0.f, 0.f);

	applyDefaultTextTemplate(m_arraySizeDisplay);
	m_arraySizeDisplay.setPosition(0.f, 95.f);
	m_arraySizeDisplay.setString("Array size : " + std::to_string(m_arraySize));

	applyDefaultTextTemplate(m_auxArrayShowDisplay);
	m_auxArrayShowDisplay.setPosition(0.f, 120.f);
	m_auxArrayShowDisplay.setString("Show auxiliary array : " + std::string(m_visualizer.isAuxiliaryArrayWillBeShown() ? "on" : "off"));

	applyDefaultTextTemplate(m_visualTimeDisplay);
	m_visualTimeDisplay.setPosition(0.f, 25.f);

	applyDefaultTextTemplate(m_comparisonsDisplay);
	m_comparisonsDisplay.setPosition(0.f, 155.f);

	applyDefaultTextTemplate(m_writesMainDisplay);
	m_writesMainDisplay.setPosition(0.f, 180.f);

	applyDefaultTextTemplate(m_writesAuxDisplay);
	m_writesAuxDisplay.setPosition(0.f, 205.f);

	resetStatDisplay();

	m_maskRect.setSize(sf::Vector2f(m_window.getSize()));
	m_maskRect.setFillColor(sf::Color(0, 0, 0, 180));

	applyDefaultTextTemplate(m_fileSelectedDisplay);

	if (m_taskFiles.empty())
	{
		m_fileSelectedDisplay.setString("File not found.");
	}
	else
	{
		m_fileSelectedDisplay.setString(m_taskFiles.front().string());
	}

	updateOffset();
}

VisualizerScreen::~VisualizerScreen()
{
	if (m_testSortThread.joinable())
	{
		m_visualizer.abort();
		if (m_testStep != TestStep::End)
		{
			std::lock_guard<std::mutex> lock(m_condMutex);
			m_testStep = TestStep::End;
		}
		m_cond.notify_all();
		m_testSortThread.join();
	}

	for (size_t i = 0; i < m_sortingAlgorithms.size(); i++)
	{
		delete m_sortingAlgorithms[i];
	}

	for (size_t i = 0; i < m_additionalSortingAlgorithms.size(); i++)
	{
		delete m_additionalSortingAlgorithms[i];
	}

	for (size_t i = 0; i < m_shuffleAlgorithms.size(); i++)
	{
		delete m_shuffleAlgorithms[i];
	}

	lua_close(m_L);
}

void VisualizerScreen::run()
{
	sf::Clock clock;
	sf::Event ev;
	while (m_window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		while (m_window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				m_window.close();
			}
			else if (ev.type == sf::Event::KeyPressed)
			{
				if (ev.key.code == sf::Keyboard::Key::Escape)
				{
					if (!m_visualizer.isAborted() && m_testStep != TestStep::End)
					{
						m_visualizer.abort();
						if (m_testStep != TestStep::End)
						{
							std::lock_guard<std::mutex> lock(m_condMutex);
							m_testStep = TestStep::End;
						}
						m_cond.notify_all();
						m_hello.setString(": Aborted.");
						m_taskQueue = std::queue<SortingTask>();
					}
				}
				if (ev.key.code == sf::Keyboard::Key::Enter)
				{
					if (isReadyToGo() && ((!m_selectParameters && !m_taskFiles.empty()) || (m_selectParameters && !m_sortingAlgorithmList->empty())))
					{
						if (m_testSortThread.joinable())
						{
							m_testSortThread.join();
						}

						if (m_selectParameters)
						{
							m_visualizer.setArray(SampleArray::createIncrementalArray<unsigned int>(m_arraySize));
							m_visualizer.visualizerDelay(0.5f);

							m_testStep = TestStep::Shuffle;
							m_testSortThread = std::thread(&VisualizerScreen::testSort, this, std::ref(*(*m_sortingAlgorithmList)[m_sortIndex]), std::ref(*m_shuffleAlgorithms[m_shuffleIndex]));

							m_hello.setString(": Shuffling...");

							resetStatDisplay();
							updateOffset();
						}
						else
						{
							if (!processFile(m_sortingAlgorithms, m_taskFiles[m_taskIndex].string()))
							{
								m_fileSelectedDisplay.setString(m_taskFiles[m_taskIndex].string() + " Error");
								m_taskQueue = std::queue<SortingTask>();
								continue;
							}

							m_totalNumTask = m_taskQueue.size();
							m_selectParameters = true;

							startNextTask();
						}
					}
					else if (m_stepByStepMode && !m_visualizer.isDone() && m_testStep == TestStep::Sort)
					{
						m_visualizer.step();
					}
				}
				else if (ev.key.code == sf::Keyboard::Key::D)
				{
					m_stepByStepMode = !m_stepByStepMode;
				}
				else if (ev.key.code == sf::Keyboard::Key::Tab)
				{
					if (isReadyToGo())
					{
						m_selectParameters = !m_selectParameters;
						m_taskIndex = 0;

						if (m_taskFiles.empty())
						{
							m_sortingAlgorithmDisplay.setString("No task availible in the \"task/\" folder.");
						}
						else
						{
							m_fileSelectedDisplay.setString(m_taskFiles[m_taskIndex].string());
						}

						m_sortingAlgorithmDisplay.setString((*m_sortingAlgorithmList)[m_sortIndex]->getAlgorithmName());
						m_arraySizeDisplay.setString("Array size : " + std::to_string(m_arraySize));

						updateOffset();
					}
				}
				else if (m_selectParameters)
				{
					processParametersEvent(ev);
				}
				else
				{
					processTaskFileEvent(ev);
				}
			}
		}

		if (m_testStep != TestStep::End)
		{
			if (m_testStep != TestStep::Sort || !m_stepByStepMode)
			{
				m_visualizer.update(deltaTime);
			}

			if (m_visualizer.isDone())
			{
				if (m_testStep == TestStep::Shuffle)
				{
					{
						std::lock_guard<std::mutex> lock(m_condMutex);
						m_testStep = TestStep::Sort;
						m_hello.setString(": Sorting...");

						updateOffset();

						m_visualTimeClock.restart();
						m_visualizer.visualizerDelay(1.f);
						m_visualizer.resetStatistics();
						if (m_stepByStepMode) m_visualizer.step();
					}
					m_cond.notify_all();
				}
				else if (m_testStep == TestStep::Sort)
				{
					{
						std::lock_guard<std::mutex> lock(m_condMutex);
						m_testStep = TestStep::CheckSorted;
						updateStatDisplay();
						m_hello.setString(": Check...");

						updateOffset();

						m_visualizer.visualizerDelay(0.f);
					}
					m_cond.notify_all();
				}
				else if (m_testStep == TestStep::CheckSorted)
				{
					if (m_sorted) m_hello.setString(": Done !");
					else
					{
						if (m_sortingAlgorithmList == &m_sortingAlgorithms) m_hello.setString(": Not sorted, please report.");
						else m_hello.setString(": Oops, not sorted.");
					}

					updateOffset();

					m_testStep = TestStep::End;
					if (!m_taskQueue.empty())
					{
						m_taskQueue.pop();
						m_taskDelay = 2.f;
					}
				}
			}

			if (m_testStep == TestStep::Sort)
			{
				updateStatDisplay();
			}
		}

		if (m_testStep == TestStep::End && !m_taskQueue.empty() && (m_taskDelay -= deltaTime) <= 0.f)
		{
			startNextTask();
		}

		m_window.clear();
		m_window.draw(m_visualizer);
		m_window.draw(m_hello);
		m_window.draw(m_numTaskDisplay);
		m_window.draw(m_sortingAlgorithmDisplay);
		m_window.draw(m_visualTimeDisplay);
		m_window.draw(m_arraySizeDisplay);
		m_window.draw(m_auxArrayShowDisplay);
		m_window.draw(m_comparisonsDisplay);
		m_window.draw(m_writesMainDisplay);
		m_window.draw(m_writesAuxDisplay);
		if (!m_selectParameters)
		{
			m_window.draw(m_maskRect);
			m_window.draw(m_fileSelectedDisplay);
		}
		m_window.display();
	}
}

void VisualizerScreen::applyDefaultTextTemplate(sf::Text& text)
{
	text.setFont(m_arial);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
}

void VisualizerScreen::updateOffset()
{
	sf::FloatRect rect = m_sortingAlgorithmDisplay.getGlobalBounds();
	m_hello.setPosition(rect.left + rect.width + m_arial.getGlyph(' ', 20, false, 1.f).advance, m_sortingAlgorithmDisplay.getPosition().y);

	rect = m_hello.getGlobalBounds();
	m_numTaskDisplay.setPosition(rect.left + rect.width + m_arial.getGlyph(' ', 20, false, 1.f).advance, m_hello.getPosition().y);
}

void VisualizerScreen::resetStatDisplay()
{
	m_visualTimeDisplay.setString("Visual time : --:--");
	m_comparisonsDisplay.setString("Comparisons : 0");
	m_writesMainDisplay.setString("Writes to main array : 0");
	m_writesAuxDisplay.setString("Writes to auxiliary array : 0");
}

void VisualizerScreen::updateStatDisplay()
{
	sf::Int32 time = m_visualTimeClock.getElapsedTime().asMilliseconds() / 1000 - 1;
	if (time >= 60 * 10)
	{
		if ((time % 60) < 10)
		{
			m_visualTimeDisplay.setString("Visual time : " + std::to_string(time / 60) + ":0" + std::to_string(time % 60));
		}
		else
		{
			m_visualTimeDisplay.setString("Visual time : " + std::to_string(time / 60) + ":" + std::to_string(time % 60));
		}
	}
	else if (time >= 0)
	{
		if ((time % 60) < 10)
		{
			m_visualTimeDisplay.setString("Visual time : 0" + std::to_string(time / 60) + ":0" + std::to_string(time % 60));
		}
		else
		{
			m_visualTimeDisplay.setString("Visual time : 0" + std::to_string(time / 60) + ":" + std::to_string(time % 60));
		}
	}

	m_comparisonsDisplay.setString("Comparisons : " + std::to_string(m_visualizer.getStatistics().comparisons));
	m_writesMainDisplay.setString("Writes to main array : " + std::to_string(m_visualizer.getStatistics().writesMain));
	m_writesAuxDisplay.setString("Writes to auxiliary array : " + std::to_string(m_visualizer.getStatistics().writesAux));
}

void VisualizerScreen::processParametersEvent(const sf::Event& ev)
{
	if (isReadyToGo())
	{
		if (ev.key.code == sf::Keyboard::Key::Right)
		{
			if (!m_sortingAlgorithmList->empty())
			{
				m_sortIndex = (m_sortIndex + 1) % m_sortingAlgorithmList->size();
				m_sortingAlgorithmDisplay.setString((*m_sortingAlgorithmList)[m_sortIndex]->getAlgorithmName());

				updateOffset();

				m_numTaskDisplay.setString("");
			}
		}
		else if (ev.key.code == sf::Keyboard::Key::Left)
		{
			if (!m_sortingAlgorithmList->empty())
			{
				if (m_sortIndex == 0) m_sortIndex = m_sortingAlgorithmList->size() - 1;
				else m_sortIndex--;
				m_sortingAlgorithmDisplay.setString((*m_sortingAlgorithmList)[m_sortIndex]->getAlgorithmName());

				updateOffset();

				m_numTaskDisplay.setString("");
			}
		}
		else if (ev.key.code == sf::Keyboard::Key::Up)
		{
			if (m_arraySize < 1 << 15)
			{
				m_arraySize <<= 1;
				m_arraySizeDisplay.setString("Array size : " + std::to_string(m_arraySize));
			}
		}
		else if (ev.key.code == sf::Keyboard::Key::Down)
		{
			if (m_arraySize > 8)
			{
				m_arraySize >>= 1;
				m_arraySizeDisplay.setString("Array size : " + std::to_string(m_arraySize));
			}
		}
		else if (ev.key.code == sf::Keyboard::Key::A)
		{
			m_sortIndex = 0;
			if (m_sortingAlgorithmList == &m_sortingAlgorithms) m_sortingAlgorithmList = &m_additionalSortingAlgorithms;
			else m_sortingAlgorithmList = &m_sortingAlgorithms;

			if (m_sortingAlgorithmList->empty())
			{
				m_sortingAlgorithmDisplay.setString("No additional sort in the \"additionalSort/\" folder.");
			}
			else
			{
				m_sortingAlgorithmDisplay.setString((*m_sortingAlgorithmList)[m_sortIndex]->getAlgorithmName());
			}

			updateOffset();
		}
		else if (ev.key.code == sf::Keyboard::Key::Num1 || ev.key.code == sf::Keyboard::Key::Numpad1)
		{
			m_visualizer.showAuxiliaryArrayWhenUsed(!m_visualizer.isAuxiliaryArrayWillBeShown());
			m_auxArrayShowDisplay.setString("Show auxiliary array : " + std::string(m_visualizer.isAuxiliaryArrayWillBeShown() ? "on" : "off"));
		}
	}
}

void VisualizerScreen::processTaskFileEvent(const sf::Event& ev)
{
	if (isReadyToGo())
	{
		if (ev.key.code == sf::Keyboard::Key::Left)
		{
			if (!m_taskFiles.empty())
			{
				if (m_taskIndex == 0) m_taskIndex = m_taskFiles.size() - 1;
				else m_taskIndex--;

				m_fileSelectedDisplay.setString(m_taskFiles[m_taskIndex].string());
			}
		}
		else if (ev.key.code == sf::Keyboard::Key::Right)
		{
			if (!m_taskFiles.empty())
			{
				m_taskIndex = (m_taskIndex + 1) % m_taskFiles.size();

				m_fileSelectedDisplay.setString(m_taskFiles[m_taskIndex].string());
			}
		}
	}
}

void VisualizerScreen::startNextTask()
{
	if (m_testSortThread.joinable())
	{
		m_testSortThread.join();
	}

	SortingTask& task = m_taskQueue.front();
	m_visualizer.setArray(SampleArray::createIncrementalArray<unsigned int>(task.getArraySize()));
	m_visualizer.visualizerDelay(0.5f);

	m_testStep = TestStep::Shuffle;

	if (m_shuffleAlgorithm.isFuncValid()) m_testSortThread = std::thread(&VisualizerScreen::testSort, this, std::ref(task.getSortingAlgorithm()), std::ref(m_shuffleAlgorithm));
	else m_testSortThread = std::thread(&VisualizerScreen::testSort, this, std::ref(task.getSortingAlgorithm()), std::ref(*m_shuffleAlgorithms[0]));

	m_taskDelay = 0.f;
	m_arraySizeDisplay.setString("Array size : " + std::to_string(task.getArraySize()));
	m_sortingAlgorithmDisplay.setString(task.getSortingAlgorithm().getAlgorithmName());

	m_hello.setString(": Shuffling...");
	resetStatDisplay();

	m_numTaskDisplay.setString("(" + std::to_string(m_totalNumTask - m_taskQueue.size() + 1) + " of " + std::to_string(m_totalNumTask) + ")");

	updateOffset();
}

void VisualizerScreen::testSort(sv::SortingAlgorithm& sort, sv::ShuffleAlgorithm& shuffle)
{
	shuffle.shuffle(m_visualizer);

	{
		std::unique_lock<std::mutex> lock(m_condMutex);
		m_cond.wait(lock, [this]() { return TestStep::Shuffle != m_testStep; });
	}

	sort.sort(m_visualizer);

	{
		std::unique_lock<std::mutex> lock(m_condMutex);
		m_cond.wait(lock, [this]() { return TestStep::Sort != m_testStep; });
	}

	m_visualizer.setCompareDelay(1.f / m_visualizer.getArrayLength());
	m_sorted = m_visualizer.checkSortedVisualisation();
}

void VisualizerScreen::pushTask(const std::vector<sv::SortingAlgorithm*>& algorithms, const std::string& name, size_t arraySize)
{
	for (auto algorithm : algorithms)
	{
		if (algorithm->getAlgorithmName() == name)
		{
			m_taskQueue.emplace(*algorithm, arraySize);
			break;
		}
	}
}

bool VisualizerScreen::processFile(const std::vector<sv::SortingAlgorithm*>& algorithms, const std::string& luaFile)
{
	int initSize = lua_gettop(m_L);
	if (std::filesystem::exists(luaFile) && luaL_dofile(m_L, luaFile.c_str()) == LUA_OK)
	{
		if (!lua_istable(m_L, -1)) goto freeLua;
		lua_getfield(m_L, -1, "shuffle");
		lua_setglobal(m_L, "shuffle");

		lua_getfield(m_L, -1, "tasks");
		if (!lua_istable(m_L, -1)) goto freeLua;

		size_t lenght = lua_rawlen(m_L, -1);

		for (size_t i = 1; i <= lenght; i++)
		{
			lua_rawgeti(m_L, -1, i);
			lua_getfield(m_L, -1, "algorithmName");
			if (!lua_isstring(m_L, -1)) goto freeLua;

			std::string name = lua_tostring(m_L, -1);

			lua_getfield(m_L, -2, "arraySize");
			if (!lua_isinteger(m_L, -1)) goto freeLua;

			size_t arraySize = lua_tointeger(m_L, -1);

			pushTask(algorithms, name, arraySize);

			lua_pop(m_L, 3);
		}

		lua_settop(m_L, initSize);
		return true;
	}

freeLua:

	lua_settop(m_L, initSize);
	return false;
}

bool VisualizerScreen::isReadyToGo() const
{
	return m_testStep == TestStep::End && m_taskQueue.empty();
}

VisualizerScreen::SortingTask::SortingTask(sv::SortingAlgorithm& algorithm, size_t arraySize) : m_algorithm(&algorithm), m_arraySize(arraySize)
{
}

sv::SortingAlgorithm& VisualizerScreen::SortingTask::getSortingAlgorithm() const
{
	return *m_algorithm;
}

size_t VisualizerScreen::SortingTask::getArraySize() const
{
	return m_arraySize;
}
