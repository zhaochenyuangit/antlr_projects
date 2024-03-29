#include <iostream>
#include <fstream>
#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"

#include "EvalVisitor.h"

using namespace antlr4;

int main(int argc, const char** argv){
  if(argc < 2){
    std::cout<<"no input file\n";
    return 1;
  }
  std::fstream input_file(argv[1]); 
  if(false == input_file.is_open()){
    std::cout<<"cannot read file: "<< argv[1]<<"\n";
    return 1;
  }
  ANTLRInputStream input(input_file);
  ExprLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  
  ExprParser parser(&tokens);
  tree::ParseTree *tree = parser.prog();
  
  std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

  EvalVisitor eval;
  eval.visit(tree);
  return 0;
}

