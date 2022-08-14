#include "sysYBaseVisitor.h"

#include "llvm-c/Core.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Support/Error.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"

namespace llvm{
class AstVisitor : public sysYBaseVisitor
{
private:
  LLVMContext TheContext;
  std::unique_ptr<Module> TheModule;
  std::map<std::string, Value *> NamedValues;
public:
  AstVisitor(const char* source_file_name){
    TheModule = std::make_unique<Module>("test.ll", TheContext);
    TheModule->setSourceFileName(source_file_name);
    TheModule->setTargetTriple("riscv64");
  }
  std::any visitCompUnit(sysYParser::CompUnitContext *ctx) override;
  std::any visitVarDecl(sysYParser::VarDeclContext *ctx) override;
  //std::any visitUninitVarDef(sysYParser::UninitVarDefContext *ctx) override ;
  //std::any visitInitVarDef(sysYParser::InitVarDefContext *ctx) override;
};
}

