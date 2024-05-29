#include "lexer.hpp"
#include "parser.tab.hpp"

int main()
{
    FooLexer lexer;
    yy::parser parser(lexer);
    return parser();
}
