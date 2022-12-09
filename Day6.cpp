#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Day6_Input.h"

namespace Day6
{
	// Fancy way to count bits called Brian Kernighan’s Algorithm
	template<typename T>
	unsigned int countSetBits(T n)
	{
		unsigned int count = 0;
		while (n) {
			n &= (n - 1);
			count++;
		}
		return count;
	}

	void execute(const char* inputString, int numIdCharacters)
	{
		// Use a bitfield to count different characters in a string of any length
		uint64_t charBitField;
		// No need to start at 1 since we need at least n characters anyway
		const char* curChar = inputString + numIdCharacters - 1;
		while (*curChar)
		{
			charBitField = 0;
			// Set all character bits counting backwards (since order doesn't matter)
			for (uint64_t i = 0; i < numIdCharacters; ++i)
				charBitField |= 1ULL << (*(curChar - i) - 'A');
			if (countSetBits(charBitField) == numIdCharacters)
				break; // We've found the right index!
			++curChar;
		}
		// Add 1 due to the way the question is asked
		std::cout << "First index of signal: " << ((int)(curChar - inputString) + 1) << std::endl;
	}

	void part1(const char* inputString)
	{
		execute(inputString, 4);
	}
	
	void part2(const char* inputString)
	{
		execute(inputString, 14);
	}

	int main()
	{
		part2(input);
		return 0;
	}
}
