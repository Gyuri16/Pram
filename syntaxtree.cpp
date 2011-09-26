#include "syntaxtree.h"
#include "context.h"

#include <iostream>

//////////  MainNode  //////////

MainNode::MainNode(SyntaxTreeNode *blockList) {
	blocks = blockList;
}

MainNode::~MainNode() {
	delete blocks;
}

void MainNode::generateCode(Context &context) {
	blocks->generateCode(context);
}

//////////  ~MainNode~  //////////

//////////  BlockList  //////////

BlockList::~BlockList() {
	for (unsigned int i = 0; i < blocks.size(); i++) {
		delete blocks[i];
	}
}

void BlockList::addBlock(SyntaxTreeNode *block) {
	std::cout << "adding block " << std::endl;
	blocks.push_back(block);
}


void BlockList::generateCode(Context &context) {
	for (unsigned int i = 0; i < blocks.size(); i++) {
		blocks[i]->generateCode(context);
	}
}

//////////  ~BlockList~  //////////

//////////  StatementList  //////////

StatementList::~StatementList() {
	for (unsigned int i = 0; i < statements.size(); i++) {
		delete statements[i];
	}
}

void StatementList::addStatement(SyntaxTreeNode *statement) {
	statements.push_back(statement);
}

void StatementList::generateCode(Context &context) {
	for (unsigned int i = 0; i < statements.size(); i++) {
		statements[i]->generateCode(context);
	}
}

//////////  ~StatementList~  //////////

//////////  FunctionDefinition  //////////

FunctionDefinition::FunctionDefinition(std::string name, std::string type)
: name(name), type(type) {}

void FunctionDefinition::generateCode(Context &context) {
	context.addFunction(name);                               // TODO : function type
}

//////////  ~FunctionDefinition~  //////////

//////////  FunctionNode  //////////

FunctionNode::~FunctionNode() {
	std::cout << "destroy" << std::endl;
	delete definition; delete body;
}

FunctionNode::FunctionNode(SyntaxTreeNode *def, SyntaxTreeNode *statements)
: definition(def), body(statements) {}

void FunctionNode::generateCode(Context &context) {
	definition->generateCode(context);
	body->generateCode(context);
}

//////////  ~FunctionNode~  //////////

//////////  AssignmentNode  //////////

AssignmentNode::AssignmentNode(std::string idName, SyntaxTreeNode *expression)
: left(idName), right(expression) {}

AssignmentNode::~AssignmentNode() {
	delete right;
}

void AssignmentNode::generateCode(Context &context) {
	right->generateCode(context);
	addrAndNamespace addr = context.getAddress(left);
	context.addInstruction(STORE, addr.first);                       // TODO: namespaces
}

//////////  ~AssignmentNode~  //////////

//////////  WhileNode  //////////

WhileNode::WhileNode(SyntaxTreeNode *expression, SyntaxTreeNode *statement)
: expr(expression), stmnt(statement) {}

WhileNode::~WhileNode() {
	delete expr; delete stmnt;
}

void WhileNode::generateCode(Context &context) {
	int begin = context.getCurrentAddress();
	expr->generateCode(context);
	int addr = context.reserveInstruction(JMPFALSE);
	stmnt->generateCode(context);
	context.addInstruction(JMPTRUE, begin);
	context.backPatch(addr);
}

//////////  ~WhileNode~  //////////

//////////  DoWhileNode  //////////

DoWhileNode::DoWhileNode(SyntaxTreeNode *statement, SyntaxTreeNode *expression)
: stmnt(statement), expr(expression) {}

DoWhileNode::~DoWhileNode() {
	delete expr; delete stmnt;
}

void DoWhileNode::generateCode(Context &context) {
	int begin = context.getCurrentAddress();
	stmnt->generateCode(context);
	expr->generateCode(context);
	context.addInstruction(JMPTRUE, begin);
}

//////////  ~DoWhileNode~  //////////

//////////  IfThenNode  //////////

