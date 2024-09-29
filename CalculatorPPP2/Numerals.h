#pragma once
#include <iostream>

struct RomanNumeral {		//struct to store Roman Numerals
	std::string Numeral;
	int value;



	RomanNumeral() :
		Numeral(""), value(0)
	{
	}
	RomanNumeral(std::string numeral) :
		Numeral(numeral), value(0)
	{
		value = AsInt();
	}

	int AsInt();
};

std::string IntToNumeralStr(int number);		//transforms an integer into a Roman numeral in string
bool VerifyNumeral(RomanNumeral Numeral);		//verify if a Roman numerakìl *Is correct
