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
          // llvm::Constant *value_4 =
          // llvm::ConstantInt::get(Type::getInt32Ty(f.getContext()), 4, true);
          // Value *var_retval = new
          // AllocaInst(Type::getInt32Ty(f.getContext()),
          //                                    NULL, value_4, "retval",
          //                                    entry_bb);
          // Value *var_i = new AllocaInst(Type::getInt32Ty(f.getContext()),
          // NULL,
          //                               value_4, "i", entry_bb);
          Value *var_retval = builder.CreateAlloca(
              Type::getInt32Ty(f.getContext()), nullptr, "retval");
          Value *var_i = builder.CreateAlloca(Type::getInt32Ty(f.getContext()),
                                              nullptr, "i");

          // retval, i에 해당하는 변수 할당

          Value *value_0 =
              Constant::getNullValue(Type::getInt32Ty(f.getContext()));
          // StoreInst *store_inst = new StoreInst(value_0, var_i, entry_bb);
          builder.CreateStore(value_0, var_i, /*isVolatile=*/false);
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

          // Value *lhs = new LoadInst(Type::getInt32Ty(f.getContext()), var_i,
          // "",
          //                           cond_bb);
          IRBuilder<> builder_cond(cond_bb);
          Value *lhs = builder_cond.CreateLoad(var_i);
          // i값을 condBB에 load
          // ICmpInst *cond_instruction =
          //     new ICmpInst(cond_bb, ICmpInst::ICMP_SGE, lhs, value_0,"");
          Value *cond_instruction =
              builder_cond.CreateICmpSGE(lhs, value_0, "");
          BranchInst::Create(body_bb, end_bb, cond_instruction, cond_bb);
          // i의 값에 따라 bodyBB와 endBB로 분기하는 분기문 생성
          BasicBlock::iterator to_remove_cond = cond_bb->begin();
          Instruction *inst_to_remove_cond = &(*to_remove_cond);
          inst_to_remove_cond->dropAllReferences();
          inst_to_remove_cond->eraseFromParent();
          // condBB의 원래 분기문을 없애면 condBB부분 완성

          IRBuilder<> builder_end(end_bb);
          builder_end.CreateStore(value_0, var_retval, /*isVolatile=*/false);
          builder_end.CreateBr(return_bb);
          BasicBlock::iterator to_remove_end = end_bb->begin();
          Instruction *inst_to_remove_end = &(*to_remove_end);
          inst_to_remove_end->dropAllReferences();
          inst_to_remove_end->eraseFromParent();
          // endBB에 0대입하는 것 넣어주기

          //---테스트 완료---
          BasicBlock::iterator split_point_switch_start = body_bb->begin();
          BasicBlock *case_bb =
              body_bb->splitBasicBlock(split_point_switch_start, "caseBB");

          IRBuilder<> builder_body(body_bb);
          Value *var_switch = builder_body.CreateLoad(var_i);
          builder_body.CreateSwitch(var_switch, case_bb, 10);
          builder_body.CreateBr(end_bb);
          BasicBlock::iterator to_remove_body = body_bb->begin();
          Instruction *inst_to_remove_body = &(*to_remove_body);
          inst_to_remove_body->dropAllReferences();
          inst_to_remove_body->eraseFromParent();
          // switch문 추가

          // entry_bb->dump();
          // original_entry_bb->dump();
          // cond_bb->dump();
          body_bb->dump();
          case_bb->dump();
          end_bb->dump();
          return_bb->dump();
          //프린트하는 부분

          // BinaryOperator::Create(Instruction::Add, op, rhs, "", nextInst);
          // BinaryOperator::Create(Instruction::Add, op, rhs, "", nextInst);
        }
      }
      return false;
    }
  }
};
} // namespace

char VirtualPass::ID = 0;
