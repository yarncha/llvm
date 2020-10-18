#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/TypeBuilder.h"
using namespace llvm;
using namespace std;

namespace {
struct VirtualPass : public FunctionPass {
  static char ID;
  VirtualPass() : FunctionPass(ID) {}

  // Used Functions S ----------
  BasicBlock* splitBasicBlockWithNumberAndName(BasicBlock *target_basicblock, string new_basicblock_name, int number_to_move) {
    BasicBlock::iterator split_point = (BasicBlock::iterator)target_basicblock->getFirstNonPHIOrDbgOrLifetime();
    for (int counter = 0; counter < number_to_move; counter++) {
      split_point++;
    }
    BasicBlock * splitted= target_basicblock->splitBasicBlock(split_point, new_basicblock_name);
    return splitted;
  }
  // target_basicblock -> new_basicblock 구조로 만들어짐

  void deleteInstructionWithPosition(BasicBlock *target_basicblock, int number_to_move) {
    BasicBlock::iterator to_remove = target_basicblock->begin();
    for (int counter = 0; counter < number_to_move; counter++) {
      to_remove++;
    }
    Instruction *inst_to_remove = &(*to_remove);
    inst_to_remove->dropAllReferences();
    inst_to_remove->eraseFromParent();
  }
  // Used Functions E ----------

  virtual bool runOnFunction(Function &f) {
    errs() << "- Start of function [" << f.getName() << "]\n";

    // main function이 아닌, adderObfuscated function에서만 난독화 진행
    if (f.getName() != "main") {
      for (BasicBlock &bb : f) {

        // entry bb에서 난독화를 진행하도록
        if (bb.getName() == "entry") {
          BasicBlock *entry_bb = &bb; // BasicBlock 클래스를 사용하기 위해 바꿔줌


          BasicBlock *original_entry_bb = splitBasicBlockWithNumberAndName(entry_bb,"originEntry",0);
          IRBuilder<> builder(entry_bb);
          Value *var_retval = builder.CreateAlloca(Type::getInt32Ty(f.getContext()), nullptr, "retval");
          Value *var_i = builder.CreateAlloca(Type::getInt32Ty(f.getContext()), nullptr, "i");
          // entry_bb에 retval, i에 해당하는 변수 할당
          Value *value_0 = Constant::getNullValue(Type::getInt32Ty(f.getContext()));
          builder.CreateStore(value_0, var_i, false);
          // i 변수에 0값 넣어줌
          BranchInst::Create(original_entry_bb, entry_bb);
          deleteInstructionWithPosition(entry_bb,0);
          // entry_bb 완성


          BasicBlock *cond_bb = splitBasicBlockWithNumberAndName(original_entry_bb,"condBB",6);
          BasicBlock *end_bb = splitBasicBlockWithNumberAndName(cond_bb,"endBB",8);
          BasicBlock *body_bb = splitBasicBlockWithNumberAndName(cond_bb,"bodyBB",0);
          BasicBlock *return_bb = splitBasicBlockWithNumberAndName(end_bb,"returnBB",0);
          // condBB, bodyBB, endBB, returnBB로 나누기


          IRBuilder<> builder_cond(cond_bb);
          Value *lhs = builder_cond.CreateLoad(var_i);
          // i값을 condBB에 load
          Value *cond_instruction = builder_cond.CreateICmpSGE(lhs, value_0, "");
          BranchInst::Create(body_bb, end_bb, cond_instruction, cond_bb);
          // i의 값에 따라 bodyBB와 endBB로 분기하는 분기문 생성
          deleteInstructionWithPosition(cond_bb, 0);
          // condBB의 원래 분기문을 없애면 condBB부분 완성

          IRBuilder<> builder_end(end_bb);
          builder_end.CreateStore(value_0, var_retval, false);
          // endBB에 0대입하는 것 넣어주기
          builder_end.CreateBr(return_bb);
          deleteInstructionWithPosition(end_bb, 0);
          // endBB부분 완성

          IRBuilder<> builder_ret(return_bb);
          Value *var_to_ret = builder_ret.CreateLoad(var_retval);
          Value *return_inst = builder_ret.CreateRet(var_to_ret);
          deleteInstructionWithPosition(return_bb, 0);
          // returnBB에서 retval을 리턴할 수 있도록 변경

          BasicBlock *case_bb = splitBasicBlockWithNumberAndName(body_bb,"caseBB",0);
          IRBuilder<> builder_body(body_bb);
          Value *var_switch = builder_body.CreateLoad(var_i);
          deleteInstructionWithPosition(body_bb, 0);
          //switch문을 넣을 body_bb

          BasicBlock *sw_epliog_bb = BasicBlock::Create(f.getContext(), "epliogBB", &f, end_bb);
          IRBuilder<> builder_epliog(sw_epliog_bb);
          builder_epliog.CreateBr(cond_bb);
          // 에필로그 블럭 생성

          builder_body.CreateSwitch(var_switch, sw_epliog_bb, 10);
          // 스위치문 생성

          //---테스트 완료---

          BasicBlock *case_2_bb = splitBasicBlockWithNumberAndName(case_bb,"case2BB",4);
          BasicBlock *case_3_bb = splitBasicBlockWithNumberAndName(case_2_bb,"case3BB",3);
          //case 세 개로 나누기

          //
          // BasicBlock::iterator to_remove_body = body_bb->begin();
          // Instruction *inst_to_remove_body = &(*to_remove_body);
          // inst_to_remove_body->dropAllReferences();
          // inst_to_remove_body->eraseFromParent();
          // switch문 추가

          entry_bb->dump();
          original_entry_bb->dump();
          cond_bb->dump();
          body_bb->dump();
          case_bb->dump();
          case_2_bb->dump();
          case_3_bb->dump();
          sw_epliog_bb->dump();
          end_bb->dump();
          return_bb->dump();
          //프린트하는 부분
        }
      }
      return false;
    }
  }
};
} // namespace

char VirtualPass::ID = 0;
