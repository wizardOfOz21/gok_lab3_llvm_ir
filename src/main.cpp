#include "parser.tab.hpp"
#include "lexer.hpp"
#include "builder.hpp"
#include "ast/program.hpp"
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

    init();
    ProgramAST* root = 0;
    yy::parser parser(root, lexer);
    parser();

    if (!root->declare()) {
        LogErrorV("Не получилось ):");
        return 0;
    };

    root->codegen();

    TheModule->print(outs(), nullptr);
    return 0;
}
