#pragma once
#include <iostream>
#include <vector>

struct Variable {			//variable structs
	std::string Name;		//name and value of variable
	double Value;
	Variable() :			//default constructor
		Name(""), Value(0)
	{
	}
	Variable(std::string name, double value) :		//constructor
		Name(name), Value(value)
	{
	}
};


class VarTable
{
private:
	std::vector<Variable> VariableTable;	//table of all variables

public:
	void NewVariable(std::string name, double value);		//creates a new variable
	void AssignVariable(std::string name, double value);	//modifies an already ex*Isting variable
	void DeleteVariable(std::string name);	//deletes a variable
	bool IsDeclared(std::string name);		//checks if a variable *Is in VarTable
	double GetVarValue(std::string name);	//gets a variable value by the name
	int GetIndex(std::string name); //gets a variable index in the vector
	void PrintVariables(std::ostream& Output);		//prints all variables
};

