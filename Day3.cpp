#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Day3_Input.h"
#include <cassert>

namespace Day3
{
	// Sets bits in a bitfield depending on which letters occur within contents
	uint64_t toIntRepresentation(const std::string& contents)
	{
		uint64_t result = 0;
		for (char character : contents)
		{
			bool caps = character >= 'A' && character <= 'Z';
			int charIndex = caps ? character - 'A' + 27 : character - 'a' + 1;
			result |= (1ULL << charIndex);
		}

		return result;
	}

	int findFirstSetBit(uint64_t value)
	{
		for (int i = 0; i < 64; ++i)
			if ((value >> i) & 1)
			{
				return i;
				break;
			}

		assert(false);
		return -1;
	}

	struct Rucksack
	{
		std::vector<std::string> compartments;

		int doubledPriority;

		void findDoubledItem()
		{
			constexpr uint64_t doubledItemMask = 0b1111111111111111111111111111111111111111111111111111110; // trailing 0 since 'a' starts at prio 1. All character bits are set.
			uint64_t mask = 0;
			doubledPriority = 0;
			if (compartments.size() > 0)
			{
				mask = doubledItemMask;

				// Bitwise & the mask with all compartment int representations. This will remove more and more bits until only 1 is left (assuming the input is is correct)
				for (size_t i = 0; i < compartments.size(); ++i)
				{
					uint64_t compartmentInt = toIntRepresentation(compartments[i]);
					mask &= compartmentInt;
				}
				doubledPriority = findFirstSetBit(mask);
			}
		}
	};

	void part1()
	{
		std::stringstream inputStream;
		inputStream.str(input);

		int numCompartments = 2;
		int totalPrio = 0;
		std::string line;
		while (std::getline(inputStream, line))
		{
			// Setup rucksack with the proper strings in the right compartments
			Rucksack rucksack;
			int length = line.length();
			int compartmentLength = length / numCompartments;
			for (int i = 0; i < numCompartments; ++i)
				rucksack.compartments.push_back(line.substr(compartmentLength * i, compartmentLength));

			// With all compartments set, find doubled-up characters
			rucksack.findDoubledItem();
			totalPrio += rucksack.doubledPriority;
		}

		std::cout << "Total doubled prio: " << totalPrio << std::endl;
	}

	void part2()
	{
		std::stringstream inputStream;
		inputStream.str(input);

		int numElvesPerGroup = 3;
		int totalPrio = 0;

		std::string line;

		// Use compartments as elves, a rucksack as a group of elf rucksacks. The logic is otherwise the same, find the single overlapping character among the compartments.
		Rucksack currentRucksack;
		while (std::getline(inputStream, line))
		{
			currentRucksack.compartments.push_back(line);

			if (currentRucksack.compartments.size() == numElvesPerGroup)
			{
				currentRucksack.findDoubledItem();
				totalPrio += currentRucksack.doubledPriority;
				currentRucksack = Rucksack();
			}
		}

		std::cout << "Total overlapping prio: " << totalPrio << std::endl;
	}

	int main()
	{
		part2();
		return 0;
	}
}
