#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Day5_Input.h"

namespace Day5
{
	std::vector<std::vector<char>> parseStacks(std::stringstream& inputStream)
	{
		// Need to parse the stacks from the input.
		// Just go left to right, top to bottom, until we reach an empty line.
		// Since the input has a set format we can simply loop
		std::vector<std::vector<char>> stacks;

		std::string line;
		std::stringstream lineStream;
		while (std::getline(inputStream, line))
		{
			// First empty line means we're done with stacks
			if (line.empty())
				return stacks;

			int numStacks = (int)std::ceil(line.length() / 4.0f);	// 4 columns per stack (except for last column): '[X] '
			// Ensure stacks exist
			if (stacks.size() == 0)
				for (int i = 0; i < numStacks; ++i)
					stacks.push_back(std::vector<char>());

			// Loop through the line 4 characters at a time
			for (int i = 0; i < numStacks; ++i)
			{
				char crate = line[(i * 4) + 1];
				if (crate != ' '
					&& (crate < '1' || crate > '9'))	// ignore stack numbers
					stacks[i].insert(stacks[i].begin(), crate);
			}
		}

		return stacks;
	}

	void logStringStreamError(std::stringstream& str)
	{
		if (!str.good())
		{
			std::cout << "Stringstream broke :c ";
			if (str.eof())
				std::cout << "EOF";
			if (str.fail())
				std::cout << "FAIL";
			if (str.bad())
				std::cout << "BAD";
		}
	}

	void moveCrates(std::vector<char>& srcCrates, std::vector<char>& dstCrates, int num, bool multiPickup)
	{
		// move num crates from src to dst
		if (multiPickup)
		{
			// Move crates all at once
			int numCratesOnSrc = srcCrates.size();
			auto srcBeginIter = srcCrates.begin() + numCratesOnSrc - num;
			dstCrates.insert(dstCrates.end(), srcBeginIter, srcCrates.end());
			srcCrates.erase(srcBeginIter, srcCrates.end());
		}
		else
		{
			// Move crates one by one
			for (int i = 0; i < num; ++i)
			{
				if (srcCrates.size() > 0)
				{
					char crate = srcCrates.back();
					srcCrates.pop_back();
					dstCrates.push_back(crate);
				}
			}
		}
	}

	void execute(const char* inputString, bool multiPickup)
	{
		std::stringstream inputStream;
		inputStream.str(inputString);

		std::vector<std::vector<char>> stacks = parseStacks(inputStream);
	
		// stacks have been parsed, handle operations
		std::string line;
		std::stringstream lineStream;
		std::string moveStr, fromStr, toStr;

		int src, num, dst;
		while (std::getline(inputStream, line))
		{
			// Format: 'move 3 from 6 to 2'
			lineStream.str(line);
			lineStream.seekg(0);	// Need to reset seek position for some reason :/
			lineStream >> moveStr >> num >> fromStr >> src >> toStr >> dst;

			auto& srcStack = stacks[src - 1];
			auto& dstStack = stacks[dst - 1];

			moveCrates(srcStack, dstStack, num, multiPickup);
		}

		// Output our result
		std::cout << "Crate message: ";
		for (size_t i = 0; i < stacks.size(); ++i)
			if (stacks[i].size() > 0)
				std::cout << stacks[i].back();
		std::cout << std::endl;
	}

	void part1(const char* inputString)
	{
		execute(inputString, false);
	}

	void part2(const char* inputString)
	{
		execute(inputString, true);
	}

	int main()
	{
		part2(input);
		return 0;
	}
}