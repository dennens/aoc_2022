#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Day4_Input.h"

namespace Day4
{
	struct Range
	{
		int min;
		int max;

		void parse(std::string& string)
		{
			std::stringstream stream(string);
			stream >> min;
			stream >> max;
			max *= -1;	// undo the '-' used to indicate range
		}

		bool contains(const Range& other) const
		{
			return min <= other.min && max >= other.max;
		}

		bool contains(int i) const
		{
			return i >= min && i <= max;
		}

		bool overlaps(const Range& other)
		{
			return contains(other.min)		// Other min is within our range
				|| contains(other.max)		// Other max is within our range
				|| contains(other)			// Other is entirely within our range
				|| other.contains(*this);	// We are entirely within other's range
		}
	};

	void parseRangePair(const std::string& line, Range& r1, Range& r2)
	{
		int split = line.find_first_of(",");
		std::string r1string = line.substr(0, split);
		std::string r2string = line.substr(split + 1);
		r1.parse(r1string);
		r2.parse(r2string);
	}

	enum class Count
	{
		FullContain,
		Overlaps
	};

	void execute(Count countWhat)
	{
		std::stringstream inputStream;
		inputStream.str(input);

		int numCountedPairs = 0;
		std::string line;
		std::stringstream lineStream;
		while (std::getline(inputStream, line))
		{
			Range r1, r2;
			parseRangePair(line, r1, r2);

			numCountedPairs += countWhat == Count::Overlaps ? r1.overlaps(r2) : (r1.contains(r2) || r2.contains(r1));
		}
		std::cout << "Counted pairs: " << numCountedPairs << std::endl;
	}

	void part1()
	{
		execute(Count::FullContain);
	}

	void part2()
	{
		execute(Count::Overlaps);
	}

	int main()
	{
		part2();
		return 0;
	}
}
