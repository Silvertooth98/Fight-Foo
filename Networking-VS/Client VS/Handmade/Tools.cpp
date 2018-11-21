#include "Tools.h"
#include <string>
#include <iostream>

void CutString(std::string& Str, unsigned int Start, unsigned int End)
{
	Str = Str.substr(Start, End);
}

void AddString(std::string& Str, std::string& AddedStr)
{
	Str += " " + AddedStr;
}

void ListString(const std::string& Str)
{
	for (auto it = Str.begin(); it != Str.end(); it++)
	{
		std::cout << *it << std::endl;
	}

	for (size_t i = 0; i < Str.size(); i++)
	{
		std::cout << Str[i] << std::endl;
	}
}

bool SortDescending(int left, int right)
{
	return left > right;
}

bool IsOdd(const int num)
{
	if (num % 2 == 1)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool SortNameLength(std::string Str1, std::string Str2)
{
	if (Str1.size() > Str2.size())
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool SortNameVowel(std::string Str1, std::string Str2)
{
	if (Str1[0] == 'A' || Str1[0] == 'E' || Str1[0] == 'I' || Str1[0] == 'O' || Str1[0] == 'U' ||
		Str1[0] == 'a' || Str1[0] == 'e' || Str1[0] == 'i' || Str1[0] == 'o' || Str1[0] == 'u')
	{
		return true;
	}

	else
	{
		return false;
	}
}

void ParseString(std::string StrParse, std::vector<std::string>& StrVector, std::string Separator)
{
	size_t StrPos = 0;
	std::string SubStr;

	if (StrParse.find(Separator) == std::string::npos)
	{
		std::cout << "Cannot find token separator!" << std::endl;
	}

	while ((StrPos = StrParse.find(Separator)) != std::string::npos)
	{
		SubStr = StrParse.substr(0, StrPos);
		StrVector.push_back(SubStr);
		StrParse.erase(0, StrPos + 1);
		std::cout << SubStr << std::endl;
	}

	if (StrParse.size() > 0)
	{
		StrVector.push_back(StrParse);

		std::cout << StrParse << std::endl;
	}
}

void BinaryToDecimalTable(std::vector<int>& BinaryVector, const int bitrate)
{
	int DecimalNumber = 0;

	std::cout << "Binary = ";

	for (size_t i = 0; i < BinaryVector.size(); i++)
	{
		DecimalNumber += BinaryVector[i] * std::pow(2, (bitrate - 1) - i);

		std::cout << BinaryVector[i];
	}
	std::cout << std::endl;
	std::cout << "Decimal = " << DecimalNumber << std::endl;
}

void DecimalToBinaryDoubling(int DecimalNumber, const int bitrate, std::vector<int>& Binary)
{
	int BinaryNumber = 0;
	int HalvedNumber = 0;

	std::cout << "Decimal = " << DecimalNumber;

	for (size_t i = 0; i < 0; i++)
	{
		HalvedNumber = DecimalNumber / 2;

		if (HalvedNumber % 2)
		{
			Binary.push_back(0);
		}
		else
		{
			Binary.push_back(1);
		}
	}

	std::cout << std::endl;
	std::cout << "Binary = " << std::endl;

	for (size_t i = 0; i < Binary.size(); i++)
	{
		std::cout << Binary[i];
	}
}