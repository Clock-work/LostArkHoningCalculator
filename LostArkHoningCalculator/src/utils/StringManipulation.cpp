#include "LostArkHoningCalculator/utils/StringManipulation.h"
#include "LostArkHoningCalculator/utils/Functions.h"
#include <string>
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */

namespace utils {

	float stof(std::wstring input)
	{
		auto pos = input.find(L'.');
		if ( pos != std::wstring::npos )
		{
			input[pos] = L',';
		}
		return std::stof(input);
	}

	float stof(std::string input)
	{
		auto pos = input.find('.');
		if ( pos != std::string::npos )
		{
			input[pos] = ',';
		}
		return std::stof(input);
	}

	std::wstring floatToWString(float amount)
	{
		if ( hasDecimalPoints(amount) )
		{
			return to_wstring_with_precision(amount, 4);
		}
		else
		{
			return std::to_wstring((int) amount);
		}
	}

	std::string floatToString(float amount)
	{
		if ( hasDecimalPoints(amount) )
		{
			return to_string_with_precision(amount, 4);
		}
		else
		{
			return std::to_string((int) amount);
		}
	}

	static const long hextable[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1, 0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	};

	static long stringToHex(const char* hex)
	{
		long ret = 0;
		while ( *hex && ret >= 0 )
		{
			ret = ( ret << 4 ) | hextable[*hex++];
		}
		return ret;
	}

	static bool isEscaped(const std::string& input, int position)
	{
		bool escaped = false;
		while ( position >= 0 )
		{
			if ( input.at(position) != '\\' )
			{
				break;
			}
			escaped = escaped ? false : true;
			position--;
		}
		return escaped;
	}

	std::wstring utf8ToWstring(const std::string& input)
	{
		std::wstring output;
		for ( int i = 0; i < input.size(); ++i )
		{
			if ( input.substr(i, 2) == "\\u" && input.size() > i + 5 )
			{
				if ( !isEscaped(input, i - 1) )
				{
					std::string hexcode = input.substr(i + 2, 4);
					int hexNum = stringToHex(hexcode.c_str());
					output += static_cast<wchar_t>( hexNum );
					i += 5;
				}
				else
					output += input.at(i);
			}
			else if ( input.substr(i, 2) == "\\U" && input.size() > i + 9 )
			{
				std::string hexcode = input.substr(i + 2, 8);
				int hexNum = stringToHex(hexcode.c_str());
				output += static_cast<wchar_t>( hexNum );
				i += 9;
			}
			else
				output += input.at(i);
		}
		return output;
	}

	std::string wStringToUtf8(const std::wstring& input)
	{
		return std::string(input.begin(), input.end());//todo: in the future IMPLEMENT this like the one above
	}

}