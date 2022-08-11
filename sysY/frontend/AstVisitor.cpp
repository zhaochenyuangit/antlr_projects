#include "AstVisitor.hpp"

using namespace antlr4;
using namespace llvm;

std::any AstVisitor::visitCompUnit(sysYParser::CompUnitContext *ctx)
{
    LLVMModuleRef mod = LLVMModuleCreateWithName("test.ll");
    LLVMSetTarget(mod, "riscv64");

    Module *mod_ptr = unwrap(mod);
    mod_ptr->print(outs(), nullptr);
}
