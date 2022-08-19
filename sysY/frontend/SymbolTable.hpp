#include <map>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace llvm
{
    struct SymbolEntry
    {
        Type *ty;
        Constant *init_value;
    };

    class BaseScope
    {
        std::map<std::string, std::shared_ptr<SymbolEntry>> VarSymbols;
        std::map<std::string, std::shared_ptr<SymbolEntry>> FuncSymbols;
        size_t depth;
        std::shared_ptr<BaseScope> Parent;

    public:
        BaseScope() : BaseScope(0, nullptr) {}
        BaseScope(size_t d, std::shared_ptr<BaseScope> p) : depth(d), Parent(p) {}

        std::shared_ptr<BaseScope> getParent() { return this->Parent; }
        size_t getDepth() { return this->depth; }

        bool define(std::string name, Type *ty, Constant *init_v)
        {
            auto entry = std::make_shared<SymbolEntry>(SymbolEntry{ty, init_v});
            if (auto type = dyn_cast<FunctionType>(ty))
            {
                auto ret = FuncSymbols.insert({name, entry});
                return !(ret.second);
            }
            // else it's a variable
            auto ret = VarSymbols.insert({name, entry});
            return /*error?*/ !(ret.second);
        }
        bool define(std::string name, Type *ty)
        {
            auto entry = std::make_shared<SymbolEntry>(SymbolEntry{ty, nullptr});
            if (auto type = dyn_cast<FunctionType>(ty))
            {
                auto ret = FuncSymbols.insert({name, entry});
                return !(ret.second);
            }
            // else it's a variable
            auto ret = VarSymbols.insert({name, entry});
            return /*error?*/ !(ret.second);
        }

        SymbolEntry *resolveVar(std::string name)
        {
            auto it = VarSymbols.find(name);
            if (it != VarSymbols.end())
            {
                return it->second.get();
            }
            else if (nullptr != Parent)
            {
                return Parent->resolveVar(name);
            }
            return nullptr;
        }
        Type *resolveFunc(std::string name)
        {
            auto it = FuncSymbols.find(name);
            if (it != FuncSymbols.end())
            {
                return it->second.get()->ty;
            }
            else if (nullptr != Parent)
            {
                return Parent->resolveFunc(name);
            }
            return nullptr;
        }
        
        void print()
        {
            outs() << "Variables:\n";
            for (auto entry : VarSymbols)
            {
                outs() << entry.first << " " << entry.second->ty->getTypeID() << "\n";
            }
            outs() << "Functions:\n";
            for (auto entry : VarSymbols)
            {
                outs() << entry.first << " " << entry.second->ty->getTypeID() << "\n";
            }
        }
    };

}