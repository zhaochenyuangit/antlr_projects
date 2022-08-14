#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

//#include "llvm/Analysis/Verifier.h"

#include "llvm/IR/CallingConv.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm-c/Core.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Support/Casting.h"

using namespace llvm;

LLVMModuleRef createModule()
{
  LLVMModuleRef mod = LLVMModuleCreateWithName("sum.ll");
  //LLVMSetDataLayout(mod, "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128");
  //LLVMSetTarget(mod, "x86_64-unknown-linux-gnu");

  LLVMSetDataLayout(mod,"e-m:e-p:64:64-i64:64-i128:128-n64-S128");
  LLVMSetTarget(mod,"riscv64");
  return mod;
}

void createFunction(Module *mod){
  LLVMContext &context = mod->getContext();
  FunctionType *FcType = FunctionType::get(Type::getInt32Ty(context),
  {Type::getInt32Ty(context),Type::getInt32Ty(context)},false);

  Function *funcSum = Function::Create(FcType, GlobalValue::ExternalLinkage,"sum",mod);
  funcSum->setCallingConv(CallingConv::C);

  //FunctionCallee FcCallee = mod->getOrInsertFunction("sum", FcType);
  Function::arg_iterator args = funcSum->arg_begin();
  Value *int32_a = args++;
  //int32_a->setName("a");
  Value *int32_b = args++;
  //int32_b->setName("b");

  BasicBlock *labelEntry = BasicBlock::Create(context, "entry", funcSum, 0);
  //IRBuilder<> builder(labelEntry);
  //builder.CreateAlloca();
  AllocaInst *ptrA = new AllocaInst(IntegerType::get(context,32),0, "addr.a",labelEntry);
  //ptrA->setAlignment(Align(8));
  AllocaInst *ptrB = new AllocaInst(IntegerType::get(context,32),0, "addr.b",labelEntry);
  //ptrB->setAlignment(Align(8));
  StoreInst *st0 = new StoreInst(int32_a, ptrA, false, labelEntry);
  //st0->setAlignment(Align(8));
  StoreInst *st1 = new StoreInst(int32_b, ptrB,false, Align(8), labelEntry);
  
  LoadInst *ld0 = new LoadInst(IntegerType::get(context,32), ptrA, "val_a", false, Align(8), labelEntry);
  LoadInst *ld1 = new LoadInst(IntegerType::get(context,32), ptrB, "val_b", false, Align(8), labelEntry);

  BinaryOperator *addRes = BinaryOperator::Create(Instruction::Add, ld0, ld1,"add", labelEntry);
  ReturnInst *ret = ReturnInst::Create(context, addRes,labelEntry);

  for(auto I=labelEntry->begin(), E=labelEntry->end();I!=E;++I){
      bool ret = isa<StoreInst>(I);
  }

}

int main()
{
  Module *mod = unwrap(createModule());
  createFunction(mod);
  mod->print(outs(),nullptr);
  //std::error_code EC;

  //WriteBitcodeToFile(*mod, raw_fd_ostream("sum.bc",EC));
  
  
  return 0;
}
