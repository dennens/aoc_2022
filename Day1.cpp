#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Day1_Input.h"


namespace Day1
{
	std::vector<int> parseInput(std::stringstream& inputStream)
	{
		std::vector<int> elves = { 0 };

		std::string line;
		std::stringstream lineStream;
		while (std::getline(inputStream, line))
		{
			if (line.length() == 0)
			{
				// newline
				elves.push_back(0);
				continue;
			}
			lineStream.str(line);
			lineStream.seekg(0);

			int food;
			lineStream >> food;
			elves[elves.size() - 1] += food;
		}

		return elves;
	}

	int getTopNTotalFood(std::vector<int>& elves, int n)
	{
		std::sort(elves.begin(), elves.end(), [](int a, int b) { return a > b; });
		int total = 0;
		for (int i = 0; i < n; ++i)
			total += elves[i];
		return total;
	}

	void execute(int topNElves)
	{
		std::stringstream inputStream;
		inputStream.str(input);

		std::vector<int> elves = parseInput(inputStream);

		std::cout << "Top " << topNElves << " total food: " << getTopNTotalFood(elves, topNElves) << std::endl;
	}

	void part1()
	{
		execute(1);
	}

	void part2()
	{
		execute(3);
	}

	int main()
	{
		part2();
		return 0;
	}
}
