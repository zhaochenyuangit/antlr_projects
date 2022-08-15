#include "AstVisitor.hpp"
#include <any>
#include "llvm/Support/FormatVariadic.h"
#include <string>
#include <cstdlib>

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
    size_t tok = ctx->bType()->type->getType();
    for (auto vardef : ctx->varDef())
    {
        try
        {
            Value *v = std::any_cast<Value *>(visit(vardef));
            v = ConstantInt::get(IntegerType::getInt16Ty(*TheContext), 1);
        }
        catch (std::bad_any_cast e)
        {
            errs() << "error: badcast\n";
        }
    }

    return nullptr;
}

std::any AstVisitor::visitInitVarDef(sysYParser::InitVarDefContext *ctx)
{
    std::string &&id = ctx->Identifier()->getText();
    Value *v = std::any_cast<Value *>(visit(ctx->initVal()));
    Constant *const_v = dyn_cast<Constant>(v);
    if (nullptr == const_v)
    {
        errs() << "initializer must be a constant!\n";
    }

    GlobalVariable *global_var = dyn_cast<GlobalVariable>(TheModule->getOrInsertGlobal(id, IntegerType::getInt32Ty(*TheContext)));
    //Constant* expr = BinaryConstantExpr();
    if (Constant *derived = dyn_cast<Constant>(v))
    {
        global_var->setInitializer(derived);
        global_var->setAlignment(Align{4});
    }
    return v;
}

std::any AstVisitor::visitUninitVarDef(sysYParser::UninitVarDefContext *ctx)
{
    return nullptr;
}

std::any AstVisitor::visitLVal(sysYParser::LValContext *ctx)
{
    return nullptr;
}

std::any AstVisitor::visitIntNumber(sysYParser::IntNumberContext *ctx)
{
    StringRef num_str{ctx->IntConst()->getText()};
    uint8_t radix = 10;
    if (num_str.startswith("0x") || num_str.startswith("0X"))
        radix = 16;

    Value *v = ConstantInt::get(IntegerType::getInt32Ty(*TheContext), num_str, radix);
    return v;
}

std::any AstVisitor::visitFloatNumber(sysYParser::FloatNumberContext *ctx)
{
    StringRef num_str{ctx->FloatConst()->getText()};
    Value *v = ConstantFP::get(Type::getFloatTy(*TheContext), num_str);
    return v;
}