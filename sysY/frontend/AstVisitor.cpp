#include "AstVisitor.hpp"
#include <any>
#include "llvm/Support/FormatVariadic.h"
#include "llvm/ADT/StringRef.h"
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
    helper.CurrTypeTok = tok;
    for (auto vardef : ctx->varDef())
        visit(vardef);

    return nullptr;
}

std::any AstVisitor::visitInitVarDef(sysYParser::InitVarDefContext *ctx)
{
    std::string id = ctx->Identifier()->getText();
    Value *v = std::any_cast<Value *>(visit(ctx->initVal()));
    Constant *const_v = dyn_cast<Constant>(v);
    if (nullptr == const_v)
    {
        errs() << "initializer must be a constant!\n";
    }
    size_t type = helper.CurrTypeTok;
    // It is a global var if declared in global scope
    if (0 == CurrScope->getDepth())
    {
        if (type == sysYParser::Int)
        {
            GlobalVariable *global_var = dyn_cast<GlobalVariable>(TheModule->getOrInsertGlobal(id, IntegerType::getInt32Ty(*TheContext)));
            // Constant* expr = BinaryConstantExpr();
            global_var->setInitializer(const_v);
            global_var->setAlignment(Align{4});
            auto entry = std::make_shared<SymbolEntry>(SymbolEntry{IntegerType::getInt32Ty(*TheContext), const_v});
            CurrScope->define(id, entry);
        }
        else if (type == sysYParser::Float)
        {
            GlobalVariable *global_var = dyn_cast<GlobalVariable>(TheModule->getOrInsertGlobal(id, Type::getFloatTy(*TheContext)));
            global_var->setInitializer(const_v);
            global_var->setAlignment(Align(4));
            auto entry = std::make_shared<SymbolEntry>(SymbolEntry{Type::getFloatTy(*TheContext), const_v});
            CurrScope->define(id, entry);
        }
    }
    return nullptr;
}

std::any AstVisitor::visitUninitVarDef(sysYParser::UninitVarDefContext *ctx)
{
    std::string id = ctx->Identifier()->getText();
    
    size_t type = helper.CurrTypeTok;
    // It is a global var if declared in global scope
    if (0 == CurrScope->getDepth())
    {
        if (type == sysYParser::Int)
        {
            GlobalVariable *global_var = dyn_cast<GlobalVariable>(TheModule->getOrInsertGlobal(id, IntegerType::getInt32Ty(*TheContext)));
            // Constant* expr = BinaryConstantExpr();
            auto zero_init = ConstantInt::get(IntegerType::getInt32Ty(*TheContext),0,false);
            global_var->setInitializer(zero_init);
            global_var->setAlignment(Align{4});
            auto entry = std::make_shared<SymbolEntry>(SymbolEntry{IntegerType::getInt32Ty(*TheContext), zero_init});
            CurrScope->define(id, entry);
        }
        else if (type == sysYParser::Float)
        {
            GlobalVariable *global_var = dyn_cast<GlobalVariable>(TheModule->getOrInsertGlobal(id, Type::getFloatTy(*TheContext)));
            auto zero_init = ConstantInt::get(IntegerType::getFloatTy(*TheContext),0,false);
            global_var->setInitializer(zero_init);
            global_var->setAlignment(Align(4));
            auto entry = std::make_shared<SymbolEntry>(SymbolEntry{Type::getFloatTy(*TheContext), zero_init});
            CurrScope->define(id, entry);
        }
    }
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