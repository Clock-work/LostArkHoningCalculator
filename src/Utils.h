#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

//returns how much cheaper the target is than the source in 1% to n%. 
//a negative result means that the target is more expensive
inline float howMuchCheaper(float cheaperTarget, float expensiveSource)
{
	return ( 1.0f - cheaperTarget / expensiveSource ) * 100.0f;
}

inline bool isLess(float less, float more)
{
	return less < more - 0.000001f;
}

inline bool isMore(float more, float less)
{
	return more > less + 0.000001f;
}

inline bool isMoreOrEqual(float more, float less)
{
	return more >= less - 0.000001f;
}

template <typename T> 
inline std::string to_string_noZeros(const T& t)
{
	std::string str { std::to_string(t) };
	int offset { 1 };
	if ( str.find_last_not_of('0') == str.find('.') )
	{
		offset = 0;
	}
	str.erase(str.find_last_not_of('0') + offset, std::string::npos);
	return str;
}

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
inline std::vector<std::string> readUtf8FileToLines(const std::string& filePath)
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
inline bool writeUtf8FileFromLines(const std::string& filePath, const std::vector<std::string>& lines)
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

inline void sleep(int milleseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milleseconds));
}


