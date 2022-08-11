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


class AstVisitor : public sysYBaseVisitor  {
public:

  virtual std::any visitCompUnit(sysYParser::CompUnitContext *ctx) override;
};
