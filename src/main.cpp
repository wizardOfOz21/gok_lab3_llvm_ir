#include "parser.tab.hpp"
#include "lexer.hpp"
#include "ast.hpp"
#include <string>
#include <fstream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Введите имя файла с входными данными" << std::endl;
        return 0;
    }

    std::string name(argv[1]);
    std::fstream input_stream(name);
    FooLexer lexer(input_stream, 1);

    ProgramAST* root = 0;
    yy::parser parser(root, lexer);
    parser();

    // * op = (ExprAST)((FuncAST*)(root->decls[0]))->body[0];

    std::cout << root << std::endl;
    return 0;
}
