
/*Made by Shinima, A little calculator in c++ based on the one detailed in programming, principles and practices*/


#include <iostream>
#include <cmath>
#include <fstream>
#include "TokenStream.h"
#include "VarTable.h"
#include "Numerals.h"


//grammar:

/*
Statement:
	Expression
	("command")
Expression:
	Term
	Expression + Term
	Expression - Term
Term:
	Part
	Term * Part
	Term / Part
	Term % Part
Part:
	Secondary
	Part^secondary
	Part!
	Secondary#root(Part)
Secondary:
	Primary
	{Expression}
Primary:
	Number
	(Expression)
Number:
	double
*/



//General Functions
void Calculate(TokenStream& ts, VarTable& vt, bool& Romanized, std::ifstream& Ifile, std::ofstream& Ofile, int& counter); //calculates input
int Factorial(int num);			//factorial !x
int Romanization(bool& Romanized); //romanize itself
int IOchoose(TokenStream& ts, std::ifstream& Ifile, std::ofstream& Ofile);				//makes possible writing to file by Output and getting input by Is
void ResetIo(TokenStream& ts, std::ifstream& Ifile, std::ofstream& Ofile, int& counter);	//resets IO back to terminal
int Help();

//Variable specific functions
double Declaration(TokenStream& ts, VarTable& vt); //handles variable declaration
double AssignVar(TokenStream& ts, VarTable& vt);	//modifies a variable
double SearchVar(TokenStream& ts, VarTable& vt);	//searchs for a variable in variable vector
double RemoveVar(TokenStream& ts, VarTable& vt);	//removes a variable
double ListVars(VarTable& vt);		//prints all variables

//grammar functions
double Statement(TokenStream& ts, VarTable& vt, bool& Romanized, std::ifstream& Ifile, std::ofstream& Ofile, bool& skip);
double Expression(TokenStream& ts, VarTable& vt);
double Term(TokenStream& ts, VarTable& vt);
double Part(TokenStream& ts, VarTable& vt);
double Secondary(TokenStream& ts, VarTable& vt);
double Primary(TokenStream& ts, VarTable& vt);

 

//constants that determine special characters used
const char Quit_Char = 'q';
const char End_Char = ';';
const std::string Prompt = ">";
const std::string Result = "= ";
std::ostream* Output = &(std::cout); //output



int main()
{
	TokenStream ts;	//Token stream object
	VarTable vt;	//variable table
	bool Romanized = false;		//enables Roman Numerals output
	std::ifstream Ifile;		//if and of streams
	std::ofstream Ofile;
	int counter = 0;			//counter for files and output formatting


	vt.NewVariable("Pi", 3.14159265358979323846);		//pre inserted variable
	vt.NewVariable("e", 2.718281828459);

	*Output << "welcome to cpp calc, use '" << End_Char <<  "' to end calculation, use '" << Quit_Char << "' to quit, also use: '" << ts.Help_KW << "' command to view commands\n\n";

	while (true) {		//main loop
		try {
			Calculate(ts, vt, Romanized, Ifile, Ofile, counter);		//Gets and operates on input and outputs
			break;
		}
		catch (std::exception& e) {			//programmed exceptions
			*Output << e.what() << '\n';
			ts.Clear();						//clears input from error

		}
		catch (...) {						//unexpected exceptions
			*Output << "exception occurred";
			return 2;
		}
	}
	return 0;
}

void Calculate(TokenStream& ts, VarTable& vt, bool& Romanized, std::ifstream& Ifile, std::ofstream& Ofile, int& counter) {
	Token t;
	double res = 0;
	bool skip;		//used for commands that don't have to return a value

	while (ts.GetIs()) {
		skip = false;
		*Output << Prompt;

		t = ts.GetToken();			//evaluate special chars, if there aren't put back and solve

		while (t.Type == End_Char) { t = ts.GetToken(); } //discard prints
		if (t.Type == Quit_Char) return;
		ts.PutBack(t);

		double result = Statement(ts, vt, Romanized, Ifile, Ofile, skip);
		if (skip) continue;	//used when commands are ran as to not produce junk values

		if (Output != &std::cout) {		//operation counter when inputs are taken from files for formatting purposes
			*Output << counter << ". ";
			counter++;
		}
		if (Romanized) {		//Roman Numerals output
			*Output << Result << IntToNumeralStr(result) << '\n';
		}
		else {
			*Output << Result << result << '\n';
		}
	}

	ResetIo(ts, Ifile, Ofile, counter);		//resets IO
	*Output << "Done!\n";
	Calculate(ts, vt, Romanized, Ifile, Ofile, counter);		//recursive so it doesn't trigger break; in main
}

