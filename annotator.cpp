#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
	struct NameAnnotatorPass : public FunctionPass {
		static char ID;
		NameAnnotatorPass() : FunctionPass(ID) {}
        bool runOnFunction(Function &F) override;
        void printIntValue(Value* intValue);
	};
}

void NameAnnotatorPass::printIntValue(Value* intValue) {
    if (ConstantInt* CI = dyn_cast<ConstantInt>(intValue)) {
        if (CI->getBitWidth() <= 64) {
            int constIntValue = CI->getSExtValue();
            errs() << constIntValue << ".";
        }
    }
    else {
        // foo was not actually a ConstantInt
        errs() << "(not constant).";
        intValue->dump();
    }
}

bool NameAnnotatorPass::runOnFunction(Function &F) {
	auto name = F.getName();
    auto annotatedName = name;

    std::vector<Argument*> functionArguments;
    for (auto A = F.arg_begin(); A != F.arg_end(); A++) {
        functionArguments.push_back(A);
    }

    for (auto A  = std::begin(functionArguments); A != std::end(functionArguments); A++) {
        auto arg = *A;
        arg->dump();
    }

    /*
	auto name = StringRef(origName + "_but_unironically");
	F.setName(Twine(name));
    */
    errs() << "type-annotator: " << name << " -> " << annotatedName << "\n";
    F.setName(Twine(annotatedName));

	return true;
}

char NameAnnotatorPass::ID = 0;
static RegisterPass<NameAnnotatorPass> X("typeAnnotator", "Mangle each symbol name to include the type information",
		false /* Only looks at CFG */,
		false /* Analysis Pass */);
