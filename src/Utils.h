#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

inline void roundUpTo2DecimalsIfPossible(double& input)
{
	int number = input * 100;
	int nextDecimal = static_cast<int>( input * 1000 ) % 10;
	if ( nextDecimal > 0 )
	{
		number++;
	}
	input = number / 100.0;
}

inline std::vector<std::string> splitString(std::string inputString, char splitChar, int maxParts = 0)
{
	std::vector<std::string> vector;
	std::string temp;
	auto pos = inputString.find(splitChar);
	while ( pos < std::string::npos )
	{
		if ( --maxParts == 0 )
		{
			break;
		}
		vector.emplace_back(inputString.substr(0, pos));
		if ( pos == inputString.size() - 1 )
		{
			vector.push_back(std::string());
			return vector;
		}
		inputString.erase(inputString.begin(), inputString.begin() + pos + 1);
		pos = inputString.find(splitChar);
	}
	vector.emplace_back(std::move(inputString));
	return vector;
}

inline std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& vector, char splitChar, int maxParts = 0)
{
	std::vector<std::vector<std::string>> splitted;
	for ( const auto& string : vector )
	{
		splitted.push_back(splitString(string, splitChar, maxParts));
	}
	return splitted;
}

//Simple function to read an utf8 file into lines splitted at \n
static std::vector<std::string> readUtf8FileToLines(const std::string& filePath)
{
	std::ifstream file(filePath);
	std::vector<std::string> lines;
	std::string line;
	try
	{
		if ( file.is_open() )
		{
			while ( std::getline(file, line) )
			{
				lines.emplace_back(std::move(line));
			}
		}
		if ( lines.empty() )
		{
			std::cout << std::string(std::string("error reading file: ") + filePath);
		}
	}
	catch ( const std::exception& e )
	{
		lines.clear();
		std::cout << std::string(std::string("error reading file: ") + filePath + " exception: " + e.what());
	}
	return lines;
}

//Simple function to write an utf8 file from lines splitted at \n
static bool writeUtf8FileFromLines(const std::string& filePath, const std::vector<std::string>& lines)
{
	try
	{
		std::ofstream file;
		file.open(filePath);
		for ( size_t i = 0; i < lines.size(); ++i )
		{
			file << lines[i];
			if ( i != lines.size() - 1ull )
			{
				file << '\n';
			}
		}
		return true;
	}
	catch ( const std::exception& e )
	{
		std::cout << std::string(std::string("error writing file: ") + filePath + " exception: " + e.what());
	}
	return false;
}