int Factorial(int num) {			//does factorial op
	int res = 1;


	if (num == 0) return 1;


	for (int multnum = 1; multnum <= num; multnum++) {
		res *= multnum;
	}
	return res;
}
int Romanization(bool& Romanized) {		//roman numerals command
	if (!Romanized) {
		Romanized = true;
		*Output << "Now Romanized!\n";
	}
	else {
		Romanized = false;
		*Output << "Unromanized :(\n";
	}
	return 0;
}

int IOchoose(TokenStream& ts, std::ifstream& Ifile, std::ofstream& Ofile) {
	Token tk = ts.GetToken();														//gets input file name
	if (tk.Type != ts.Name_Id) throw std::runtime_error("unvalid input file name");
	std::string Ifilename = tk.StringVal;

	tk = ts.GetToken();				//searchs for parameter to for output
	if (tk.Type == ts.To_Id) {
		tk = ts.GetToken();			//gets output file name
		if (tk.Type != ts.Name_Id) throw std::runtime_error("unvalid output file name");
		std::string Ofilename = tk.StringVal;

		*Output << "are you sure you want to take input from the file: '" << Ifilename << "' and Output to the file '" << Ofilename <<"' Y to confirm\n";
		char ch;
		tk = ts.GetToken();
		if (tk.Type == End_Char) tk = ts.GetToken();					//confirmation, throws away print characters so they dont get in the way
		if (tk.StringVal == "Y" || tk.StringVal == "y") {
			Ifile.open(Ifilename);	//opens files
			Ofile.open(Ofilename);
			if (!Ifile) throw std::runtime_error("non existent input file");
			if (!Ofile) throw std::runtime_error("non existent output file");

			ts.SetIs(&Ifile);			//sets Is as pointer
			*Output << "starting!\n";
			Output = &Ofile;		//sets output as pointer

			return 0;
		}
		else return 0;
	}
	else {

		*Output << "are you sure you want to take input from the file: '" << Ifilename << "' Y to confirm\n";	//confirmation
		char ch;
		tk = ts.GetToken();
		if (tk.StringVal == "Y" || tk.StringVal == "y") {
			Ifile.open(Ifilename);	//opens input file
			if (!Ifile) throw std::runtime_error("non existent input file");

			ts.SetIs(&Ifile);		//sets Is as pointer
			*Output << "starting!\n";
			return 0;
		}
		else return 0;
	}
}

void ResetIo(TokenStream& ts, std::ifstream& Ifile, std::ofstream& Ofile, int& counter) {
	counter = 0;			//resets counter Is Output closes files and clears
	ts.SetIs(&std::cin);
	Output = &std::cout;
	Ifile.close();
	Ofile.close();
	ts.Clear();
}

int Help() {
	std::cout << "There are currently 10 commands:\n";
	std::cout << "var [name] = [expression];      creates a new variable\n";
	std::cout << "make [name] = [expression];     modifies a current variable\n";
	std::cout << "rm [name];                      removes a current variable\n";
	std::cout << "print [name];                   prints a current variable\n";
	std::cout << "list;                           lists all current variables\n";
	std::cout << "from [filename];       or       from [filename] to [filename];     takes input from a file with from and with to send output to a file\n";
	std::cout << "[number]#rad#[number];          does the [number] root of [number] can be placed inside expressions\n";
	std::cout << "numeral                         makes everything like when rome was great (you can toggle it by writing it)\n";
	std::cout << "help                            this handy command\n";
	return 0;
}

double Declaration(TokenStream& ts, VarTable& vt) {		//creates a new variable handles error handling
	Token token = ts.GetToken();
	std::string name;
	double value;

	if (token.Type != ts.Name_Id) throw std::exception("no variable name or unvalid name");
	name = token.StringVal;
	token = ts.GetToken();
	if (token.Type != '=') throw std::exception("no assignment operator '='");
	value = Expression(ts, vt);	//calculates an Expression after the = operator
	vt.NewVariable(name, value);
	*Output << "variable '" << name << "' created succesfully\n";
	return value;
}
double AssignVar(TokenStream& ts, VarTable& vt) {		//modifies a variable error handling on input format handled here error handling on variable name in class
	Token token = ts.GetToken();
	std::string name;
	double value;
	if(token.Type != ts.Name_Id) throw std::exception("unvalid identifier");
	name = token.StringVal;
	token = ts.GetToken();
	if(token.Type != '=') throw std::exception("missing assignment operator '='");
	value = Expression(ts, vt);	//calculates an Expression after the = operator
	vt.AssignVariable(name, value);
	*Output << "variable '" << name << "' assigned succesfully\n";
	return value;
}
double SearchVar(TokenStream& ts, VarTable& vt) {		//Gets a value from the variable table
	Token token = ts.GetToken();
	if (token.Type != ts.Name_Id) throw std::exception("need identifier for print function");
	return vt.GetVarValue(token.StringVal);
}
double RemoveVar(TokenStream& ts, VarTable& vt) {		//removes a variable
	Token token = ts.GetToken();
	if(token.Type != ts.Name_Id) throw std::exception("need identifier for remove function");
	vt.DeleteVariable(token.StringVal);
	*Output << "variable '" << token.StringVal << "' succesfully removed\n";
	return 0;
}
double ListVars(VarTable& vt) {		//prints all variables
	vt.PrintVariables(*Output);
	return 0;
}

