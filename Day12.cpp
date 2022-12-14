#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <assert.h>

#include "Day12_Input.h"


namespace Day12
{
	struct HeightMap
	{
		// Heightmap just uses the raw input as data
		HeightMap(const char* map)
			: map(map)
		{
			std::string tmp(map);
			int len = tmp.length();
			// Determine map size and start/end points
			for (int i = 0; i < len; ++i)
			{
				if (width == -1
					&& (map[i] == '\n'
						|| map[i] == '\r'))
					width = i + 1;

				if (map[i] == 'S')
					start = i;
				if (map[i] == 'E')
					end = i;

				// We've got all the data we need, exit
				if (start != -1
					&& end != -1
					&& width != -1)
					break;
			}
			height = (len + 1) / width;
		}

		bool valid(int x, int y) const
		{
			// Must be within bounds
			return x >= 0 && x < width&& y >= 0 && y < height;
		}

		int index(int x, int y) const
		{
			// Convert coordinates to direct index in the input array
			assert(valid(x, y));
			return x + width * y;
		}

		int elevation(int x, int y) const
		{
			// Returns elevation as a value between 0 and 25 inclusive
			char elev = map[index(x, y)]; 
			return elev == 'S' ? 0 : elev == 'E' ? 25 : elev - 'a';
		}
		int elevation(int idx) const
		{
			auto c = coords(idx);
			return elevation(c.first, c.second);
		}

		std::pair<int, int> coords(int index) const
		{
			// Convert raw index to coordinates
			return { index % width, index / width };
		}

		std::vector<int> getNeighbors(int idx) const
		{
			// Get all orthogonally neighboring cell indices
			std::vector<int> ret;
			int x, y;
			std::tie(x, y) = coords(idx);
			if (x > 0)
				ret.push_back(idx - 1);
			if (x < width - 1)
				ret.push_back(idx + 1);
			if (y > 0)
				ret.push_back(idx - width);
			if (y < height - 1)
				ret.push_back(idx + width);
			return ret;
		}

		int dist(int index1, int index2) const
		{
			// Distance between two nodes
			std::pair<int, int> p1 = coords(index1);
			std::pair<int, int> p2 = coords(index2);
			int dx = p2.first - p1.first;
			int dy = p2.second - p1.second;
			return dx * dx + dy * dy;
			//return std::abs(dx) + std::abs(dy);
		}

		int width = -1;
		int height = -1;

		int start = -1;
		int end = -1;

		const char* map;
	};

	struct PathNode
	{
		PathNode* parent;
		int index;
		int distance;
		int pathLength;
		int score;
	};

	int printGrid(const HeightMap& map, const PathNode* currentNode)
	{
		// Trace back path through node parents
		std::vector<int> traversal;
		while (currentNode != nullptr)
		{
			traversal.push_back(currentNode->index);
			currentNode = currentNode->parent;
		}

		// Direct copy of the map data to overlay path on
		size_t size = map.width * map.height;
		char* mapCopy = new char[size];
		memcpy(mapCopy, map.map, size);

		for (size_t i = 1; i < traversal.size() - 1; ++i)
		{
			int index = traversal[i];
			int x, y, nx, ny, px, py;
			std::tie(x, y) = map.coords(index);
			
			std::tie(nx, ny) = map.coords(traversal[i + 1]);
			std::tie(px, py) = map.coords(traversal[i - 1]);

			//horizontal
			if ((px < x && nx > x)
				|| (px > x && nx < x))
				mapCopy[index] = (char)196;

			// bend left -> down
			if ((px < x && ny > y)
				|| (py > y && nx < x))
				mapCopy[index] = (char)191;

			// bend left -> up
			if ((px < x && ny < y)
				|| (py < y && nx < x))
				mapCopy[index] = (char)217;

			// bend right -> up
			if ((px > x && ny < y)
				|| (py < y && nx > x))
				mapCopy[index] = (char)192;
			// bend right -> down
			if ((px > x && ny > y)
				|| (py > y && nx > x))
				mapCopy[index] = (char)218;
			// vertical
			if ((py < y && ny > y)
				|| (py > y && ny < y))
				mapCopy[index] = (char)179;
		}

		std::cout << mapCopy << std::endl << std::endl;

		delete[] mapCopy;
		return (int)traversal.size();
	}

	void remove(std::vector<PathNode*>& list, PathNode* node)
	{
		for (auto iter = list.begin(); iter != list.end(); ++iter)
			if (*iter == node)
			{
				list.erase(iter);
				return;
			}
	}

	int calculateScore(int dist, int path)
	{
		// Semi-arbitrary score calculation that takes both traversed path and distance to the final node into account
		return dist + (path * path);
	}

	void updateClosestNode(const HeightMap& map, int index, int x, int y, int elevationToFind, int& closestNode, int& shortestDistance)
	{
		if (map.valid(x, y)) 
		{
			int nodeIndex = map.index(x, y);
			int dist = map.dist(index, nodeIndex);
			if (map.elevation(x, y) == elevationToFind
				&& (closestNode == -1 || dist < shortestDistance))
			{
				shortestDistance = dist;
				closestNode = nodeIndex;
			}
		}
	}

