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
        std::map<std::string, std::shared_ptr<SymbolEntry>> Symbols;
        size_t depth;
        BaseScope *Parent;

    public:
        BaseScope() : BaseScope(0, nullptr) {}
        BaseScope(size_t d, BaseScope *p) : depth(d), Parent(p) {}
        bool define(std::string name, std::shared_ptr<SymbolEntry> entry)
        {
            auto ret = Symbols.insert({name, entry});
            return /*error?*/ (!ret.second);
        }
        SymbolEntry *resolve(std::string name)
        {
            auto it = Symbols.find(name);
            if (it != Symbols.end()){
                return it->second.get();
            }
            else if (nullptr != Parent)
            {
                return Parent->resolve(name);
            }
            return nullptr;
        }
        BaseScope *getParent() { return this->Parent; }
        size_t getDepth() { return this->depth; }
        void print(){
            for(auto entry: Symbols){
                outs()<<entry.first<<" "<<entry.second->ty->getTypeID()<<"\n";
            }
        }
    };

}