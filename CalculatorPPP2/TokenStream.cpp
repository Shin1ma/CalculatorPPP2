#include "TokenStream.h"

//class implementation

//class constants for command keywords initialized 
const std::string TokenStream::Declaration_KW = "var";
const std::string TokenStream::Print_KW = "print";
const std::string TokenStream::Remove_KW = "rm";
const std::string TokenStream::Assign_KW = "make";
const std::string TokenStream::List_KW = "list";
const std::string TokenStream::Numeral_KW = "numeral";
const std::string TokenStream::From_KW = "from";
const std::string TokenStream::To_KW = "to";
const std::string TokenStream::Help_KW = "help";

const std::string TokenStream::Root_KW = "rad";

TokenStream::TokenStream() :		//default constructor
	full(false), Buffer(0), Is(&std::cin)
{
}

Token TokenStream::GetToken() {
	if (full) {					//if buffer is full no need to analyze input
		full = false;
		return Buffer;
	}
		
	char ch;				//get char from input stream and tokenize it
	*Is >> ch;

	if (!*Is) return Token(BadToken);		//if Is failed the current character is junk and is returned as BadToken

	switch (ch) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '{':
		case '}':
		case '!':
		case '%':
		case '^':
		case '=':
		case ';':
		case 'q':
		{
			return Token(ch);			//let the token have as type ch as this characters evaluate by themself, constructor Token(type)
		}
		case '.':case '0':case '1':case '2':case '3':
		case '4':case '5':case '6':case '7':case '8':case '9':
		{
			Is->putback(ch);				//put back the number so it can be assessed as the full number, ex: 123 -> 23 if we don't put back
			double val;
			*Is >> val;
			return Token(Number_Id, val);		//return number token, constructor Token(type, value)
		}
		case 'I':case 'V':case'X':case'L':case'C':case'D':		//roman numerals
		case'M':
		{
			Is->putback(ch);
			std::string numeralstr;
			numeralstr = GetName();
			RomanNumeral Numeral(numeralstr);
			if(!VerifyNumeral(Numeral)) throw std::runtime_error("Incorrect numeral");		//make a numeral and verifies it

			return Token(Number_Id, Numeral);
		}
		case '#':		//used for roots so they can be used with variables # is a "spacer" so that the variable name doesn't unite with root
		default: {								//handles string tokenization
			if (std::isalpha((unsigned char)ch) || ch == '#') {	
				std::string s;
				if (ch != '#') {			//removes # from input
					Is->putback(ch);
				}
				s = GetName();		//gets clear name of variable or command without uniting with other calculations

				if (s == Declaration_KW) {			//if it's a command initialize it as so if not it must be a name
					return Token(Declaration_Id);
				}
				if (s == Print_KW) {
					return Token(Print_Id);
				}
				if (s == Remove_KW) {
					return Token(Remove_Id);
				}
				if (s == Assign_KW) {
					return Token(Assign_Id);
				}

				if (s == List_KW) {
					return Token(List_Id);
				}

				if (s == Numeral_KW) {
					return Token(Numeral_Id);
				}

				if (s == From_KW) {
					return Token(From_Id);
				}

				if (s == To_KW) {
					return Token(To_Id);
				}

				if (s == Help_KW) {
					return Token(Help_Id);
				}

				if (s == Root_KW) {
					return Token(Root_Id);
				}

				return Token(Name_Id, s);
				
			}
			throw std::exception("Invalid Token");
		}
	}
}

void TokenStream::PutBack(Token buffer) {						//put token in buffer
	if (full) {
		throw std::exception("Buffer is full can't putback()");
	}
	Buffer = buffer;
	full = true;
}

void TokenStream::Clear() {				//clears buffer and input if errors appear
	if (Is == &std::cin) {				//dont do error clear with files as it will reset EOF
		Is->clear();
		Is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');	//and this clears file
	}
	else {
		Token tk = GetToken();			//throws away characters till it finds an end character
		while (tk.Type != ';') {
			tk = GetToken();
		}
	}
	full = false;
}

std::string TokenStream::GetName() {
	char ch;
	std::string name;
	while (*Is >> std::noskipws >> ch) {		//gets all the letters till a number or a whitespace
		if (isalpha(ch) || ch == '.') {
			name += ch;
		}
		else {
			Is->unget();		//puts the non used char back in input 
			break;
		}
	}
	std::skipws(*Is);		//resets I/O flag
	if (name == "") throw std::runtime_error("bad name"); 
	return name;
}