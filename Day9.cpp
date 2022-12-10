#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>

#include "Day9_Input.h"

namespace Day9
{
	constexpr int sign(int v)
	{
		return (v > 0) - (v < 0);
	}

	struct Position
	{
		int x = 0;
		int y = 0;

		bool operator<(const Position& other) const
		{
			// Abuse tuple operator< so I don't have to think of one myself
			return std::tuple<int, int>{ x, y } < std::tuple<int, int>{other.x, other.y};
		}
	};

	void visualizeGrid(const std::vector<Position>& knotPositions)
	{
		Position min;
		Position max;
		for (int i = 0; i < (int)knotPositions.size(); ++i)
		{
			min.x = std::min(min.x, knotPositions[i].x);
			min.y = std::min(min.y, knotPositions[i].y);
			max.x = std::max(max.x, knotPositions[i].x);
			max.y = std::max(max.y, knotPositions[i].y);
		}

		std::cout << "Visualization" << std::endl;
		for (int y = max.y; y >= min.y; --y)
		{
			for (int x = min.x; x <= max.x; ++x)
			{
				bool anyKnot = false;
				for (int k = 0; k < (int)knotPositions.size(); ++k)
				{
					if (knotPositions[k].x == x && knotPositions[k].y == y)
					{
						anyKnot = true;
						char out = (k == 0 ? 'H' : k == knotPositions.size() - 1 ? 'T' : '0' + k);
						std::cout << out;
						break;
					}
				}
				if (!anyKnot)
					std::cout << '.';
			}
			std::cout << std::endl;
		}
	}

	void execute(const char* inputString, int numKnots)
	{
		std::stringstream inputStream;
		inputStream.str(inputString);

		std::string line;
		std::stringstream lineStream;

		// Keep track of tail positions
		std::set<Position> tailPositions = { { 0, 0 } };
		std::vector<Position> knotPositions;
		for (int i = 0; i < numKnots; ++i)
			knotPositions.push_back({ 0, 0 });

		// convenience references
		Position& head = knotPositions[0];
		Position& tail = knotPositions[numKnots - 1];

		while (std::getline(inputStream, line))
		{
			lineStream.str(line);
			lineStream.seekg(0);

			char dir;
			int count;
			lineStream >> dir >> count;
			for (int i = 0; i < count; ++i)
			{
				// Move head
				if		(dir == 'U') ++head.y;
				else if (dir == 'D') --head.y;
				else if (dir == 'L') --head.x;
				else if (dir == 'R') ++head.x;

				// Play catch-up with all knots
				for (int k = 1; k < numKnots; ++k)
				{
					const Position& next = knotPositions[k - 1];
					Position& self = knotPositions[k];
					int yDiff = next.y - self.y;
					int xDiff = next.x - self.x;

					// Move if any of the axes is too far away
					if (std::abs(yDiff) > 1
						|| std::abs(xDiff) > 1)
					{
						// If either of the axes is too far away, move both:
						// The primary axis to catch up normally,
						// and the secondary to catch up diagonally.
						// If 'next' is not diagonally, sign for secondary axis returns 0
						// and secondary doesn't move
						self.y += sign(yDiff);
						self.x += sign(xDiff);
					}
				}
				tailPositions.insert(tail);
				
				/* For debugging
				visualizeGrid(knotPositions);
				std::cin.ignore();*/
			}
		}

		std::cout << "Number of tail positions: " << tailPositions.size() << std::endl;
	}

	void part1()
	{
		execute(sampleInput, 2);
		execute(sampleInput2, 2);
		execute(input, 2);
	}

	void part2()
	{
		int numKnots = 10;
		execute(sampleInput, numKnots);
		execute(sampleInput2, numKnots);
		execute(input, numKnots);
	}

	int main()
	{
		part1();
		return 0;
	}
}
