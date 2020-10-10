#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/TypeBuilder.h"
using namespace llvm;

namespace {
struct VirtualPass : public FunctionPass {
  static char ID;
  VirtualPass() : FunctionPass(ID) {}

  virtual bool runOnFunction(Function &f) {
    errs() << "- Start of function [" << f.getName() << "]\n";

    // main 블럭이 아닌, adderObfuscated블럭에서만 난독화 진행
    if (f.getName() != "main") {
      for (BasicBlock &bb : f) {
        if (bb.getName() == "entry") { // entry bb에서 난독화를 진행하도록
          BasicBlock *entry_bb = &bb;
          // BasicBlock 클래스를 사용하기 위해 바꿔줌

          BasicBlock::iterator split_point =
              (BasicBlock::iterator)entry_bb->getFirstNonPHIOrDbgOrLifetime();
          BasicBlock *original_entry_bb =
              entry_bb->splitBasicBlock(split_point, "originEntry");
          // 할당할 변수들을 넣을 공간 생성

          IRBuilder<> builder(entry_bb);
          Value *var = builder.CreateAlloca(builder.getInt32Ty());
          Value *var2 = builder.CreateAlloca(builder.getInt32Ty());
          // retval, i에 해당하는 변수 할당
