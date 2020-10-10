#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
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
          Value *var_retval = builder.CreateAlloca(builder.getInt32Ty());
          Value *var_i = builder.CreateAlloca(builder.getInt32Ty());
          // retval, i에 해당하는 변수 할당

          Value *value_0 = Constant::getNullValue(builder.getInt32Ty());
          StoreInst *store_inst = new StoreInst(value_0, var_i, entry_bb);
          // i 변수에 0값 넣어줌

          BranchInst::Create(original_entry_bb, entry_bb);
          BasicBlock::iterator to_remove = entry_bb->begin();
          Instruction *inst_to_remove = &(*to_remove);
          inst_to_remove->dropAllReferences();
          inst_to_remove->eraseFromParent();
          // entry_bb의 끝에 분기문을 넣어주고 원래 있던 분기문을 삭제

          BasicBlock::iterator split_point_cond = original_entry_bb->begin();
          for (int counter = 0; counter < 6; counter++) {
            split_point_cond++;
          }
          BasicBlock *cond_bb =
              original_entry_bb->splitBasicBlock(split_point_cond, "condBB");

          BasicBlock::iterator split_point_end = cond_bb->begin();
          for (int counter = 0; counter < 7; counter++) {
            split_point_end++;
          }
          BasicBlock *end_bb =
              cond_bb->splitBasicBlock(split_point_end, "endBB");

          BasicBlock::iterator split_point_body = cond_bb->begin();
          BasicBlock *body_bb =
              cond_bb->splitBasicBlock(split_point_body, "bodyBB");

          BasicBlock::iterator split_point_ret = end_bb->begin();
          BasicBlock *return_bb =
              end_bb->splitBasicBlock(split_point_ret, "returnBB");
          // condBB, bodyBB, endBB, returnBB로 구분


          entry_bb->dump();
          original_entry_bb->dump();
          functioning_bb->dump();
          return_bb->dump();

          // BinaryOperator::Create(Instruction::Add, op, rhs, "", nextInst);
          // BinaryOperator::Create(Instruction::Add, op, rhs, "", nextInst);

          // IRBuilder<> builderBody(VMBodyBB);
          // builderBody.createSwitch(pc_switch, defaultBB, bbs.size());
        }
      }
      return false;
    }
  }
};
} // namespace

char VirtualPass::ID = 0;
