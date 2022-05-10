#include <iostream>
#include <string>
#include <vector>

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mylooppass"

std::string toStr(bool Value) {
  return Value ? "yes" : "no";
}

namespace {
  int NumLoops = 0; // "Number of loops");
  // int NumLoopNests = 0; // "Number of loop nests");
  // int NumNormalizedLoops = 0; // "Number of normalized loops");
  // int NumPerfectLoops = 0; // "Number of perfect loops");

  class MyLoopPass: public FunctionPass {
  public:
    static char ID; // Pass identification, replacement for typeid
    // Deriving from FunctionPass for the correct order of loops
    MyLoopPass() : FunctionPass(ID) {}

    void getAnalysisUsage(AnalysisUsage &AU) const override;
    bool runOnFunction(Function &F) override;

    // Could be of depth 1 (zero inner loops)
    bool processLoopNest(const Loop *L) const;
    int getLoopLevels(const Loop *L) const;
    bool isPerfectNest(const Loop *L) const;
    bool isNormalized(const Loop *L, ScalarEvolution &SE) const;
    bool isRectangularPolyhedron(const Loop *L, ScalarEvolution &SE) const;
  };

  void MyLoopPass::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequired<ScalarEvolutionWrapperPass>();
  }

  bool MyLoopPass::runOnFunction(Function &F) {
    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

    errs() << "Function " << F.getName() << ":\n";
    for (const Loop *L: LI.getLoopsInPreorder()) {
      if (L->getParentLoop() != nullptr) {
        continue;
      }
      processLoopNest(L);
    }

    return false;
  }
} // end anonymous namespace

bool MyLoopPass::processLoopNest(const Loop *L) const {
  ScalarEvolution &SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
  errs() << "Loop " << NumLoops++ << ": ";
  errs() << "levels=" << getLoopLevels(L);
  // errs() << ", perfect nest=" << toStr(isPerfectNest(L));
  errs() << ", normalized=" << toStr(isNormalized(L, SE));
  errs() << ", rectangular polyhedron="
         << toStr(isRectangularPolyhedron(L, SE));
  errs() << "\n\n";
  return false; // haven't changed CFG
}

// Returns nestedness level of a loop, 1 for one loop
int MyLoopPass::getLoopLevels(const Loop *L) const {
  int Levels = 1;
  int MaxSubLoopLevels = 0;
  for (auto &SubLoop: L->getSubLoops()) {
      MaxSubLoopLevels = std::max(MaxSubLoopLevels, getLoopLevels(SubLoop));
  }
  return Levels + MaxSubLoopLevels;
}

// https://github.com/pmodels/bolt-llvm/blob/master/lib/Transforms/Scalar/LoopInterchange.cpp#L640
bool MyLoopPass::isPerfectNest(const Loop *L) const {
  switch (L->getSubLoops().size()) {
  case 0:
    return true;
  case 1:
    break;
  default:
    return false;
  }

  const Loop *SubLoop = L->getSubLoops()[0];

  BasicBlock *LHeader = L->getHeader();
  BasicBlock *SubLoopHeader = SubLoop->getHeader();
  BasicBlock *SubLoopPreheader = SubLoop->getLoopPreheader();
  BasicBlock *LLatch = L->getLoopLatch();

  // Check for infinite loops?
  BranchInst *LHeaderBI = dyn_cast<BranchInst>(LHeader->getTerminator());
  errs() << "HeaderBI=";
  if (LHeaderBI == nullptr) {
    errs() << "<empty>\n";
    return false;
  }
  errs() << *LHeaderBI << "\n";

  for (auto Succ: LHeaderBI->successors()) {
    errs() << "\n";
    errs() << "LHeader (" << LHeader << ")\n";
    errs() << "SubLoopPreheader (" << SubLoopPreheader << ")\n";
    errs() << "Succ (" << Succ << ")\n";
    errs() << "SubLoopHeader (" << SubLoopHeader << ")\n";
    errs() << "LLatch (" << LLatch << ")\n";
  }

  return true;
}

// Returns true if and only if all the loops in this nest are normalized:
// Induction variable starts at 0 an increments by 1
bool MyLoopPass::isNormalized(const Loop *L, ScalarEvolution &SE) const {
  if (L->getCanonicalInductionVariable() == nullptr) {
    return false;
  }
  for (auto &SubLoop: L->getSubLoops()) {
    return isNormalized(SubLoop, SE);
  }
  return true;
}

// Checks every level for the rectangularity of the induction variable,
// assumes the perfection of the nest had been checked
bool MyLoopPass::isRectangularPolyhedron(const Loop *L, ScalarEvolution &SE) const {
  if (L->getSubLoops().size() > 1) {
    errs() << "no bounds";
    return false;
  }

  Optional< Loop::LoopBounds > bounds = L->getBounds(SE);
  // errs() << "initial=" << bounds->getInitialIVValue() << "\n";
  // errs() << "final=" << bounds->getFinalIVValue() << "\n";
  // errs() << "initial_inv=" << L->isLoopInvariant(&bounds->getInitialIVValue()) << " ";
  // errs() << "final_inv=" << L->isLoopInvariant(&bounds->getFinalIVValue()) << " ";
  if (!bounds || !L->isLoopInvariant(&bounds->getFinalIVValue())) {
    return false;
  }

  if (L->getSubLoops().empty()) {
    return true;
  }
  return isRectangularPolyhedron(*L->begin(), SE);
}


char MyLoopPass::ID = 0;
static RegisterPass<MyLoopPass> X("mylooppass",
  "Loop info: depth, perfect and normalized", true, true);
