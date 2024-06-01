#include "lexer.hpp"
#include "parser.tab.hpp"
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
    yy::parser parser(true,lexer);
    return parser();
}
