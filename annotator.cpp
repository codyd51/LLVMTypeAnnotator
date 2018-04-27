#include <string>
#include <sstream>
#include <iostream>

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
        std::string identifierForType(Type* t);
	};
}

std::string NameAnnotatorPass::identifierForType(Type* t) {
    auto typeID = t->getTypeID();
    /*
    // PrimitiveTypes - make sure LastPrimitiveTyID stays up to date.
    57     VoidTyID = 0,    ///<  0: type with no size
    58     HalfTyID,        ///<  1: 16-bit floating point type
    59     FloatTyID,       ///<  2: 32-bit floating point type
    60     DoubleTyID,      ///<  3: 64-bit floating point type
    61     X86_FP80TyID,    ///<  4: 80-bit floating point type (X87)
    62     FP128TyID,       ///<  5: 128-bit floating point type (112-bit mantissa)
    63     PPC_FP128TyID,   ///<  6: 128-bit floating point type (two 64-bits, PowerPC)
    64     LabelTyID,       ///<  7: Labels
    65     MetadataTyID,    ///<  8: Metadata
    66     X86_MMXTyID,     ///<  9: MMX vectors (64 bits, X86 specific)
    67     TokenTyID,       ///< 10: Tokens
    68
    69     // Derived types... see DerivedTypes.h file.
    70     // Make sure FirstDerivedTyID stays up to date!
    71     IntegerTyID,     ///< 11: Arbitrary bit width integers
    72     FunctionTyID,    ///< 12: Functions
    73     StructTyID,      ///< 13: Structures
    74     ArrayTyID,       ///< 14: Arrays
    75     PointerTyID,     ///< 15: Pointers
    76     VectorTyID       ///< 16: SIMD 'packed' format, or other vector type
    */
    std::map<Type::TypeID, std::string> type_identifiers = {
        {Type::TypeID::VoidTyID, "v"},
        {Type::TypeID::HalfTyID, "f"},
        {Type::TypeID::FloatTyID, "f"},
        {Type::TypeID::DoubleTyID, "f"},
        {Type::TypeID::X86_FP80TyID, "f"},
        {Type::TypeID::FP128TyID, "f"},
        {Type::TypeID::PPC_FP128TyID, "f"},
        {Type::TypeID::LabelTyID, "C"},
        {Type::TypeID::MetadataTyID, "X"},
        {Type::TypeID::X86_MMXTyID, "M"},
        {Type::TypeID::TokenTyID, "t"},
        //derived types
        {Type::TypeID::IntegerTyID, "i"},
        {Type::TypeID::FunctionTyID, "F"},
        {Type::TypeID::StructTyID, "S"},
        {Type::TypeID::ArrayTyID, "A"},
        {Type::TypeID::PointerTyID, "p"},
        {Type::TypeID::VectorTyID, "A"},
    };
    return type_identifiers[typeID];
}

bool NameAnnotatorPass::runOnFunction(Function &F) {
    std::stringstream ss;
	auto name = F.getName();
    //don't modify main symbol
    if (name == "main") {
        return false;
    }

    ss << name.str() << "__returns_" << identifierForType(F.getReturnType()) << "__args_";

    std::vector<Argument*> functionArguments;
    for (auto A = F.arg_begin(); A != F.arg_end(); A++) {
        functionArguments.push_back(A);
    }

    for (auto A  = std::begin(functionArguments); A != std::end(functionArguments); A++) {
        auto arg = *A;
        auto typeIdentifier = identifierForType(arg->getType());
        ss << typeIdentifier;
    }

    auto annotatedName = ss.str();
    F.setName(Twine(annotatedName));
    errs() << "type-annotator: " << name << " -> " << annotatedName << "\n";

	return true;
}

char NameAnnotatorPass::ID = 0;
static RegisterPass<NameAnnotatorPass> X("typeAnnotator", "Mangle each symbol name to include the type information",
		false /* Only looks at CFG */,
		false /* Analysis Pass */);
