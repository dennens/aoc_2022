#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//#include "Day1_Input.h"


namespace Day0
{
	void part1(const char* inputString)
	{
		std::stringstream inputStream;
		inputStream.str(inputString);

		std::string line;
		std::stringstream lineStream;

		while (std::getline(inputStream, line))
		{
			lineStream.str(line);
			lineStream.seekg(0);
		}
	}

	int main()
	{
		return 0;
	}
}
