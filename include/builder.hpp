#pragma once

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <algorithm>
#include <iostream>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace llvm;
using std::make_unique;
using std::map;
using std::unique_ptr;
using std::string;

extern unique_ptr<LLVMContext> TheContext;
extern unique_ptr<Module> TheModule;
extern unique_ptr<IRBuilder<>> Builder;
extern map<std::string, AllocaInst *> NamedValues;
extern unique_ptr<FunctionPassManager> TheFPM;
extern Function* MainFunc;

void init();

void LogError(const string& msg);

Value *LogErrorV(const string& msg);

AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          const std::string &VarName);

Function* CreateMainFunction();
