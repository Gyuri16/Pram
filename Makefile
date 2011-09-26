# $Id$
# plain simple Makefile to build exprtest

CXX = g++
LEX = flex
YACC = bison

CXXFLAGS = -W -Wall -Wextra -ansi -g 
LDFLAGS = -g

HEADERS = driver.h parser.h scanner.h  \
    y.tab.h FlexLexer.h location.hh position.hh stack.hh \
    symboltable.h context.h codeobject.h syntaxtree.h

all: pram

# Generate scanner and parser

parser.cc: parser.yy
	$(YACC) -o parser.cc --defines=parser.h --report=state parser.yy

scanner.cc: scanner.ll
	$(LEX) -o scanner.cc scanner.ll

# Implicit rule to compile c++ files

%.o: %.cc 
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
# Link executable

pram: parser.o scanner.o driver.o *.cpp
	$(CXX) $(LDFLAGS) -o $@ parser.o scanner.o driver.o  *.cpp 

clean:
	rm -f pram *.o

#extraclean: clean
	rm -f parser.cc parser.h scanner.cc
