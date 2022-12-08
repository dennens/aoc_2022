#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "Day7_Input.h"

namespace Day7
{
	struct File
	{
		File() :
			isDir(false),
			size(0),
			parent(nullptr)
		{}

		bool isDir;
		int size;
		std::map<std::string, File> files;
		File* parent;
	};

	void list(File& cd, std::stringstream& inputStream)
	{
		std::string line;
		std::stringstream lineStream;
		std::string filename;
		do
		{
			// reached end of file
			if (!std::getline(inputStream, line))
				return;

			lineStream.str(line);
			lineStream.seekg(0);

			File f;
			if (std::isdigit(lineStream.peek())) // File
			{
				int size;
				lineStream >> size >> filename;
				f.size = size;
			}
			else // Dir
			{
				// twice to skip over the 'dir' part
				lineStream >> filename >> filename;
				f.isDir = true;
			}

			f.parent = &cd;
			cd.files[filename] = f;

		} while (inputStream.peek() != '$');
	}

	void changeDir(File*& cd, const std::string& newDir, File& root)
	{
		static std::string dirUp = "..";
		static std::string rootDir = "/";

		if (newDir == dirUp)
			cd = cd->parent;
		else if (newDir == rootDir)
			cd = &root;
		else
			cd = &cd->files[newDir];
	}

	int determineDirSize(File& dir)
	{
		dir.size = 0;
		for (auto& file : dir.files)
			dir.size += file.second.isDir ? determineDirSize(file.second) : file.second.size;
		return dir.size;
	}

	File execute(const char* inputString)
	{
		File root;
		File* cd = nullptr;

		std::stringstream inputStream;
		inputStream.str(inputString);

		std::string line;
		std::stringstream lineStream;

		while (std::getline(inputStream, line))
		{
			lineStream.str(line);
			lineStream.seekg(0);

			std::string command;
			// skip over $
			lineStream >> command >> command;
			if (command == "cd")
			{
				std::string newDir;
				lineStream >> newDir;
				changeDir(cd, newDir, root);
			}
			else
				list(*cd, inputStream);
		}

		determineDirSize(root);

		return root;
	}

	int addDirSizeIfMaxSize(int maxSize, const File& cd)
	{
		int size = cd.size <= maxSize ? cd.size : 0;

		for (auto& iter : cd.files)
			if (iter.second.isDir)
				size += addDirSizeIfMaxSize(maxSize, iter.second);

		return size;
	}

	void part1(const char* inputString)
	{
		File root = execute(inputString);

		int totalSize = addDirSizeIfMaxSize(100000, root);
		std::cout << "Total size: " << totalSize << std::endl;
	}

	int findSmallestDirBetweenSizes(int min, int max, const File& cd)
	{
		if (cd.isDir && cd.size <= max && cd.size >= min)
			max = cd.size;

		for (auto& iter : cd.files)
			max = findSmallestDirBetweenSizes(min, max, iter.second);

		return max;
	}

	void part2(const char* inputString)
	{
		const int fileSystemSize = 70000000;
		const int updateSize = 30000000;

		File root = execute(inputString);

		int freeSpace = fileSystemSize - root.size;
		int requiredFreedSpace = updateSize - freeSpace;

		int dirToDeleteSize = findSmallestDirBetweenSizes(requiredFreedSpace, root.size, root);
		std::cout << "Dir to delete size: " << dirToDeleteSize << std::endl;
	}


	int main()
	{
		part2(input);
		return 0;
	}
}
