#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>
#include <vector>
#include <string>
#include <exception>

class symbolTableException: public std::exception {
private:
	const std::string str;
public:
	symbolTableException(std::string error) throw(): str(error) {}
	~symbolTableException() throw() {}

	virtual const char* what() const throw () {
		return str.c_str();
	}
};

class SymbolTable {
private:
	std::map<int,int> table;
	std::map<std::string, int> names;
	int next;
public:
	SymbolTable();
	int getValue(int index) throw(symbolTableException);
	int getIndex(std::string name) throw(symbolTableException);
	int addName(std::string name);
	int addNameWithValue(std::string name, int value);
};

#endif