double Statement(TokenStream& ts, VarTable& vt, bool& Romanized, std::ifstream& Ifile, std::ofstream& Ofile, bool& skip) {
	Token token = ts.GetToken();
	switch (token.Type)			//evaluate if token is command or expression
	{
	case ts.Declaration_Id:
		skip = true;
		return Declaration(ts, vt);
	case ts.Print_Id:
		skip = true;
		return SearchVar(ts, vt);
	case ts.Remove_Id:
		skip = true;
		return RemoveVar(ts, vt);
	case ts.Assign_Id:
		skip = true;
		return AssignVar(ts, vt);
	case ts.List_Id:
		skip = true;
		return ListVars(vt);
	case ts.Numeral_Id:
		skip = true;
		return Romanization(Romanized);
	case ts.From_Id:
		skip = true;
		return IOchoose(ts, Ifile, Ofile);
	case ts.Help_Id:
		skip = true;
		return Help();
	case ts.BadToken:	//if token is junk skips cicle and returns -1
		skip = true;
		return -1;
	default:
		ts.PutBack(token);
		return Expression(ts, vt);
	}
}
double Expression(TokenStream& ts, VarTable& vt) {
	double left = Term(ts, vt);			//evaluates left number as a term and gets next token
	Token token = ts.GetToken();

	while (true) {					//if there's an operation do it, if there isn't and the token is a number put it back in buffer and return
		switch (token.Type) {
		case '+':
			left += Term(ts, vt);
			token = ts.GetToken();
			break;
		case '-':
			left -= Term(ts, vt);
			token = ts.GetToken();
			break;
		default:
			ts.PutBack(token);
			return left;
		}
	}
}
double Term(TokenStream& ts, VarTable& vt) {
	double left = Part(ts, vt);			//same as expression() but evaluates terms(*,/, etc)
	Token token = ts.GetToken();

	while (true) {
		switch (token.Type) {
		case '*':
			left *= Part(ts, vt);
			token = ts.GetToken();
			break;
		case '/': {
			double right = Part(ts, vt);
			if (right == 0) {
				throw std::exception("can't divide by 0");
			}
			left /= right;
			token = ts.GetToken();
			break;
		}
		case '%': {
			double right = Part(ts, vt);
			int ileft = (int)left;				
			int iright = (int)right;
			if (ileft != left || iright != right) throw std::exception("non-integer remainder"); //type check
			ileft %= iright;
			left = (double)ileft;
			token = ts.GetToken();
			break;
		}
		default:
			ts.PutBack(token);
			return left;
		}
	}
}
double Part(TokenStream& ts, VarTable& vt) {						//evaluates factorials/roots and powers
	double left = Secondary(ts, vt);
	Token token = ts.GetToken();
	while (true) {
		switch (token.Type) {
		case '!': {
			int ileft = (int)left;
			if (ileft != left) throw std::exception("non-integer factorial");		//type check
			return Factorial(left);
		}
		case '^': {
			double right = Secondary(ts, vt);
			left = std::pow(left, right);
			token = ts.GetToken();
			break;
		}
		case ts.Root_Id:
		{
			double right = Secondary(ts, vt);
			double pow = 1 / left;
			left = std::pow(right, pow);
			token = ts.GetToken();
			break;
		}
		default:
			ts.PutBack(token);
			return left;
		}
	}
}
double Secondary(TokenStream& ts, VarTable& vt) {			//evaluates curly brackets '{}'
	Token token = ts.GetToken();
	while (true) {
		switch (token.Type) {
		case '{': {
			double res = Expression(ts, vt);
			token = ts.GetToken();
			if (token.Type != '}') throw std::exception("expected closure bracket: '}'");
			return res;
			}
		default:
			ts.PutBack(token);
			return Primary(ts, vt);
		}
	}
}
double Primary(TokenStream& ts, VarTable& vt) {			//evaluates parenthesis and numbers
	Token token = ts.GetToken();

	while (true) {
		switch (token.Type) {
		case '(': {
			double res = Expression(ts, vt);
			token = ts.GetToken();
			if (token.Type != ')') throw std::exception("unclosed parenthesis: ')'");
			return res;
		}
		case ts.Number_Id:
			return token.Value;
		case ts.Name_Id:
			return vt.GetVarValue(token.StringVal);
		case '-':
			return -Primary(ts, vt);
		case '+':
			return +Primary(ts, vt);
		default:
			throw std::exception("expected primary: '(' or number");
		}
	}
}