	int findNearest(const HeightMap& map, int index, int elevationToFind)
	{
		// Circular sweep around node 'index'
		int sweep = 1;
		int x, y;
		std::tie(x, y) = map.coords(index);
		int shortestDistance = 1337 * 1337 * 2;
		int closestNode = -1;

		while (true)
		{
			// Can't just return the very first index with the right elevation that we find,
			// since another may end up being closer (can be skipped if manhattan distance is used)
			int td = sweep > 0 ? 1 : -1;
			int tx = x;
			for (; tx != x + sweep; tx += td)
				updateClosestNode(map, index, tx, y, elevationToFind, closestNode, shortestDistance);

			x = tx;
			int ty = y;
			for (; ty != y + sweep; ty += td)
				updateClosestNode(map, index, x, ty, elevationToFind, closestNode, shortestDistance);

			if (closestNode != -1)
				return closestNode;

			y = ty;
			sweep *= -1;
			sweep += sweep > 0 ? 1 : -1;
		}
	}

	int part1_calculateDistance(const HeightMap& map, int node)
	{
		// Regular distance to map end
		return map.dist(node, map.end);
	}

	bool part1_elevationValid(int diff)
	{
		// Can fall down any distance, climb only 1
		return diff <= 1;
	}

	int part2_calculateDistance(const HeightMap& map, int node)
	{
		// Custom distance function to find the nearest 'a' elevation
		if (map.elevation(node) == 0)
			return 0;
		int nearestA = findNearest(map, node, 0);
		return map.dist(node, nearestA);
	}

	bool part2_elevationValid(int diff)
	{
		// Since we're reversing the search, invert the elevation validation
		return diff >= -1;
	}

	int execute(const HeightMap& map, int rootStart, std::function<int(const HeightMap&, int)> calculateDistance, std::function<bool(int)> elevationValid)
	{
		PathNode root{ nullptr, rootStart, calculateDistance(map, rootStart), 0 };
		root.score = calculateScore(root.distance, 0);

		std::unordered_map<int, PathNode> allNodes = { { root.index, root } };
		std::map<int, std::vector<PathNode*>> unhandledNodes;
		unhandledNodes[root.score].push_back(&root);
		PathNode* currentNode;
		int iters = 0;
		while (true)
		{
			// Node reparenting may cause some score entries to become empty
			while (unhandledNodes.size() > 0
				&& unhandledNodes.begin()->second.empty())
				unhandledNodes.erase(unhandledNodes.begin());

			if (unhandledNodes.size() == 0)
			{
				// No more nodes to check, so path doesn't exist
				currentNode = nullptr;
				break;
			}
			currentNode = unhandledNodes.begin()->second[0];
			// Distance of 0 means we've reached the target!
			if (currentNode->distance == 0)
				break;

			// Add neighbors to the nodes to process, if they're a valid path from where we are now
			int elevation = map.elevation(currentNode->index);
			std::vector<int> neighbors = map.getNeighbors(currentNode->index);
			for (size_t i = 0; i < neighbors.size(); ++i)
			{
				int neighborElevation = map.elevation(neighbors[i]);
				int elevationDiff = neighborElevation - elevation;
				if (elevationValid(elevationDiff)
					&& (currentNode->parent == nullptr
						|| neighbors[i] != currentNode->parent->index))	// our parent is a neighbor by definition, ignore it
				{
					auto iter = allNodes.find(neighbors[i]);
					if (iter != allNodes.end())
					{
						// re-parent - our path is shorter
						if (currentNode->pathLength + 1 < iter->second.pathLength)
						{
							// Remove old version from unhandled nodes if it exists
							remove(unhandledNodes[iter->second.score], &iter->second);
							
							// Update and add to unhandled nodes
							iter->second.parent = currentNode;
							iter->second.pathLength = currentNode->pathLength + 1;
							iter->second.score = calculateScore(iter->second.distance, iter->second.pathLength);
							unhandledNodes[iter->second.score].push_back(&iter->second);
						}
					}
					else
					{
						// new node
						int dist = calculateDistance(map, neighbors[i]);
						PathNode node{ currentNode, neighbors[i], dist, currentNode->pathLength + 1 };
						node.score = calculateScore(dist, node.pathLength);
						allNodes[node.index] = node;
						unhandledNodes[node.score].push_back(&allNodes[node.index]);
					}
				}
			}

			// Node's been handled, remove from list
			remove(unhandledNodes[currentNode->score], currentNode);
		}

		if (currentNode != nullptr)
		{
			int numSteps = printGrid(map, currentNode);
			std::cout << "Required number of steps: " << numSteps - 1 << std::endl;
			return numSteps;
		}
		else
		{
			std::cout << "No valid path found for start node " << rootStart << std::endl;
			return -1;
		}
	}


	void part1(const char* input)
	{
		HeightMap map(input);
		execute(map, map.start, part1_calculateDistance, part1_elevationValid);
	}

	void part2(const char* input)
	{
		HeightMap map(input);
		execute(map, map.end, part2_calculateDistance, part2_elevationValid);
	}

	int main()
	{
		part1(input);
		part2(input);
		return 0;
	}
}
