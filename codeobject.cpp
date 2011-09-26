#include "codeobject.h"

#include <sstream>

CodeObject::CodeObject(std::string name): name(name) {}

std::string CodeObject::getName() {
	return name;
}

void CodeObject::addInstruction(InstrPair instruction) {
	code.push_back(instruction);
}

int CodeObject::reserveInstruction(Instructions JMPType) {
	code.push_back(std::make_pair(JMPType, 0));
	return code.size() - 1;
}

void CodeObject::backPatch(int address) {
	code[address].second = code.size();
}

int CodeObject::addLocal(std::string name) throw(symbolTableException) {
	return locals.addName(name);
}

int CodeObject::getLocalAddress(std::string name) throw(symbolTableException) {
	return locals.getIndex(name);
}

std::string CodeObject::dump() { // TODO
	std::ostringstream result;
	const char *opNames[] = {"RET", "READ_LOC", "READ_GLOB", "WRITE", "CALL", "LOAD", "JMP", "JMPFALSE", "STORE", "ADD", "SUB", "MUL", "DIV"};
	for (unsigned int i = 0; i < code.size(); i++) {
		result << opNames[code[i].first]
				<< " " << code[i].second << std::endl;
	}
	return result.str();
}