IfThenNode::IfThenNode(SyntaxTreeNode *expression, SyntaxTreeNode *then)
: expr(expression), thenStmnt(then) { }

IfThenNode::~IfThenNode() {
	delete expr; delete thenStmnt;
}

void IfThenNode::generateCode(Context &context) {
	expr->generateCode(context);
	int addr = context.reserveInstruction(JMPFALSE);
	thenStmnt->generateCode(context);
	context.backPatch(addr);

}

//////////  ~IfThenNode~  //////////

//////////  IfThenElseNode  //////////

IfThenElseNode::IfThenElseNode(SyntaxTreeNode *expression, SyntaxTreeNode *then, SyntaxTreeNode *Else)
: expr(expression), thenStmnt(then), elseStmnt(Else) { }

IfThenElseNode::~IfThenElseNode() {
	delete expr; delete thenStmnt; delete elseStmnt;
}

void IfThenElseNode::generateCode(Context &context) {
	expr->generateCode(context);
	int addr = context.reserveInstruction(JMPFALSE);
	thenStmnt->generateCode(context);

	int addr2 = context.reserveInstruction(JMP);
	context.backPatch(addr);
	elseStmnt->generateCode(context);
	context.backPatch(addr2);
}

//////////  ~IfThenElseNode~  //////////

//////////  IDexpression  //////////

IDexpression::IDexpression(std::string name)
: name(name) {}

void IDexpression::generateCode(Context &context) {
	addrAndNamespace addr = context.getAddress(name);
	context.addInstruction(LOAD, addr.first);                       // TODO: namespaces
}

//////////  ~IDexpression~  //////////

//////////  BinaryExpressionNode  //////////


BinaryExpressionNode::BinaryExpressionNode(SyntaxTreeNode *left, SyntaxTreeNode *right)
: left(left), right(right) {}

BinaryExpressionNode::~BinaryExpressionNode() {
	delete left; delete right;
}

void BinaryExpressionNode::generateCode(Context &context) {
	left->generateCode(context);
	right->generateCode(context);
}

//////////  ~BinaryExpressionNode~  //////////

//////////  AdditionNode  //////////

AdditionNode::AdditionNode(SyntaxTreeNode *left, SyntaxTreeNode *right)
: BinaryExpressionNode(left, right) {}

void AdditionNode::generateCode(Context &context) {
	BinaryExpressionNode::generateCode(context);
	context.addInstruction(ADD, noArgument);
}

//////////  ~AdditionNode~  //////////

//////////  SubtractionNode  //////////

SubtractionNode::SubtractionNode(SyntaxTreeNode *left, SyntaxTreeNode *right)
: BinaryExpressionNode(left, right) {}

void SubtractionNode::generateCode(Context &context) {
	BinaryExpressionNode::generateCode(context);
	context.addInstruction(SUB, noArgument);
}

//////////  ~SubtractionNode~  //////////

//////////  MultiplicationNode  //////////

MultiplicationNode::MultiplicationNode(SyntaxTreeNode *left, SyntaxTreeNode *right)
: BinaryExpressionNode(left, right) {}

void MultiplicationNode::generateCode(Context &context) {
	BinaryExpressionNode::generateCode(context);
	context.addInstruction(MUL, noArgument);
}

//////////  ~MultiplicationNode~  //////////

//////////  DivisionNode  //////////

DivisionNode::DivisionNode(SyntaxTreeNode *left, SyntaxTreeNode *right)
: BinaryExpressionNode(left, right) {}

void DivisionNode::generateCode(Context &context) {
	BinaryExpressionNode::generateCode(context);
	context.addInstruction(DIV, noArgument);
}

//////////  ~DivisionNode~  //////////

//////////  VariableDefinition  //////////

VariableDefinition::VariableDefinition(std::string name, std::string type)
	: name(name), type(type) {

}
void VariableDefinition::generateCode(Context &context) {
	context.addLocal(name);                                    //TODO: namespaces
}




