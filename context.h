#ifndef CONTEXT_H
#define CONTEXT_H

#include "symboltable.h"
#include "codeobject.h"
#include "syntaxtree.h"


enum IDNamespace {local, global, share};
typedef std::pair<int, IDNamespace> addrAndNamespace;

class Context {
private:
	SymbolTable &globals;
	SymbolTable &share;
	std::vector<CodeObject *> &codeObjects;
	unsigned int activeCodeObject;
	SyntaxTreeNode *rootNode;

	int newCodeObject(std::string name);
public:
	Context(SymbolTable &globals, SymbolTable &share, std::vector<CodeObject *> &codeObjects);
	void addSyntaxtTreeRoot(SyntaxTreeNode *root);
	void generateCode();

	int addLocal(std::string name);

	addrAndNamespace getAddress(std::string name);

	int addFunction(std::string name);

	void addInstruction(Instructions instruction, ArgumentType argument);
	int reserveInstruction(Instructions JMPType);
	void backPatch(int address);
};

#endif
