#include "context.h"

#include <iostream>

Context::Context(SymbolTable &globals, SymbolTable &share, std::vector<CodeObject *> &codeObjects):
	globals(globals), share(share), codeObjects(codeObjects), activeCodeObject(0) {

}

void Context::addSyntaxtTreeRoot(SyntaxTreeNode *root) {
	rootNode = root;
}

void Context::generateCode() {
	rootNode->generateCode(*this);
}

int Context::addLocal(std::string name) {
	return codeObjects[activeCodeObject]->addLocal(name);
}

addrAndNamespace Context::getAddress(std::string name) {
	addrAndNamespace result;
	try {
		result = std::make_pair(codeObjects[activeCodeObject]->getLocalAddress(name), local);
	} catch (symbolTableException &e) {
		try {
			result = std::make_pair(globals.getIndex(name), global);
		} catch  (symbolTableException &e) {
			throw;
		}
	}
	return result;
}

int Context::newCodeObject(std::string name) {
	codeObjects.push_back(new CodeObject(name));
	activeCodeObject = codeObjects.size() - 1;
	return activeCodeObject;
}

int Context::addFunction(std::string name) {
	std::cout << "adding function" << std::endl;
	int address = newCodeObject(name);
	globals.addNameWithValue(name, address);
	return address;
}

void Context::addInstruction(Instructions instruction, ArgumentType argument) {
	std::cout << "adding instr" << std::endl;
	codeObjects[activeCodeObject]->addInstruction(std::make_pair(instruction, argument));
}

int Context::reserveInstruction(Instructions JMPType) {
	return codeObjects[activeCodeObject]->reserveInstruction(JMPType);
}

void Context::backPatch(int address) {
	codeObjects[activeCodeObject]->backPatch(address);
}
