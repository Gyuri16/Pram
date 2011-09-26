#ifndef CODEOBJECT_H_
#define CODEOBJECT_H_

#include "symboltable.h"

enum Instructions {RET, READ_LOC, READ_GLOB, WRITE, CALL, LOAD, JMP, JMPFALSE, STORE, ADD, SUB, MUL, DIV};

typedef unsigned int ArgumentType;
typedef std::pair<Instructions, ArgumentType> InstrPair;
const int noArgument = 0;

class CodeObject {
private:
	std::string name;
	SymbolTable locals;
	std::vector<InstrPair> code;
public:
	CodeObject(std::string name);
	std::string getName();

	void addInstruction(InstrPair instruction);
	int reserveInstruction(Instructions JMPType);
	void backPatch(int address);

	int addLocal(std::string name) throw(symbolTableException);
	int getLocalAddress(std::string name) throw(symbolTableException);

	std::string dump();
};


#endif /* CODEOBJECT_H_ */
