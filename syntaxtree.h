#ifndef SYNTAXTREE_H_
#define SYNTAXTREE_H_


#include <vector>
#include <string>

class Context; // Forward declaration

class SyntaxTreeNode {
public:
	virtual ~SyntaxTreeNode() {};
	virtual void generateCode(Context &context) = 0;
};

class MainNode: public SyntaxTreeNode {
private:
	SyntaxTreeNode *blocks;
public:
	MainNode(SyntaxTreeNode *blockList);
	~MainNode();
	void generateCode(Context &context);

};

class BlockList: public SyntaxTreeNode {
private:
	std::vector<SyntaxTreeNode *> blocks;
public:
	~BlockList();
	void addBlock(SyntaxTreeNode *block);
	void generateCode(Context &context);
};

class StatementList: public SyntaxTreeNode {
private:
	std::vector<SyntaxTreeNode *> statements;
public:
	~StatementList();
	void addStatement(SyntaxTreeNode *statement);
	void generateCode(Context &context);
};

class FunctionDefinition: public SyntaxTreeNode {
private:
	std::string name;
	std::string type;
public:
	FunctionDefinition(std::string name, std::string type);
	void generateCode(Context &context);
};

class FunctionNode: public SyntaxTreeNode {
private:
	SyntaxTreeNode *definition;
	SyntaxTreeNode *body;
public:
	~FunctionNode();
	FunctionNode(SyntaxTreeNode *def, SyntaxTreeNode *statements);
	void generateCode(Context &context);
};

class AssignmentNode: public SyntaxTreeNode {
private:
	std::string left;       /* identifier */
	SyntaxTreeNode *right;  /* expression */
public:
	AssignmentNode(std::string idName, SyntaxTreeNode *expression);
	~AssignmentNode();
	void generateCode(Context &context);
};

class WhileNode: public SyntaxTreeNode {
private:
	SyntaxTreeNode *expr;
	SyntaxTreeNode *stmnt;
public:
	WhileNode(SyntaxTreeNode *expression, SyntaxTreeNode *statement);
	~WhileNode();
	void generateCode(Context &context);
};

class DoWhileNode: public SyntaxTreeNode {
private:
	SyntaxTreeNode *stmnt;
	SyntaxTreeNode *expr;
public:
	DoWhileNode(SyntaxTreeNode *statement, SyntaxTreeNode *expression);
	~DoWhileNode() ;
	void generateCode(Context &context);
};

class IDexpression: public SyntaxTreeNode {
private:
	std::string name;
public:
	IDexpression(std::string name);
	void generateCode(Context &context) ;
};

class BinaryExpressionNode: public SyntaxTreeNode {
private:
	SyntaxTreeNode *left;
	SyntaxTreeNode *right;
public:
	BinaryExpressionNode(SyntaxTreeNode *left, SyntaxTreeNode *right);
	virtual ~BinaryExpressionNode();
	void generateCode(Context &context);
};

class AdditionNode: public BinaryExpressionNode {
public:
	AdditionNode(SyntaxTreeNode *left, SyntaxTreeNode *right);
	void generateCode(Context &context);
};

class SubtractionNode: public BinaryExpressionNode {
public:
	SubtractionNode(SyntaxTreeNode *left, SyntaxTreeNode *right);
	void generateCode(Context &context);
};

class MultiplicationNode: public BinaryExpressionNode {
public:
	MultiplicationNode(SyntaxTreeNode *left, SyntaxTreeNode *right);
	void generateCode(Context &context);
};

class DivisionNode: public BinaryExpressionNode {
public:
	DivisionNode(SyntaxTreeNode *left, SyntaxTreeNode *right);
	void generateCode(Context &context);
};

class VariableDefinition: public SyntaxTreeNode {
private:
	std::string name;
	std::string type;
public:
	VariableDefinition(std::string name, std::string type);
	void generateCode(Context &context);
};

#endif /* SYNTAXTREE_H_ */
