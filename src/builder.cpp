#include "builder.hpp"

unique_ptr<LLVMContext> TheContext;
unique_ptr<Module> TheModule;
unique_ptr<IRBuilder<>> Builder;
map<std::string, AllocaInst *> NamedValues;
unique_ptr<FunctionPassManager> TheFPM;
unique_ptr<LoopAnalysisManager> TheLAM;
unique_ptr<FunctionAnalysisManager> TheFAM;
unique_ptr<CGSCCAnalysisManager> TheCGAM;
unique_ptr<ModuleAnalysisManager> TheMAM;

Function *MainFunc;
string entry_function_name = "";
string main_func_name = "forty_two";

void init()
{
    TheContext = make_unique<LLVMContext>();
    TheModule = make_unique<Module>("LLang", *TheContext);
    Builder = make_unique<IRBuilder<>>(*TheContext);
    TheFPM = std::make_unique<FunctionPassManager>();
    TheLAM = std::make_unique<LoopAnalysisManager>();
    TheFAM = std::make_unique<FunctionAnalysisManager>();
    TheCGAM = std::make_unique<CGSCCAnalysisManager>();
    TheMAM = std::make_unique<ModuleAnalysisManager>();
    MainFunc = CreateMainFunction();

    // Устанавливаем оптимизации
    // Promote allocas to registers.
    // Add transform passes.
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->addPass(InstCombinePass());
    // Reassociate expressions.
    TheFPM->addPass(ReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->addPass(GVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->addPass(SimplifyCFGPass());
    ///

    PassBuilder PB;
    PB.registerModuleAnalyses(*TheMAM);
    PB.registerFunctionAnalyses(*TheFAM);
    PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
}

void LogError(const string &msg)
{
    std::cout << msg << std::endl;
}

Value *LogErrorV(const string &msg)
{
    LogError(msg);
    return nullptr;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                   const std::string &VarName)
{
    BasicBlock *bb = &TheFunction->getEntryBlock();
    IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                     TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Type::getInt32Ty(*TheContext), nullptr,
                             VarName);
}

// создаёт функцию – входную точку с одним BB
Function *CreateMainFunction()
{
    std::vector<Type *> Ints(0, Type::getInt32Ty(*TheContext));
    FunctionType *FT =
        FunctionType::get(Type::getInt32Ty(*TheContext), Ints, false);
    if (!FT)
    {
        LogErrorV("Неверный прототип у главной функции!??");
        return nullptr;
    }
    Function *Main =
        Function::Create(FT, Function::ExternalLinkage, main_func_name, TheModule.get());
    if (!Main)
    {
        LogErrorV("Не получилось создать гланую функцию");
        return nullptr;
    }

    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", Main);

    if (!verifyFunction(*Main))
    {
        LogErrorV("Главная функция не верифицировалась):");
        return nullptr;
    };
    return Main;
}

void in_func_name_print()
{
    auto func_name = Builder->GetInsertBlock()->getParent()->getName();
    std::cout << "В функции " << func_name.str() << std::endl;
}
