TEST_DIR 		= ./test
SRC_DIR 		= src
BUILD_DIR 		= build
INCLUDE_DIR 	= include
PARSER_SRC 		= ${SRC_DIR}/parser.y
LEXER_SRC 		= ${SRC_DIR}/lexer.l
LEXER_OUT 		= ${BUILD_DIR}/lex.yy.cc
PARSER_OUT_H	= ${BUILD_DIR}/parser.tab.hh
PARSER_OUT		= ${BUILD_DIR}/parser.tab.cc
PROG			= ${BUILD_DIR}/prog

lexer.l = ${SRC_DIR}/lexer.l
parser.y = ${SRC_DIR}/parser.y
lex.yy.cc = ${BUILD_DIR}/lex.yy.cc
lex.yy.o = ${BUILD_DIR}/lex.yy.o
parser.tab.cpp = ${BUILD_DIR}/parser.tab.cpp
parser.tab.hpp = ${BUILD_DIR}/parser.tab.hpp
parser.tab.o = ${BUILD_DIR}/parser.tab.o
lexer.hpp = ${INCLUDE_DIR}/lexer.hpp
parser.hpp = ${INCLUDE_DIR}/parser.hpp
main.o = ${BUILD_DIR}/main.o
main.cpp = ${SRC_DIR}/main.cpp
prog = ${BUILD_DIR}/prog

lex.yy.cc: ${lexer.l}
	flex -o ${lex.yy.cc} ${lexer.l}

lex.yy.o: lex.yy.cc ${lexer.hpp} parser.tab.hpp
	g++ -c ${lex.yy.cc} -I${INCLUDE_DIR} -I${BUILD_DIR} -o ${lex.yy.o}

parser.tab.hpp parser.tab.cpp: ${parser.y}
	bison -Wconflicts-sr -Wconflicts-rr -Wcounterexamples -H${parser.tab.hpp} -o${parser.tab.cpp} ${parser.y};

parser.tab.o: parser.tab.cpp ${lexer.hpp}
	g++ -c ${parser.tab.cpp} -I${INCLUDE_DIR} -I{BUILD_DIR} -o ${parser.tab.o}

main.o: ${main.cpp} ${lexer.hpp} parser.tab.hpp
	g++ -c ${main.cpp} -I${INCLUDE_DIR} -I${BUILD_DIR} -o ${main.o}

prog: main.o parser.tab.o lex.yy.o
	g++ ${main.o} ${parser.tab.o} ${lex.yy.o} -o ${prog}

build: mkdir prog

run: 
	./${PROG} ${I}

# make run I=test/1

crun: build run

mkdir:
	mkdir -p build

.PHONY: clean
clean:
	rm -r build
