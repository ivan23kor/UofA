#include <iostream>
#include <string>
#include <vector>

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

#define DEBUG_TYPE "mylooppass"
#define err errs() << "\033[31m"
#define dbg dbgs() << "\033[32m"
#define end "\033[0m\n"
#define TRIP_CNT_UNK -1

class VUOQPass : public FunctionPass {
public:
  static char ID;
  VUOQPass() : FunctionPass(ID) {}
  bool runOnFunction(Function &F) override;

private:
  void getAnalysisUsage(AnalysisUsage &AU) const override;
  void printLLVMUnrollPreferences(Loop *L, const LoopInfo &LI,
                                  ScalarEvolution &SE,
                                  const TargetTransformInfo &TTI) const;
};

void VUOQPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
  AU.addRequired<LoopInfoWrapperPass>();
  AU.addRequired<ScalarEvolutionWrapperPass>();
  AU.addRequired<TargetTransformInfoWrapperPass>();
}

bool VUOQPass::runOnFunction(Function &F) {
  auto &SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
  auto &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

  dbg << "Function " << F.getName() << ":" << end;
  for (auto *L : LI.getLoopsInPreorder()) {
    if (L->getParentLoop() != nullptr)
      continue;

    auto &TTI = getAnalysis<TargetTransformInfoWrapperPass>().getTTI(F);
    printLLVMUnrollPreferences(L, LI, SE, TTI);
  }

  return false;
}

void VUOQPass::printLLVMUnrollPreferences(
    Loop *L, const LoopInfo &LI, ScalarEvolution &SE,
    const TargetTransformInfo &TTI) const {
  // Get UnrollingPreferences. Necessary for reading current LLVM unrolling
  // decisions and for forcing/changing them
  TargetTransformInfo::UnrollingPreferences UP;
  TTI.getUnrollingPreferences(L, SE, UP);

  dbg << "Threshold: " << UP.Threshold << end;
  dbg << "MaxPercentThresholdBoost: " << UP.MaxPercentThresholdBoost << end;
  dbg << "OptSizeThreshold: " << UP.OptSizeThreshold << end;
  dbg << "PartialThreshold: " << UP.PartialThreshold << end;
  dbg << "PartialOptSizeThreshold: " << UP.PartialOptSizeThreshold << end;
  dbg << "Count: " << UP.Count << end;
  dbg << "DefaultUnrollRuntimeCount: " << UP.DefaultUnrollRuntimeCount << end;
  dbg << "MaxCount: " << UP.MaxCount << end;
  dbg << "FullUnrollMaxCount: " << UP.FullUnrollMaxCount << end;
  dbg << "BEInsns: " << UP.BEInsns << end;
 	dbg << "Partial: " << UP.Partial << end;
 	dbg << "Runtime: " << UP.Runtime << end;
 	dbg << "AllowRemainder: " << UP.AllowRemainder << end;
 	dbg << "AllowExpensiveTripCount: " << UP.AllowExpensiveTripCount << end;
 	dbg << "Force: " << UP.Force << end;
 	dbg << "UpperBound: " << UP.UpperBound << end;
 	dbg << "UnrollRemainder: " << UP.UnrollRemainder << end;
 	dbg << "UnrollAndJam: " << UP.UnrollAndJam << end;
  dbg << "UnrollAndJamInnerLoopThreshold: " << UP.UnrollAndJamInnerLoopThreshold << end;
  dbg << "MaxIterationsCountToAnalyze: " << UP.MaxIterationsCountToAnalyze << end;
}

// int VUOQPass::getTripCount(const Loop *L, ScalarEvolution &SE) const {
//   auto Bounds = L->getBounds(SE);
//   if (!Bounds) {
//     err << "No bounds!" << end;
//     return TRIP_CNT_UNK;
//   }
//   dbg << "Initial: " << Bounds->getInitialIVValue() << ", Final: " <<
//   Bounds->getFinalIVValue() << end;
//
//   return TRIP_CNT_UNK;
// }

} // end anonymous namespace

char VUOQPass::ID = 0;
static RegisterPass<VUOQPass>
    X("vuoqpass", "Loop info: depth, perfect and normalized", true, true);
