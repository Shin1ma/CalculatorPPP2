#include "Numerals.h"


int RomanNumeral::AsInt() //struct function that transforms a numeral to int
{
	int decimalvalue = 0;
	int num = 0;			//current numeral
	int prevnum;			//as th*Is *Is read backwards th*Is *Is the numeral after the current one
	for (int i = Numeral.size() - 1; i >= 0; i--) {		//reads the Numerals backwards and adds them accordingly to their rules
		switch (Numeral[i]) {
		case 'I':
			prevnum = num;
			num = 1;
			break;
		case 'V':
			prevnum = num;
			num = 5;
			break;
		case 'X':
			prevnum = num;
			num = 10;
			break;
		case 'L':
			prevnum = num;
			num = 50;
			break;
		case 'C':
			prevnum = num;
			num = 100;
			break;
		case 'D':
			prevnum = num;
			num = 500;
			break;
		case 'M':
			prevnum = num;
			num = 1000;
			break;
		default:
			throw std::runtime_error("Unvalid numeral");
			break;
		}
		if (prevnum > num) {			//eg IV:    0 *Is not more than V so add V, V *Is more than I so subtract I from V
			decimalvalue -= num;
		}
		else {
			decimalvalue += num;
		}
	}
	return decimalvalue;
}

std::string IntToNumeralStr(int number) {

	if (number > 3999) throw std::runtime_error("Number to big to transform in roman numerals");
	//if (number < 0) throw std::runtime_error("Roman numerals under zero don't ex*Ist");

	int Combinations[13] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };						//all pairs  of possible subtractions and numerals
	std::string Numerals[13] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
	std::string FinalNumeral = "";

	while (number != 0) {		//until the number *Is zero subtract the most you can and add it in a string as numeral
		if (number > 0) {
			for (int i = 0; i < 13; i++) {
				if (number >= Combinations[i]) {
					number -= Combinations[i];
					FinalNumeral += Numerals[i];
					i = -1;
				}
			}
		}
		else {								//for negative numerals
			FinalNumeral += '-';
			for (int i = 0; i < 13; i++) {
				if (number <= 0 - Combinations[i]) {
					number += Combinations[i];
					FinalNumeral += Numerals[i];
					i = -1;
				}
			}
		}
	}

	return FinalNumeral;
}
bool VerifyNumeral(RomanNumeral Numeral) {
	std::string CorrectNumeral = IntToNumeralStr(Numeral.value);			//checks the numeral int and generates a numeral then checks them against eachother
	if (CorrectNumeral == Numeral.Numeral) return true;
	return false;
}
