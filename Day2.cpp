#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Day2_Input.h"

namespace Day2
{
	int calculateScore(int selfChoice, int opponentChoice)
	{
		// 0: Rock
		// 1: Paper
		// 2: Scissors
		int resultScore = 
			selfChoice == (opponentChoice + 1) % 3 ? 6		// Win (be 1 higher, cycled)
			: selfChoice == opponentChoice ? 3				// Draw
			: 0;											// Lose
		return selfChoice + resultScore + 1;
	}

	enum class InputDefinition
	{
		OwnChoice,
		GameResult
	};

	void execute(InputDefinition definition)
	{
		std::stringstream inputStream;
		inputStream.str(input);

		int totalScore = 0;

		std::string line;
		std::stringstream lineStream;
		while (std::getline(inputStream, line))
		{
			lineStream.str(line);

			char opponentChoiceInput, column2;
			lineStream >> opponentChoiceInput >> column2;

			// Used in case of GameResult definition
			bool win = column2 == 'Z', tie = column2 == 'Y';

			int opponentChoice = (int)(opponentChoiceInput - 'A');
			int selfChoice;
			if (definition == InputDefinition::OwnChoice)
				selfChoice = (int)(column2 - 'X');
			else
				selfChoice =
				win ? (opponentChoice + 1) % 3 :	// Win: 1 higher than opponent, cycled
				tie ? opponentChoice :				// Tie: Equal to opponent
				(opponentChoice + 2) % 3;			// Lose: 1 lower (or 2 higher) than opponent, cycled

			totalScore += calculateScore(selfChoice, opponentChoice);
		}

		std::cout << "Total score: " << totalScore << std::endl;
	}

	void part1()
	{
		execute(InputDefinition::OwnChoice);
	}

	void part2()
	{
		execute(InputDefinition::GameResult);
	}

	int main()
	{
		part2();
		
		return 0;
	}
}
