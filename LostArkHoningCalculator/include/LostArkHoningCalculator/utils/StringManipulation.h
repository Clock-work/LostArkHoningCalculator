#pragma once 
#include <string>
#include <vector>
#include <sstream>

namespace utils {

	//replacement for std variant, because here . will be replaced by , | because of locale language/etc
	float stof(std::wstring input);
	//replacement for std variant, because here . will be replaced by , | because of locale language/etc
	float stof(std::string input);

	std::wstring floatToWString(float amount);

	std::string floatToString(float amount);

	std::wstring utf8ToWstring(const std::string& input);

	std::string wStringToUtf8(const std::wstring& input);

	//use it to split a string in a vector with the parts between the splitchars. if maxParts is not 0, it will be the max amount of splitted parts | the splitchar is not included in any part | if the splitchar is the last character, the last part will be empty
	template<typename stringType, typename charType>
	std::vector<stringType> splitString(stringType inputString, charType splitChar, int maxParts = 0)
	{
		std::vector<stringType> vector;
		stringType temp;
		auto pos = inputString.find(splitChar);
		while ( pos < stringType::npos )
		{
			if ( --maxParts == 0 )
			{
				break;
			}
			vector.emplace_back(inputString.substr(0, pos));
			if ( pos == inputString.size() - 1 )
			{
				vector.push_back(stringType());
				return vector;
			}
			inputString.erase(inputString.begin(), inputString.begin() + pos + 1);
			pos = inputString.find(splitChar);
		}
		vector.emplace_back(std::move(inputString));
		return vector;
	}

	//replaces all toFind substrings by the replacement string in the inputString and will return the string with the replaced characters. WILL not MODIFY THE PARAMETER!
	template<typename stringType>
	stringType replaceInString(const stringType& inputString, const stringType& toFind, const stringType& replacement)
	{
		stringType outputString;
		outputString.reserve(inputString.size());
		for ( int i = 0; i < inputString.size(); )
		{
			if ( i + toFind.size() > inputString.size() )
			{
				outputString += inputString.substr(i);
				break;
			}
			if ( inputString.substr(i, toFind.size()) == toFind )
			{
				outputString += replacement;
				i += toFind.size();
			}
			else
			{
				outputString += inputString[i];
				++i;
			}
		}
		return outputString;
	}

	template <typename T>
	inline std::wstring to_wstring_with_precision(const T a_value, const int n = 6)
	{
		std::wostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return out.str();
	}

	template <typename T>
	inline std::string to_string_with_precision(const T a_value, const int n = 6)
	{
		std::ostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return out.str();
	}

	//splits every string of the vector into a vector itself with the amount of maxParts parts | if maxparts is zero, it will split to infinite parts | uses the splitchar like the splitstring method and its not included in any part | if the splitchar is the last character, the last part will be empty
	template<typename stringType, typename charType>
	std::vector<std::vector<stringType>> splitVector(const std::vector<stringType>& vector, charType splitChar, int maxParts = 0)
	{
		std::vector<std::vector<stringType>> splitted;
		for ( const auto& string : vector )
		{
			splitted.push_back(splitString<stringType, charType>(string, splitChar, maxParts));
		}
		return splitted;
	}

	//reverses the effect and includes splitChar
	template<typename stringType, typename charType>
	std::vector<stringType> unsplitVector(const std::vector<std::vector<stringType>>& vector, charType splitChar)
	{
		std::vector<stringType> unsplitted;
		for ( const auto& parts : vector )
		{
			stringType temp;
			for ( const auto& string : parts )
			{
				temp += string;
				temp += splitChar;
			}
			if ( !temp.empty() )
				temp.pop_back();
			unsplitted.push_back(std::move(temp));
		}
		return unsplitted;
	}

}