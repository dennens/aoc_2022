#include <algorithm>
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Day11_Input.h"


namespace Day11
{
	enum class Operator
	{
		Multiply,
		Add
	};

	enum class OperationValue
	{
		OldValue,
		Number
	};

	// Some helper functions to split up a string with given delimiter chars
	struct String
	{
		static bool isDelimiter(const char in)
		{
			return false;
		}

		template <char delim>
		static bool isDelimiter(const char in)
		{
			return in == delim;
		}

		template <char delim, char... delims>
		static bool isDelimiter(const char in)
		{
			return in == delim || isDelimiter(delims...);
		}

		template <char... delims>
		static std::vector<std::string> split(const std::string& in)
		{
			std::vector<std::string> ret;
			int len = in.length();
			int lastCut = 0;
			for (int i = 0; i < len; ++i)
			{
				if (isDelimiter<delims...>(in[i]) && i > lastCut)
				{
					ret.push_back(in.substr(lastCut, i - lastCut));
					lastCut = i + 1;
				}
			}
			ret.push_back(in.substr(lastCut, len - lastCut));
			return ret;
		}
	};

	// Helper struct to perform a simple operation
	struct Operation
	{
		OperationValue lhs;
		uint64_t lhsValue;
		Operator op;
		OperationValue rhs;
		uint64_t rhsValue;

		void parse(const std::string& string)
		{
			// Get operands and operator, then convert them to their code representations
			std::vector<std::string> split = String::split<' ', ','>(string);

			lhs = OperationValue::OldValue;
			if (std::isdigit(split[0][0]))
			{
				lhs = OperationValue::Number;
				lhsValue = atoi(split[0].c_str());
			}

			op = split[1][0] == '*' ? Operator::Multiply : Operator::Add;

			rhs = OperationValue::OldValue;
			if (std::isdigit(split[2][0]))
			{
				rhs = OperationValue::Number;
				rhsValue = atoi(split[2].c_str());
			}
		}

		void execute(uint64_t& value)
		{
			uint64_t l = lhs == OperationValue::Number ? lhsValue : value;
			uint64_t r = rhs == OperationValue::Number ? rhsValue : value;
			value = op == Operator::Add ? l + r : l * r;
		}
	};

	struct Monkey
	{
		std::vector<uint64_t> items;
		Operation operation;
		int test = 1;
		int monkeyIfTrue = 0;
		int monkeyIfFalse = 0;

		int itemsInspected = 0;
	};

	void execute(const char* inputString, int numberOfRounds, int worryDivision)
	{
		std::stringstream inputStream;
		inputStream.str(inputString);

		std::string line;
		std::stringstream lineStream;
		std::vector<Monkey> monkeys;
		Monkey* currentMonkey = nullptr;

		while (std::getline(inputStream, line))
		{
			// Monkey x, skip
			monkeys.push_back(Monkey());
			currentMonkey = &monkeys[monkeys.size() - 1];

			// starting items
			std::getline(inputStream, line);
			lineStream.str(line.substr(line.find_last_of(":") + 2));
			lineStream.clear();
			for (int item; lineStream.good() && lineStream >> item;)
			{
				currentMonkey->items.push_back(item);
				if (lineStream.peek() == ',')
					lineStream.ignore();
			}

			// operation
			std::getline(inputStream, line);
			currentMonkey->operation.parse(line.substr(line.find_last_of('=') + 2));

			// test value
			std::getline(inputStream, line);
			lineStream.str(line.substr(line.find_last_of(' ')));
			lineStream.clear();
			lineStream >> currentMonkey->test;

			// truemonkey
			std::getline(inputStream, line);
			lineStream.str(line.substr(line.find_last_of(' ')));
			lineStream.clear();
			lineStream >> currentMonkey->monkeyIfTrue;
			// falsemonkey
			std::getline(inputStream, line);
			lineStream.str(line.substr(line.find_last_of(' ')));
			lineStream.clear();
			lineStream >> currentMonkey->monkeyIfFalse;

			// empty line
			std::getline(inputStream, line);
		}

		// Needed for part 2
		// Since the numbers get HUGE, we need some way to keep track of the values
		// without keeping track of the values. Since all we're doing with the actual values
		// is checking if they're divisible by a number, find the 'lowest' common denominator
		// by multiplying all test values together.
		// Then we can modulo the item values on this number to keep it reasonable but still accurate
		// enough for our purposes.
		int lcd = 1;
		for (auto& monkey : monkeys)
			lcd *= monkey.test;

		for (int round = 0; round < numberOfRounds; ++round)
		{
			for (auto& monkey : monkeys)
			{
				for (auto itemIter = monkey.items.begin(); itemIter != monkey.items.end(); ++itemIter)
				{
					uint64_t& item = *itemIter;
					// apply operation
					monkey.operation.execute(item);
					// Reduce worry
					item /= worryDivision;
					bool testTrue = (item % monkey.test) == 0;
					// Keep item value sane
					item %= lcd;
					// Yeet item to another monkey
					monkeys[testTrue ? monkey.monkeyIfTrue : monkey.monkeyIfFalse].items.push_back(item);
					++monkey.itemsInspected;
				}
				// All items are yoted, clear list
				monkey.items.clear();
			}
		}

		// Find 2 monkeys with most items inspected
		std::sort(monkeys.begin(), monkeys.end(), [](Monkey& m1, Monkey& m2) {return m1.itemsInspected > m2.itemsInspected; });
		std::cout << "Monkey business score: " << (unsigned long long)monkeys[0].itemsInspected * monkeys[1].itemsInspected << std::endl;
	}

	void part1(const char* input)
	{
		execute(input, 20, 3);
	}

	void part2(const char* input)
	{
		execute(input, 10000, 1);
	}

	int main()
	{
		part2(sampleInput);
		part2(input);
		return 0;
	}
}
