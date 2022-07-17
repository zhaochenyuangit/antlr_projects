#include <iostream>

#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"

using namespace antlr4;

int main(int, const char**){
  ANTLRInputStream input("(1+3)*4");
  ExprLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  
  ExprParser parser(&tokens);
  tree::ParseTree *tree = parser.prog();
  
  std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

}

