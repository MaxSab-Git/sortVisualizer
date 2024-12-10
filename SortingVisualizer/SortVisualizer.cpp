#include "SortVisualizer.h"
#include <chrono>
#include <random>
#include <iostream>

namespace sv
{
	SortVisualizer::SortVisualizer() : 
		m_array(nullptr),
		m_arrayLenght(0),
		m_writeDelay(0.01f),
		m_compareDelay(0.01f),
		m_swapDelay(0.01f),
		m_running(false),
		m_queueLimit(0),
		m_elapsedTime(0.f),
		m_abort(false),
		m_auxArray(nullptr),
		m_auxArrayLenght(0),
		m_showAxiliary(false),
		m_dirtyHighlight(false)
	{
		m_barGraph.setPlaySound(true);
	}

	SortVisualizer::~SortVisualizer()
	{
		delete[] m_array;
		m_array = nullptr;

		delete[] m_auxArray;
		m_auxArray = nullptr;
	}

	void SortVisualizer::setVisualizerSize(const sf::Vector2f& size)
	{
		m_barGraph.setMaximumGraphSize(size);
		m_auxBarGraph.setMaximumGraphSize(size);
	}

	void SortVisualizer::showAuxiliaryArrayWhenUsed(bool show)
	{
		m_showAxiliary = show;
	}

	bool SortVisualizer::isAuxiliaryArrayWillBeShown() const
	{
		return m_showAxiliary;
	}

	void SortVisualizer::setArray(const unsigned int* data, size_t len)
	{
		if (!isDone()) return;

		std::lock_guard<std::mutex> lock(m_mutex);

		delete[] m_array;
		m_array = nullptr;
		m_arrayLenght = 0;

		freeAuxiliaryArray();

		m_abort = false;

		if (len > 0 && data)
		{
			m_array = new unsigned int[len];
			m_arrayLenght = len;
			unsigned int max = 0;
			for (size_t i = 0; i < len; i++)
			{
				max = std::max(max, data[i]);
				m_array[i] = data[i];
			}

			m_barGraph.resize(len, max);
			for (size_t i = 0; i < len; i++)
			{
				m_barGraph.set(i, m_array[i]);
			}
		}
	}

	void SortVisualizer::setArray(const std::vector<unsigned int>& data)
	{
		setArray(data.data(), data.size());
	}

	void SortVisualizer::setWriteDelay(float delay)
	{
		m_writeDelay = delay;
	}

	void SortVisualizer::setSwapDelay(float delay)
	{
		m_swapDelay = delay;
	}

	void SortVisualizer::setCompareDelay(float delay)
	{
		m_compareDelay = delay;
	}

	void SortVisualizer::createAuxiliaryArray(size_t len)
	{
		if (m_auxArray) return;

		freeAuxiliaryArray();

		if (len > 0)
		{
			m_auxArray = new unsigned int[len];
			m_auxArrayLenght = len;

			m_auxBarGraph.resize(len, m_barGraph.getMaxValue());
			for (size_t i = 0; i < len; i++)
			{
				m_auxArray[i] = 0;
				m_auxBarGraph.set(i, 0);
			}
		}
	}

	void SortVisualizer::freeAuxiliaryArray()
	{
		delete[] m_auxArray;
		m_auxArray = nullptr;
		m_auxArrayLenght = 0;
	}

	void SortVisualizer::setQueueLimit(size_t limit)
	{
		m_queueLimit = limit;
	}

	size_t sv::SortVisualizer::getArrayLength() const
	{
		return m_arrayLenght;
	}

	size_t SortVisualizer::getAuxArrayLenght() const
	{
		return m_auxArrayLenght;
	}

	unsigned int SortVisualizer::getValue(size_t index, VisualOperation::ArrayAccess access) const
	{
		switch (access)
		{
		case sv::VisualOperation::ArrayAccess::Main:
			if (index < m_arrayLenght) return m_array[index];
			break;
		case sv::VisualOperation::ArrayAccess::Aux:
			if (index < m_auxArrayLenght) return m_auxArray[index];
			break;
		default:
			break;
		}
		return 0;
	}

