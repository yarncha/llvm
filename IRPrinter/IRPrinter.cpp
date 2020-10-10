using namespace llvm;

namespace {
  struct PrinterPass : public FunctionPass {
    static char ID;
    PrinterPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &f) {
      errs() << "- Start of function [" << f.getName() << "]\n";

      for (BasicBlock &bb : f){
        errs() << "- Start of Basicblock ["<< bb.getName() << "], num of instructions ["
                   << bb.size() << "] instructions.\n";

        for (Instruction &i : bb)
          errs() << "- Instruction : " << i << "\n";
        for (BasicBlock::iterator it = bb->begin(); it != bb->end(); it++) {
          Instruction* i = &(*it);
          errs() << *i << "\n";
        }
      }

      return false;
    }
  };
}

char PrinterPass::ID = 0;
