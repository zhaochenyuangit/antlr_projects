#include "ExprBaseVisitor.h"

#include <cstdlib>
#include <iostream>
#include <string>

class EvalVisitor : public ExprBaseVisitor
{
    std::pair<std::string, int64_t> map{};

public:
    std::any visitPrintExpr(ExprParser::PrintExprContext *ctx){
        int64_t value = std::any_cast<int64_t>(visit(ctx->expr()));
        std::cout<<value<<"\n";
        return 0;
    }

    std::any visitInt(ExprParser::IntContext *ctx){
        std::string &&text = ctx->INT()->getSymbol()->getText();
        int64_t value = stol(text);
        return value;
    }

    std::any visitMul(ExprParser::MulContext *ctx){
        int64_t left = std::any_cast<int64_t>(visit(ctx->expr(0)));
        int64_t right = std::any_cast<int64_t>(visit(ctx->expr(1)));
        if(ctx->op->getType()==ExprParser::MUL)
        return left*right;
        return left/right;
    }
    std::any visitAdd(ExprParser::AddContext *ctx){
        int64_t left = std::any_cast<int64_t>(visit(ctx->expr(0)));
        int64_t right = std::any_cast<int64_t>(visit(ctx->expr(1)));
        if(ctx->op->getType()==ExprParser::ADD)
        return left+right;
        return left-right;
    }
};