	bool SortVisualizer::visualizerWrite(size_t index, unsigned int val, VisualOperation::ArrayAccess access)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		if (m_abort) return false;

		switch (access)
		{
		case sv::VisualOperation::ArrayAccess::Main:
			if (index < m_arrayLenght && val <= m_barGraph.getMaxValue())
			{
				m_array[index] = val;
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::Aux:
			if (index < m_auxArrayLenght && val <= m_auxBarGraph.getMaxValue())
			{
				m_auxArray[index] = val;
				goto insertOperation;
			}
			break;
		default:
			return false;
		}

		return false;

	insertOperation:

		VisualOperation op;
		op.arg1 = index;
		op.writeArg = val;
		op.type = VisualOperation::Type::Write;
		op.delay = m_writeDelay;
		op.access = access;
		m_op.push(op);
		return true;
	}

	bool SortVisualizer::visualizerCopy(size_t dest, size_t src, VisualOperation::ArrayAccess access)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		if (m_abort) return false;

		unsigned int writeArg;

		switch (access)
		{
		case sv::VisualOperation::ArrayAccess::Main:
			if (dest < m_arrayLenght && src < m_arrayLenght)
			{
				writeArg = m_array[src];
				m_array[dest] = writeArg;
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::MainAux:
			if (dest < m_arrayLenght && src < m_auxArrayLenght)
			{
				writeArg = m_auxArray[src];
				m_array[dest] = writeArg;
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::AuxMain:
			if (dest < m_auxArrayLenght && src < m_arrayLenght)
			{
				writeArg = m_array[src];
				m_auxArray[dest] = writeArg;
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::Aux:
			if (dest < m_auxArrayLenght && src < m_auxArrayLenght)
			{
				writeArg = m_auxArray[src];
				m_auxArray[dest] = writeArg;
				goto insertOperation;
			}
			break;
		default:
			return false;
		}

		return false;

	insertOperation:

		VisualOperation op;
		op.arg1 = dest;
		op.arg2 = src;
		op.writeArg = writeArg;
		op.type = VisualOperation::Type::Copy;
		op.delay = m_writeDelay;
		op.access = access;
		m_op.push(op);

		return true;
	}

	bool SortVisualizer::visualizerSwap(size_t ileft, size_t iright, VisualOperation::ArrayAccess access)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		if (m_abort) return false;

		switch (access)
		{
		case sv::VisualOperation::ArrayAccess::Main:
			if (ileft < m_arrayLenght && iright < m_arrayLenght)
			{
				std::swap(m_array[ileft], m_array[iright]);
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::MainAux:
			if (ileft < m_arrayLenght && iright < m_auxArrayLenght)
			{
				std::swap(m_array[ileft], m_auxArray[iright]);
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::AuxMain:
			if (ileft < m_auxArrayLenght && iright < m_arrayLenght)
			{
				std::swap(m_auxArray[ileft], m_array[iright]);
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::Aux:
			if (ileft < m_auxArrayLenght && iright < m_auxArrayLenght)
			{
				std::swap(m_auxArray[ileft], m_auxArray[iright]);
				goto insertOperation;
			}
			break;
		default:
			return false;
		}

		return false;

	insertOperation:

		VisualOperation op;
		op.arg1 = ileft;
		op.arg2 = iright;
		op.type = VisualOperation::Type::Swap;
		op.delay = m_swapDelay;
		op.access = access;
		m_op.push(op);

		return true;
	}

	bool SortVisualizer::visualizerCompare(size_t ileft, size_t iright, bool& result, bool (*pred)(unsigned int, unsigned int), VisualOperation::ArrayAccess access)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		result = false;
		if (m_abort) return false;

		switch (access)
		{
		case sv::VisualOperation::ArrayAccess::Main:
			if (ileft < m_arrayLenght && iright < m_arrayLenght && pred)
			{
				result = pred(m_array[ileft], m_array[iright]);
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::MainAux:
			if (ileft < m_arrayLenght && iright < m_auxArrayLenght && pred)
			{
				result = pred(m_array[ileft], m_auxArray[iright]);
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::AuxMain:
			if (ileft < m_auxArrayLenght && iright < m_arrayLenght && pred)
			{
				result = pred(m_auxArray[ileft], m_array[iright]);
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::Aux:
			if (ileft < m_auxArrayLenght && iright < m_auxArrayLenght && pred)
			{
				result = pred(m_auxArray[ileft], m_auxArray[iright]);
				goto insertOperation;
			}
			break;
		default:
			return false;
		}

		return false;

	insertOperation:

		VisualOperation op;
		op.arg1 = ileft;
		op.arg2 = iright;
		op.type = VisualOperation::Type::Compare;
		op.delay = m_compareDelay;
		op.access = access;
		m_op.push(op);

		return true;
	}

	bool SortVisualizer::visualizerExternCompare(size_t ileft, unsigned int right, bool& result, bool(*pred)(unsigned int, unsigned int), VisualOperation::ArrayAccess access)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		result = false;
		if (m_abort) return false;

		switch (access)
		{
		case sv::VisualOperation::ArrayAccess::Main:
			if (ileft < m_arrayLenght && pred)
			{
				result = pred(m_array[ileft], right);
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::Aux:
			if (ileft < m_auxArrayLenght && pred)
			{
				result = pred(m_auxArray[ileft], right);
				goto insertOperation;
			}
			break;
		default:
			return false;
		}

		return false;

	insertOperation:

		VisualOperation op;
		op.arg1 = ileft;
		op.type = VisualOperation::Type::ExternCompare;
		op.delay = m_compareDelay;
		op.access = access;
		m_op.push(op);

		return true;
	}

	bool SortVisualizer::visualizerBitMask(size_t ileft, unsigned int mask, unsigned int& result, VisualOperation::ArrayAccess access)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		result = 0;
		if (m_abort) return false;

		switch (access)
		{
		case sv::VisualOperation::ArrayAccess::Main:
			if (ileft < m_arrayLenght)
			{
				result = m_array[ileft] & mask;
				goto insertOperation;
			}
			break;
		case sv::VisualOperation::ArrayAccess::Aux:
			if (ileft < m_auxArrayLenght)
			{
				result = m_auxArray[ileft] & mask;
				goto insertOperation;
			}
			break;
		default:
			return false;
		}

		return false;

	insertOperation:

		VisualOperation op;
		op.arg1 = ileft;
		op.type = VisualOperation::Type::BitMask;
		op.delay = m_compareDelay;
		op.access = access;
		m_op.push(op);

		return true;
	}

	bool SortVisualizer::visualizerReverse(size_t start, size_t end, VisualOperation::ArrayAccess access)
	{
		if ((access != VisualOperation::ArrayAccess::Main && access != VisualOperation::ArrayAccess::Aux) || isAborted()) return false;

		end--;
		while (start < end)
		{
			if (!visualizerSwap(start, end, access)) return false;
			start++;
			end--;
		}

		return true;
	}

	void SortVisualizer::visualizerDelay(float delay)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		if (m_abort) return;

		VisualOperation op;
		op.type = VisualOperation::Type::Delay;
		op.delay = delay;
		m_op.push(op);
	}

	void SortVisualizer::visualizerDone()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() { return (m_queueLimit == 0 || m_op.size() < m_queueLimit) || m_abort; });

		if (m_abort) return;

		VisualOperation op;
		op.type = VisualOperation::Type::Done;
		op.delay = 0.f;
		m_op.push(op);

		freeAuxiliaryArray();
	}

	bool SortVisualizer::executeOperation(const VisualOperation& op)
	{
		m_running = true;
		m_dirtyHighlight = true;

		BarGraph* barGraphArg1 = nullptr;
		BarGraph* barGraphArg2 = nullptr;
		unsigned long long* writeStat1 = nullptr;
		unsigned long long* writeStat2 = nullptr;

		switch (op.access)
		{
		case VisualOperation::ArrayAccess::Main:
			barGraphArg1 = &m_barGraph;
			barGraphArg2 = &m_barGraph;
			writeStat1 = &m_stats.writesMain;
			writeStat2 = &m_stats.writesMain;
			break;
		case VisualOperation::ArrayAccess::MainAux:
			barGraphArg1 = &m_barGraph;
			barGraphArg2 = &m_auxBarGraph;
			writeStat1 = &m_stats.writesMain;
			writeStat2 = &m_stats.writesAux;
			break;
		case VisualOperation::ArrayAccess::AuxMain:
			barGraphArg1 = &m_auxBarGraph;
			barGraphArg2 = &m_barGraph;
			writeStat1 = &m_stats.writesAux;
			writeStat2 = &m_stats.writesMain;
			break;
		case VisualOperation::ArrayAccess::Aux:
			barGraphArg1 = &m_auxBarGraph;
			barGraphArg2 = &m_auxBarGraph;
			writeStat1 = &m_stats.writesAux;
			writeStat2 = &m_stats.writesAux;
			break;
		default:
			return false;
		}

		if (op.type == VisualOperation::Type::Copy || op.type == VisualOperation::Type::Write)
		{
			barGraphArg1->set(op.arg1, op.writeArg);
			*writeStat1 += 1;
		}
		else if (op.type == VisualOperation::Type::Swap)
		{
			barGraphArg1->swap(op.arg1, *barGraphArg2, op.arg2);
			*writeStat1 += 1;
			*writeStat2 += 1;
		}
		else if (op.type == VisualOperation::Type::Compare || op.type == VisualOperation::Type::ExternCompare)
		{
			m_stats.comparisons += 1;
		}
		else if (op.type == VisualOperation::Type::CheckSorted && op.access == VisualOperation::ArrayAccess::Main)
		{
			highlight(op);
		}
		else if (op.type == VisualOperation::Type::Done)
		{
			m_running = false;
			m_elapsedTime = 0.f;
			m_auxBarGraph.resize(0, 0);
			m_barGraph.reset();
			m_lastHighlight = VisualOperation();
		}

		return true;
	}

	void SortVisualizer::highlight(const VisualOperation& op)
	{
		BarGraph* barGraphArg1 = nullptr;
		BarGraph* barGraphArg2 = nullptr;

		switch (op.access)
		{
		case VisualOperation::ArrayAccess::Main:
			barGraphArg1 = &m_barGraph;
			barGraphArg2 = &m_barGraph;
			break;
		case VisualOperation::ArrayAccess::MainAux:
			barGraphArg1 = &m_barGraph;
			barGraphArg2 = &m_auxBarGraph;
			break;
		case VisualOperation::ArrayAccess::AuxMain:
			barGraphArg1 = &m_auxBarGraph;
			barGraphArg2 = &m_barGraph;
			break;
		case VisualOperation::ArrayAccess::Aux:
			barGraphArg1 = &m_auxBarGraph;
			barGraphArg2 = &m_auxBarGraph;
			break;
		}

		if (op.type == VisualOperation::Type::Write || op.type == VisualOperation::Type::ExternCompare || op.type == VisualOperation::Type::BitMask)
		{
			barGraphArg1->highlight(op.arg1, true);
			barGraphArg1->playSound(op.arg1);
		}
		else if (op.type == VisualOperation::Type::Swap || op.type == VisualOperation::Type::Compare || op.type == VisualOperation::Type::Copy)
		{
			barGraphArg1->highlight(op.arg1, true);
			barGraphArg2->highlight(op.arg2, true);

			if (barGraphArg1 == &m_auxBarGraph) barGraphArg2->playSound(op.arg2);
			else barGraphArg1->playSound(op.arg1);
		}
		else if (op.type == VisualOperation::Type::CheckSorted && op.access == VisualOperation::ArrayAccess::Main)
		{
			barGraphArg1->highlight(op.arg1, sf::Color::Green);
			barGraphArg2->highlight(op.arg2, sf::Color::Green);

			barGraphArg1->playSound(op.arg2);
		}
	}

	void SortVisualizer::undoHighlight(const VisualOperation& op)
	{
		BarGraph* barGraphArg1 = nullptr;
		BarGraph* barGraphArg2 = nullptr;

		switch (op.access)
		{
		case VisualOperation::ArrayAccess::Main:
			barGraphArg1 = &m_barGraph;
			barGraphArg2 = &m_barGraph;
			break;
		case VisualOperation::ArrayAccess::MainAux:
			barGraphArg1 = &m_barGraph;
			barGraphArg2 = &m_auxBarGraph;
			break;
		case VisualOperation::ArrayAccess::AuxMain:
			barGraphArg1 = &m_auxBarGraph;
			barGraphArg2 = &m_barGraph;
			break;
		case VisualOperation::ArrayAccess::Aux:
			barGraphArg1 = &m_auxBarGraph;
			barGraphArg2 = &m_auxBarGraph;
			break;
		default:
			return;
		}

		if (op.type == VisualOperation::Type::Write || op.type == VisualOperation::Type::ExternCompare || op.type == VisualOperation::Type::BitMask)
		{
			barGraphArg1->highlight(op.arg1, false);
		}
		else if (op.type == VisualOperation::Type::Swap || op.type == VisualOperation::Type::Compare || op.type == VisualOperation::Type::Copy)
		{
			barGraphArg1->highlight(op.arg1, false);
			barGraphArg2->highlight(op.arg2, false);
		}
	}

	void SortVisualizer::swapQueue()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		std::swap(m_executeOp, m_op);
	}

	void SortVisualizer::step()
	{
		std::lock_guard<std::mutex> lock(m_updateMutex);

		if (m_executeOp.empty() && !m_op.empty())
		{
			swapQueue();
			m_cond.notify_all();

			VisualOperation& op = m_executeOp.front();
			executeOperation(op);

			if (m_dirtyHighlight)
			{
				undoHighlight(m_lastHighlight);
				highlight(op);
				m_lastHighlight = op;
				m_dirtyHighlight = false;
			}
		}
		else if (m_running)
		{
			if (!m_executeOp.empty())
			{
				m_executeOp.pop();

				if (!m_executeOp.empty())
				{
					VisualOperation& op = m_executeOp.front();
					executeOperation(op);

					if (m_dirtyHighlight)
					{
						undoHighlight(m_lastHighlight);
						highlight(op);
						m_lastHighlight = op;
						m_dirtyHighlight = false;
					}
				}
			}

			m_elapsedTime = 0.f;
		}
	}

	void SortVisualizer::update(float deltaTime)
	{
		{
			std::lock_guard<std::mutex> lock(m_updateMutex);

			if (m_abort) return;

			if (m_executeOp.empty() && !m_op.empty())
			{
				swapQueue();

				m_cond.notify_all();

				executeOperation(m_executeOp.front());

				if (m_executeOp.front().type == VisualOperation::Type::Done)
				{
					m_executeOp.pop();
					return;
				}

				if (m_dirtyHighlight)
				{
					undoHighlight(m_lastHighlight);
					highlight(m_executeOp.front());
					m_lastHighlight = m_executeOp.front();
					m_dirtyHighlight = false;
				}
			}

			if (!m_running) return;

			m_elapsedTime += deltaTime;

			while (!m_executeOp.empty())
			{
				VisualOperation& op = m_executeOp.front();
				op.delay -= m_elapsedTime;
				if (op.delay <= 0.f)
				{
					m_elapsedTime = abs(op.delay);

					if (m_executeOp.size() > 1)
					{
						m_executeOp.pop();

						executeOperation(m_executeOp.front());
					}
					else if (m_dirtyHighlight)
					{
						undoHighlight(m_lastHighlight);
						highlight(op);
						m_lastHighlight = op;
						m_dirtyHighlight = false;

						m_executeOp.pop();
					}
					else
					{
						m_executeOp.pop();
					}
				}
				else
				{
					m_elapsedTime = 0.f;
					break;
				}
			}

			if (!m_executeOp.empty() && m_dirtyHighlight)
			{
				undoHighlight(m_lastHighlight);
				highlight(m_executeOp.front());
				m_lastHighlight = m_executeOp.front();
				m_dirtyHighlight = false;
			}
		}

		m_cond.notify_all();
	}

	bool SortVisualizer::isDone() const
	{
		return !m_running;
	}

	void SortVisualizer::abort()
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			std::lock_guard<std::mutex> lock2(m_updateMutex);

			m_abort = true;
			m_running = false;
			m_elapsedTime = 0.f;
			m_op = std::queue<VisualOperation>();
			m_executeOp = std::queue<VisualOperation>();
			m_lastHighlight = VisualOperation();
			m_barGraph.reset();
			m_auxBarGraph.reset();
			freeAuxiliaryArray();
		}
		m_cond.notify_all();
	}

	bool SortVisualizer::isAborted() const
	{
		return m_abort;
	}

	void SortVisualizer::resetStatistics()
	{
		m_stats.comparisons = 0;
		m_stats.writesMain = 0;
		m_stats.writesAux = 0;
	}

	const VisualizerStatistics& SortVisualizer::getStatistics() const
	{
		return m_stats;
	}

	void SortVisualizer::shuffleVisualisation()
	{
		if (m_arrayLenght == 0) return;

		std::default_random_engine eng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count() & 0xffffffff));
		std::uniform_int_distribution<size_t> random(0, static_cast<size_t>(m_arrayLenght - 1));

		for (size_t i = 0; i < m_arrayLenght; i++)
		{
			if (!visualizerSwap(i, random(eng))) return;
		} 

		visualizerDone();
	}

	bool SortVisualizer::checkSortedVisualisation()
	{
		if (m_arrayLenght == 0) return false;
		
		bool result = true;
		for (size_t i = 0; i < m_arrayLenght - 1; i++)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if (isAborted()) return false;

			if (m_array[i] <= m_array[i + 1])
			{

				VisualOperation op;
				op.type = VisualOperation::Type::CheckSorted;
				op.arg1 = i;
				op.arg2 = i;
				op.delay = m_compareDelay;
				op.access = VisualOperation::ArrayAccess::Main;
				m_op.push(op);
			}
			else
			{
				result = false;
				break;
			}
		}

		visualizerDone();
		return result;
	}

	void SortVisualizer::debugPrint() const
	{
		for (size_t i = 0; i < m_arrayLenght; i++)
		{
			std::cout << m_array[i] << ' ';
		}
		std::cout << std::endl;
	}

	bool SortVisualizer::less(unsigned int a, unsigned int b)
	{
		return a < b;
	}

	bool SortVisualizer::lessEquals(unsigned int a, unsigned int b)
	{
		return a <= b;
	}

	bool SortVisualizer::greater(unsigned int a, unsigned int b)
	{
		return a > b;
	}

	bool SortVisualizer::greaterEquals(unsigned int a, unsigned int b)
	{
		return a >= b;
	}

	bool SortVisualizer::equals(unsigned int a, unsigned int b)
	{
		return a == b;
	}

	bool SortVisualizer::notEquals(unsigned int a, unsigned int b)
	{
		return a != b;
	}

	void SortVisualizer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::lock_guard<std::mutex> lock(m_updateMutex);

		states.transform *= getTransform();

		if (m_showAxiliary && m_auxBarGraph.getMaxValue() > 0 && !m_abort)
		{
			sf::RenderStates mainStates = states;
			mainStates.transform.scale(1.f, 0.80f);

			target.draw(m_barGraph, mainStates);

			sf::RenderStates auxStates = states;
			auxStates.transform.translate(0.f, -m_auxBarGraph.getMaximumGraphSize().y * 0.85f).scale(1.f, 0.15f);

			target.draw(m_auxBarGraph, auxStates);
		}
		else
		{
			target.draw(m_barGraph, states);
		}
	}
}