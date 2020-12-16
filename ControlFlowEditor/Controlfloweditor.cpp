#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Cloning.h"
using namespace llvm;

namespace {
struct ControlPass : public FunctionPass {
  static char ID;
  ControlPass() : FunctionPass(ID) {}

  virtual bool runOnFunction(Function &func) {
    errs() << "- Start of function [" << func.getName() << "]\n";

    std::vector<BasicBlock*> all_basic_blocks;
    for (BasicBlock &bb : func) {
      if (bb.getName() == "if.then") { // if.then블럭에서만 난독화 진행
        BasicBlock *if_then_bb = &bb;  // BasicBlock 클래스를 사용하기 위해 바꿔줌

        BasicBlock::iterator split_point = (BasicBlock::iterator)if_then_bb->getFirstNonPHIOrDbgOrLifetime();
        BasicBlock *true_bb = if_then_bb->splitBasicBlock(split_point, "trueBB");
        //if_then_bb와 true_bb로 쪼개줌

        Value *lhs = ConstantInt::get(Type::getInt32Ty(func.getContext()), 1);
        Value *rhs = ConstantInt::get(Type::getInt32Ty(func.getContext()), 1);
        //Opaque을 위한 조건문 양 변 만들기
        // IRBuilder<> builder(if_then_bb);
        // Value *LHS = ConstantInt::get(builder.getInt32Ty(), 1);

        errs() << "BEFORE";
        if_then_bb->dump();
        ICmpInst *cond_instruction = new ICmpInst(if_then_bb->getFirstNonPHIOrDbgOrLifetime(), ICmpInst::ICMP_EQ, lhs, rhs, "newCond");
        BranchInst::Create(true_bb, false_bb, cond_instruction, if_then_bb);
        //양 변이 EQ한지를 비교하고, 이에 따른 분기문을 만들어 넣는다.

        BasicBlock::iterator to_remove = if_then_bb->begin();
        to_remove++;
        Instruction *inst_to_remove = &(*to_remove);
        inst_to_remove->dropAllReferences();
        inst_to_remove->eraseFromParent();
        //필요 없는 기존의 instruction지운다

        errs() << "AFTER";
        if_then_bb->dump();
      }
      all_basic_blocks.push_back(&bb);
      errs() << bb.getName() << "\n";
    }

      false_bb->getTerminator()->eraseFromParent();
      condition_bb->getTerminator()->eraseFromParent();

      ICmpInst *cond_instruction = new ICmpInst(condition_bb->getFirstNonPHIOrDbgOrLifetime(), ICmpInst::ICMP_EQ, lhs, rhs, "newCond");
      BranchInst::Create(true_bb, false_bb, cond_instruction, condition_bb);
      //양 변이 EQ한지를 비교하고, 이에 따른 분기문을 만들어 넣는다.

      condition_bb->dump();
      true_bb->dump();
      false_bb->dump();

      BasicBlock::iterator to_remove = condition_bb->begin();
      to_remove++;
      Instruction *inst_to_remove = &(*to_remove);
      inst_to_remove->dropAllReferences();
      inst_to_remove->eraseFromParent();
      //필요 없는 기존의 instruction지운다
    }
    return false;
  }
};
}

char ControlPass::ID = 0;
