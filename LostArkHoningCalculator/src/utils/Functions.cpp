#include "LostArkHoningCalculator/utils/Functions.h"
#include "LostArkHoningCalculator/utils/StringManipulation.h"
#include <fstream>
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "boost/thread.hpp"
#include <Windows.h>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <tlhelp32.h>
#include <psapi.h>
#include <codecvt>
#include <locale>
#include <openssl/md5.h>
#include <math.h>
#include <chrono>
#include <ShlObj_core.h>

namespace utils {

	int logLevel = 2;
	float epsilon = 0.0001f;

	bool isEqual(float a, float b, float epsilon /*= 0.0001f*/)
	{
		return std::abs(a - b) < epsilon;
	}

	bool isLessThan(float less, float more, float epsilon /*= epsilon*/)
	{
		return less + epsilon < more;
	}

	bool isMoreThan(float more, float less, float epsilon /*= epsilon*/)
	{
		return more - epsilon > less;
	}

	bool isLessOrEqualThan(float less, float moreOrEqual, float epsilon /*= epsilon*/)
	{
		return !isMoreThan(less, moreOrEqual);
	}

	bool isMoreOrEqualThan(float more, float lessOrEqual, float epsilon /*= epsilon*/)
	{
		return !isLessThan(more, lessOrEqual);
	}

	bool hasDecimalPoints(float input, float epsilon /*= epsilon*/)
	{
		return ( input - std::floor(input) ) > epsilon;
	}

	bool isZero(float input, float epsilon /*= epsilon*/)
	{
		return input <= epsilon;
	}

	bool isEqual(double a, double b, double epsilon /*= 0.0001*/)
	{
		return std::abs(a - b) < epsilon;
	}

	float gcd(float a, float b)
	{
		if ( a < b )
			return gcd(b, a);
		if ( std::fabs(b) < epsilon )
			return a;
		else
			return ( gcd(b, a - std::floor(a / b) * b) );
	}

	int getRandomNumber(int from, int to)
	{
		static bool once = true;
		if ( once )
		{
			std::srand(std::time(NULL));
			once = false;
		}
		if ( from >= to )
		{
			return from;
		}
		return std::rand() % ( to - from + 1 ) + from;
	}

	void sleep(int milliseconds)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds { milliseconds });
	}

	void sleepRandom(int fromInMilliseconds, int toInMilliseconds)
	{
		sleep(getRandomNumber(fromInMilliseconds, toInMilliseconds));
	}

	void sleepDefault()
	{
		sleepRandom(8, 10);
	}

	unsigned long long getCurrentTimeInMilliseconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
	}

	std::string md5Hash(const std::string& input)
	{
		unsigned char result[MD5_DIGEST_LENGTH];
		std::string output;
		output.reserve(input.size());
		MD5_CTX c;
		MD5_Init(&c);
		MD5_Update(&c, input.c_str(), input.size());
		MD5_Final(result, &c);
		for ( int i = 0; i < MD5_DIGEST_LENGTH; ++i )
		{
			output += std::to_string(result[i]);
		}
		return output;
	}

	std::wstring getWindowsDocumentsPath()
	{
		WCHAR docPath[MAX_PATH];
		HRESULT hr = SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docPath);
		return std::wstring(docPath);
	}

	std::wstring getWindowsAppdataPath()
	{
		WCHAR docPath[MAX_PATH];
		HRESULT hr = SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, docPath);
		return std::wstring(docPath);
	}

	std::wstring getWindowsProgramFilesPath()
	{
		WCHAR docPath[MAX_PATH];
		HRESULT hr = SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, docPath);
		return std::wstring(docPath);
	}
	std::wstring getWindowsProgramFilesX86Path()
	{
		WCHAR docPath[MAX_PATH];
		HRESULT hr = SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILESX86, NULL, SHGFP_TYPE_CURRENT, docPath);
		return std::wstring(docPath);
	}

}