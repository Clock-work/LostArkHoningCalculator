#pragma once 
#include <string>
#include <vector>

namespace utils {

	extern float epsilon;

	//check if equal
	bool isEqual(float a, float b, float epsilon = utils::epsilon);
	//returns if left float is less than right float
	bool isLessThan(float less, float more, float epsilon = utils::epsilon);
	//returns if left float is more than right float
	bool isMoreThan(float more, float less, float epsilon = utils::epsilon);
	//returns if left float is less or equal than right float
	bool isLessOrEqualThan(float less, float moreOrEqual, float epsilon = utils::epsilon);
	//returns if left float is more or equal than right float
	bool isMoreOrEqualThan(float more, float lessOrEqual, float epsilon = utils::epsilon);

	bool hasDecimalPoints(float input, float epsilon = utils::epsilon);

	bool isZero(float input, float epsilon = utils::epsilon);
	//check if equal
	bool isEqual(double a, double b, double epsilon = 0.0001);


	//greates common divisor
	float gcd(float a, float b);

	//random number inclusive
	int getRandomNumber(int from, int to);
	//makes current thread sleep for that many milliseconds between those two numbers inclusive
	void sleepRandom(int fromInMilliseconds, int toInMilliseconds);
	//the default random timed sleep 
	void sleepDefault();
	//makes current thread sleep for that many milliseconds
	void sleep(int milleseconds);

	//returns the time that has passed
	unsigned long long getCurrentTimeInMilliseconds();

	//calculates UNSECURE hash from given string
	std::string md5Hash(const std::string& input);

	//returns C:\\Users\\PC\\Documents
	std::wstring getWindowsDocumentsPath();
	//returns C:\\Users\\PC\\AppData\\Roaming
	std::wstring getWindowsAppdataPath();
	//returns C:\\Program Files
	std::wstring getWindowsProgramFilesPath();
	//returns C:\\Program Files (x86)
	std::wstring getWindowsProgramFilesX86Path();

}