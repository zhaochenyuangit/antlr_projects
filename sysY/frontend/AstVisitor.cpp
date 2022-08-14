#include "AstVisitor.hpp"

using namespace antlr4;
using namespace llvm;

std::any AstVisitor::visitCompUnit(sysYParser::CompUnitContext *ctx)
{
    visitChildren(ctx);
    TheModule->print(outs(), nullptr);
    return 0;
}

std::any AstVisitor::visitVarDecl(sysYParser::VarDeclContext *ctx)
{
    for (auto vardef : ctx->varDef())
    {
        
    }
    if (ctx->bType()->type->getType() == sysYParser::Int)
    {
        outs() << "a int decl\n";
    }
    return nullptr;
}