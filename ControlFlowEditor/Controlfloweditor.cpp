using namespace llvm;

namespace {
struct ControlPass : public FunctionPass {
  static char ID;
  ControlPass() : FunctionPass(ID) {}

  virtual bool runOnFunction(Function &func) {
    errs() << "- Start of function [" << func.getName() << "]\n";

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
