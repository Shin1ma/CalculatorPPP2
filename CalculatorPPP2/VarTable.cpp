#include "VarTable.h"


void VarTable::NewVariable(std::string name, double value){		//creates a new variable
	if(IsDeclared(name)) throw std::exception("variable with that identifier already declared");
	VariableTable.push_back(Variable(name, value));		//pushes variable in the vector
}
void VarTable::AssignVariable(std::string name, double value){		//modifies a variable
	if (!IsDeclared(name)) throw std::runtime_error("No variable with that identifier");	//error handling and gets variable index to modify it
	int i = GetIndex(name);

	VariableTable[i].Value = value;
}
void VarTable::DeleteVariable(std::string name) {		//delets a variable
	if (!IsDeclared(name)) throw std::runtime_error("No variable with that identifier");	//error handling and get variable index and erases it
	int i = GetIndex(name);

	VariableTable.erase(VariableTable.begin() + i);
}

bool VarTable::IsDeclared(std::string name){		//checks if variable exists in VariableTable
	for (int i = 0; i < VariableTable.size(); i++) {
		if (name == VariableTable[i].Name) return true;
	}
	return false;
}
double VarTable::GetVarValue(std::string name){			//gets a variable value from a name
	if (!IsDeclared(name)) throw std::runtime_error("No variable with that identifier");
	return VariableTable[GetIndex(name)].Value;
}
int VarTable::GetIndex(std::string name) {
	for (int i = 0; i < VariableTable.size(); i++) {
		if (name == VariableTable[i].Name) return i;
	}
	throw std::runtime_error("No variable with that identifier");
}


void VarTable::PrintVariables(std::ostream& Output){					//iterates and prints all variables
	for (int i = 0; i < VariableTable.size(); i++) {
		Output << i + 1 << " " << VariableTable[i].Name << " = " << VariableTable[i].Value << " " << '\n';
	}
}