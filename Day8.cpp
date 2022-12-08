#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "Day8_Input.h"

namespace Day8
{
	struct Tree
	{
		uint8_t height;
		bool visible;
	};

	int updateTreeVisibility(Tree& tree, uint8_t& tallestTree, int& visibleTrees)
	{
		uint8_t treeHeight = tree.height;
		bool isVisible = tallestTree == 255 || treeHeight > tallestTree;
		visibleTrees += isVisible && !tree.visible;
		tree.visible |= isVisible;
		if (isVisible)
			tallestTree = treeHeight;
		return treeHeight;
	}

	void execute(const char* inputString)
	{
		std::vector<std::vector<Tree>> forest;
		forest.push_back(std::vector<Tree>());

		int row = 0;
		const char* curChar = inputString;
		uint8_t tallestTree = 255;
		int visibleTrees = 0;
		// Parse input + initial left-hand visibility check
		while (*curChar != 0)
		{
			if (*curChar == '\r')
			{
				++curChar;
				continue;
			}
			if (*curChar == '\n')
			{
				tallestTree = 255;
				forest.push_back(std::vector<Tree>());
				++curChar;
				++row;
				continue;
			}
			uint8_t treeHeight = *curChar - '0';
			Tree tree;
			tree.height = treeHeight;
			tree.visible = treeHeight > tallestTree || tallestTree == 255;
			visibleTrees += tree.visible;
			if (tallestTree == 255 || treeHeight > tallestTree)
				tallestTree = treeHeight;
			forest[row].push_back(tree);
			++curChar;
		}

		int numRows = forest.size();
		int numColumns = forest[0].size();

		// right-hand check.
		// Stop when we find a tree of height 9 - no further trees will be visible from this side
		tallestTree = 255;
		for (int y = 0; y < numRows; ++y, tallestTree = 255)
			for (int x = numColumns - 1; x >= 0; --x)
				if (updateTreeVisibility(forest[y][x], tallestTree, visibleTrees) == 9)
					break;

		// top-down check
		tallestTree = 255;
		for (int x = 0; x < numColumns; ++x, tallestTree = 255)
			for (int y = 0; y < numRows; ++y)
				if (updateTreeVisibility(forest[y][x], tallestTree, visibleTrees) == 9)
					break;

		// bottom-up check
		tallestTree = 255;
		for (int x = 0; x < numColumns; ++x, tallestTree = 255)
			for (int y = numRows - 1; y >= 0; --y)
				if (updateTreeVisibility(forest[y][x], tallestTree, visibleTrees) == 9)
					break;

		std::cout << "Visible trees: " << visibleTrees << std::endl;

		// PART 2

		int highestScore = 0;
		// Start at 1, end at one before the end - any tree on the edge will have a score of 0
		// and is therefore not viable for highscore
		for (int y = 1; y < numRows - 1; ++y)
			for (int x = 1; x < numColumns - 1; ++x)
			{
				int height = forest[y][x].height;
				int up, down, left, right;

				// For all these directions, we don't need to check the very last tree: 
				// if we can see the tree before it and it didn't break out of the loop (aka it's not higher than our current tree)
				// we can see the last tree as well (and our variable will have been increased accordingly).
				// If we did check the very last option, we'd also have to subtract 1 from the direction variable since it'd count
				// a tree beyond the edge of the forest

				// look up
				// Start at 1: From the initial loop, we're not at the edge so can at least see 1 tree in all directions.
				// Up to but not including current y coordinate to check all trees above
				for (up = 1; up < y; ++up)
					if (forest[y - up][x].height >= height)
						break;
				// down
				for (down = 1; down < numRows - y - 1; ++down)
					if (forest[y + down][x].height >= height)
						break;
				// left
				for (left = 1; left < x; ++left)
					if (forest[y][x - left].height >= height)
						break;
				// right
				for (right = 1; right < numColumns - x - 1; ++right)
					if (forest[y][x + right].height >= height)
						break;

				int score = up * down * left * right;
				if (score > highestScore)
					highestScore = score;
			}

		std::cout << "Highest score: " << highestScore << std::endl;
	}

	int main()
	{
		execute(input);
		return 0;
	}
}
