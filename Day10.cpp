#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>

#include "Day10_Input.h"


namespace Day10
{
	void execute(const char* inputString, std::function<void(int, int)> onCycleIncrease)
	{
		std::stringstream inputStream;
		inputStream.str(inputString);

		std::string line;
		std::stringstream lineStream;

		int currentCycle = 0;
		int signalValue = 1;
		while (std::getline(inputStream, line))
		{
			lineStream.str(line);
			lineStream.seekg(0);

			std::string command;
			lineStream >> command;
			
			// All commands tick up the cycle
			++currentCycle;
			onCycleIncrease(currentCycle, signalValue);

			if (command[0] == 'a')
			{
				// addx command
				int value;
				lineStream >> value;
				// tick cycle again first, then adjust value
				++currentCycle;
				onCycleIncrease(currentCycle, signalValue);

				signalValue += value;
			}
		}
	}

	void part1(const char* inputString)
	{
		std::vector<int> requiredCycles = { 20, 60, 100, 140, 180, 220 };
		int nextRequiredCycleIndex = 0;
		int nextRequiredCycle = 0;
		int signalTotal = 0;

		execute(inputString, [&](int currentCycle, int signalValue) {

			if (nextRequiredCycleIndex < (int)requiredCycles.size())
				nextRequiredCycle = requiredCycles[nextRequiredCycleIndex];

			if (currentCycle == nextRequiredCycle)
			{
				std::cout << "Signal value at cycle " << currentCycle << ": " << nextRequiredCycle * signalValue << std::endl;
				signalTotal += nextRequiredCycle * signalValue;
				++nextRequiredCycleIndex;
			}
		});
		std::cout << "Signal total: " << signalTotal << std::endl;
	}

	void drawCrt(int cycle, int signal)
	{
		// renderPos - 1 because the first call (with cycle 1) will render pixel 0, and pixel is what matters here
		int renderPos = (cycle % 40) - 1;
		bool visible = std::abs(renderPos - signal) <= 1;
		std::cout << (visible ? '#' : '.');

		// newline
		if (renderPos == -1)
			std::cout << std::endl;
	}

	void part2(const char* inputString)
	{
		execute(inputString, drawCrt);
	}

	int main()
	{
		part2(sampleInput);
		part2(input);
		return 0;
	}
}
