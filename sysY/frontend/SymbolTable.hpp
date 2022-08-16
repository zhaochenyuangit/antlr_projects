#include <map>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace llvm
{

class BaseScope
{
    std::map<StringRef, Type *> Symbols;
    size_t depth;
    BaseScope *Parent;

public:
    BaseScope() : BaseScope(0, nullptr){};
    BaseScope(size_t d, BaseScope *p) : depth(d), Parent(p){};
    bool define(StringRef name, Type *ty)
    {
        auto ret = Symbols.insert({name, ty});
        return /*error?*/(!ret.second);
    }
    Type* resolve(StringRef name)
    {
        if(Type* ty = Symbols[name]){
            return ty;
        }else if(nullptr!=Parent){
            return Parent->resolve(name);
        }
        return nullptr;
    }
};

}