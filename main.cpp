#include <iostream>
#include <fstream>
#include <vector>

#include "driver.h"
#include "symboltable.h"
#include "context.h"
#include "codeobject.h"

using namespace std;

int main(int argc, char *argv[]) {
	vector<CodeObject *> *codeObjects = new vector<CodeObject *>();
	SymbolTable *globals = new SymbolTable();
	SymbolTable *share = new SymbolTable();

	Context context(*globals, *share, *codeObjects);

	example::Driver driver(context);

	for(int ai = 1; ai < argc; ++ai) {
		if (argv[ai] == std::string ("-p")) {
			driver.trace_parsing = true;
		}
		else if (argv[ai] == std::string ("-s")) {
			driver.trace_scanning = true;
		}
		else {

			std::fstream infile(argv[ai]);
			if (!infile.good())
			{
				std::cerr << "Could not open file: " << argv[ai] << std::endl;
				return 0;
			}

			bool result = driver.parse_stream(infile, argv[ai]);

			driver.context.generateCode(); // TODO: error handling
                        cout << "Done" << endl;

			if (result) {
				cout << (*codeObjects)[globals->getValue(0)]->getName() << endl;
				cout << (*codeObjects)[globals->getValue(0)]->dump();
			}

		}
	}

	return 0;

/*	CalcContext calc;
    example::Driver driver(calc);
    bool readfile = false;

    for(int ai = 1; ai < argc; ++ai)
    {
	if (argv[ai] == std::string ("-p")) {
	    driver.trace_parsing = true;
	}
	else if (argv[ai] == std::string ("-s")) {
	    driver.trace_scanning = true;
	}
	else
	{
	    // read a file with expressions

	    std::fstream infile(argv[ai]);
	    if (!infile.good())
	    {
		std::cerr << "Could not open file: " << argv[ai] << std::endl;
		return 0;
	    }

	    calc.clearExpressions();
	    bool result = driver.parse_stream(infile, argv[ai]);
	    if (result)
	    {
		std::cout << "Expressions:" << std::endl;
		for (unsigned int ei = 0; ei < calc.expressions.size(); ++ei)
		{
		    std::cout << "[" << ei << "]:" << std::endl;
		    std::cout << "tree:" << std::endl;
		    calc.expressions[ei]->print(std::cout);
		    std::cout << "evaluated: "
			      << calc.expressions[ei]->evaluate()
			      << std::endl;
		}
	    }

	    readfile = true;
	}
    }

    if (readfile) return 0;
    
    std::cout << "Reading expressions from stdin" << std::endl;

    std::string line;
    while( std::cout << "input: " &&
	   std::getline(std::cin, line) &&
	   !line.empty() )
    {
	calc.clearExpressions();
	bool result = driver.parse_string(line, "input");

	if (result)
	{
	    for (unsigned int ei = 0; ei < calc.expressions.size(); ++ei)
	    {
		std::cout << "tree:" << std::endl;
		calc.expressions[ei]->print(std::cout);
		std::cout << "evaluated: "
			  << calc.expressions[ei]->evaluate()
			  << std::endl;
	    }
	}
    }*/
}
