#include "symboltable.h"

SymbolTable::SymbolTable(): next(0) {}

int SymbolTable::getValue(int index) throw(symbolTableException) {
	if (index >= next) {
		throw symbolTableException("ID not defined");
	}
	return table[index];
}

int SymbolTable::getIndex(std::string name) throw(symbolTableException) {
	std::map<std::string, int>::iterator it = names.find(name);
	if (it == names.end()) {
		throw symbolTableException("Name not defined");
	}
	return it->second;
}

int SymbolTable::addName(std::string name) {
	return addNameWithValue(name, 0);
}

int SymbolTable::addNameWithValue(std::string name, int value) {
	//TODO: add check for already defined names
	names[name] = next;
	table[next] = value;
	return next++;
}
