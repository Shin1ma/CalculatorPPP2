#pragma once
#include<iostream>
#include <string>
#include <sstream>
#include "Numerals.h"

struct Token {							//Token Struct, used for tokenizing input
	char Type;		//type of token
	double Value;	//number value if needed
	std::string StringVal;	//string value for commands or names
	RomanNumeral Numeral;	//Roman numeral value


	Token(char type, std::string stringval):
		Type(type), StringVal(stringval), Value(0)
	{
	}
	Token(char type) :
		Type(type), Value(0)
	{
	}
	Token(char type, double value) :
		Type(type), Value(value)
	{
	}
	Token(char type, RomanNumeral numeral):
		Type(type), Numeral(numeral), Value(numeral.value)
	{}

	Token() :
		Type(0), Value(0)
	{
	}
};


class TokenStream						//used to get tokens from input and store them
{

private:
	bool full;						//buffer and check to see if it's already full
	Token Buffer;
	std::istream* Is;

public:
	//constants needed for Token identification and command identifications
	static const char Number_Id = '1';
	static const char Name_Id = 'N';
	static const std::string Declaration_KW;
	static const char Declaration_Id = 'V';
	static const std::string Print_KW;
	static const char Print_Id = 'P';
	static const std::string Remove_KW;
	static const char Remove_Id = 'R';
	static const std::string Assign_KW;
	static const char Assign_Id = 'A';
	static const std::string List_KW;
	static const char List_Id = 'L';
	static const std::string Numeral_KW;
	static const char Numeral_Id = 'K';
	static const std::string From_KW;
	static const char From_Id = 'F';
	static const std::string To_KW;
	static const char To_Id = 'T';
	static const std::string Help_KW;
	static const char Help_Id = 'H';

	static const char BadToken= 'M';		//used when Is fails so it doesn't send junk tokens

	static const std::string Root_KW;
	static const char Root_Id = 'S';



	TokenStream();						//constructor
	std::istream& GetIs() { return *Is; }
	void SetIs(std::istream* is) { Is = is; }

	Token GetToken();					//get next token from input
	void PutBack(Token buffer);			//store token in buffer
	void Clear();						//clear input and buffer in case of errors
	std::string GetName(); //Gets only letters so the name doesn't take out other parts of the operation eg. a+2 name token should be only a not the string
};

