#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <mutex>
#include "BarGraph.h"
#include <list>

namespace sv
{
	struct VisualOperation
	{
		enum class Type
		{
			Done,
			Delay,
			Compare,
			ExternCompare,
			CheckSorted,
			BitMask,
			Write,
			Copy,
			Swap
		};

		enum class ArrayAccess
		{
			Main,
			MainAux,
			AuxMain,
			Aux
		};

		size_t arg1 = 0;
		size_t arg2 = 0;
		unsigned int writeArg = 0;
		Type type = Type::Delay;
		float delay = 0.1f;
		ArrayAccess access = ArrayAccess::Main;
	};

	struct VisualizerStatistics
	{
		unsigned long long comparisons = 0;
		unsigned long long writesMain = 0;
		unsigned long long writesAux = 0;
	};

	class SortVisualizer : public sf::Drawable, public sf::Transformable
	{
	public:
		SortVisualizer();
		virtual ~SortVisualizer();

		SortVisualizer(const SortVisualizer&) = delete;
		SortVisualizer& operator=(const SortVisualizer&) = delete;

		void setVisualizerSize(const sf::Vector2f& size);
		void showAuxiliaryArrayWhenUsed(bool show);
		bool isAuxiliaryArrayWillBeShown() const;

		void setArray(const unsigned int* data, size_t len);
		void setArray(const std::vector<unsigned int>& data);
		void setWriteDelay(float delay);
		void setSwapDelay(float delay);
		void setCompareDelay(float delay);

		void createAuxiliaryArray(size_t len);
		void freeAuxiliaryArray();

		void setQueueLimit(size_t limit);

		size_t getArrayLength() const;
		size_t getAuxArrayLenght() const;
		unsigned int getValue(size_t index, VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main) const;

		bool visualizerWrite(size_t index, unsigned int val, VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main);
		bool visualizerCopy(size_t dest, size_t src, VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main);
		bool visualizerSwap(size_t ileft, size_t iright, VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main);
		bool visualizerCompare(size_t ileft, size_t iright, bool& result, bool (*pred)(unsigned int, unsigned int), VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main);
		bool visualizerExternCompare(size_t ileft, unsigned int right, bool& result, bool (*pred)(unsigned int, unsigned int), VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main);
		bool visualizerBitMask(size_t ileft, unsigned int mask, unsigned int& result, VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main);
		bool visualizerReverse(size_t start, size_t end, VisualOperation::ArrayAccess access = VisualOperation::ArrayAccess::Main);
		void visualizerDelay(float delay);
		void visualizerDone();

		void step();
		void update(float deltaTime);
		bool isDone() const;
		void abort();
		bool isAborted() const;
		void resetStatistics();
		const VisualizerStatistics& getStatistics() const;

		void shuffleVisualisation();
		bool checkSortedVisualisation();

		void debugPrint() const;

		static bool less(unsigned int a, unsigned int b);
		static bool lessEquals(unsigned int a, unsigned int b);
		static bool greater(unsigned int a, unsigned int b);
		static bool greaterEquals(unsigned int a, unsigned int b);
		static bool equals(unsigned int a, unsigned int b);
		static bool notEquals(unsigned int a, unsigned int b);

	private:
		bool executeOperation(const VisualOperation& op);
		void highlight(const VisualOperation& op);
		void undoHighlight(const VisualOperation& op);

		void swapQueue();

		BarGraph m_barGraph;
		BarGraph m_auxBarGraph;
		std::queue<VisualOperation> m_op;
		std::queue<VisualOperation> m_executeOp;
		VisualizerStatistics m_stats;

		VisualOperation m_lastHighlight;
		bool m_dirtyHighlight;

		unsigned int* m_array;
		size_t m_arrayLenght;
		unsigned int* m_auxArray;
		size_t m_auxArrayLenght;
		bool m_showAxiliary;
		float m_elapsedTime;

		float m_writeDelay;
		float m_swapDelay;
		float m_compareDelay;

		mutable std::mutex m_mutex;
		mutable std::mutex m_updateMutex;
		bool m_running;
		size_t m_queueLimit;
		std::condition_variable m_cond;
		bool m_abort;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}

