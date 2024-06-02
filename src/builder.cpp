#include "builder.hpp"

unique_ptr<LLVMContext> TheContext;
unique_ptr<Module> TheModule;
unique_ptr<IRBuilder<>> Builder;
map<std::string, AllocaInst *> NamedValues;
unique_ptr<FunctionPassManager> TheFPM;
Function* MainFunc;

void init()
{
    TheContext = make_unique<LLVMContext>();
    TheModule = make_unique<Module>("LLang", *TheContext);
    Builder = make_unique<IRBuilder<>>(*TheContext);

    TheFPM = make_unique<FunctionPassManager>();
    MainFunc = CreateMainFunction();

    // Устанавливаем оптимизации
    ///
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
    BasicBlock* bb = &TheFunction->getEntryBlock();
    IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                     TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Type::getInt32Ty(*TheContext), nullptr,
                             VarName);
}

// создаёт функцию – входную точку с одним BB
Function* CreateMainFunction() {
    std::vector<Type *> Ints(0, Type::getInt32Ty(*TheContext));
    FunctionType *FT =
        FunctionType::get(Type::getInt32Ty(*TheContext), Ints, false);
    if (!FT) {
        LogErrorV("Неверный прототип у главной функции!??");
        return nullptr;
    }
    Function *Main =
        Function::Create(FT, Function::ExternalLinkage, "main", TheModule.get());
    if (!Main) {
        LogErrorV("Не получилось создать гланую функцию");
        return nullptr;
    }

    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", Main);

    if (!verifyFunction(*Main)) {
        LogErrorV("Главная функция не верифицировалась):");
        return nullptr; 
    };
    return Main;
}
