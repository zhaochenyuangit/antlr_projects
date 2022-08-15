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

template<typename T>
T cast_any(T value){
  try
        {
            T v = std::any_cast<T>(value);
            return v;
        }
        catch (std::bad_any_cast e)
        {
            errs()<<"error: badcast\n";
        }
}

class AstVisitor : public sysYBaseVisitor
{
private:
  std::unique_ptr<LLVMContext> TheContext;
  std::unique_ptr<Module> TheModule;
  std::unique_ptr<IRBuilder<>> Builder;
  std::map<std::string, Value *> NamedValues;
public:
  AstVisitor(const char* source_file_name){
    TheContext = std::make_unique<LLVMContext>();
    TheModule = std::make_unique<Module>("test.ll", *TheContext);
    TheModule->setSourceFileName(source_file_name);
    TheModule->setTargetTriple("riscv64");
  }
  std::any visitCompUnit(sysYParser::CompUnitContext *ctx) override;
  std::any visitVarDecl(sysYParser::VarDeclContext *ctx) override;
  std::any visitUninitVarDef(sysYParser::UninitVarDefContext *ctx) override ;
  std::any visitInitVarDef(sysYParser::InitVarDefContext *ctx) override;
  //std::any visitScalarConstInitVal(sysYParser::ScalarConstInitValContext *ctx) override;
  //std::any visitScalarInitVal(sysYParser::ScalarInitValContext *ctx) override;
  std::any visitLVal(sysYParser::LValContext *ctx) override;
  std::any visitFloatNumber(sysYParser::FloatNumberContext *ctx) override;
  std::any visitIntNumber(sysYParser::IntNumberContext *ctx) override;
};
}

