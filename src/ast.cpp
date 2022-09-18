#include "includes/ast.hpp"


// #include <algorithm>
// #include <vector>
// #include <map>

// static std::unique_ptr<llvm::LLVMContext> TheContext;
// static std::unique_ptr<llvm::Module> TheModule;
// static std::unique_ptr<llvm::IRBuilder<>> Builder;
// static std::map<std::string, llvm::Value *> NamedValues;

// static void InitializeModule() {
//   // Open a new context and module.
//   TheContext = std::make_unique<llvm::LLVMContext>();
//   TheModule = std::make_unique<llvm::Module>("my cool jit", *TheContext);

//   // Create a new builder for the module.
//   Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
// }


// llvm::Type* get_llvm_type(std::string name) 
// {
//     if (name=="float")
//     {
//         return llvm::Type::getFloatTy(*TheContext);
//     } 
//     else if (name == "double")
//     {
//         return llvm::Type::getDoubleTy(*TheContext);
//     }
//     else if (name == "int")
//     {
//         return llvm::Type::getInt32Ty(*TheContext);
//     }
//     else if (name == "long")
//     {
//         return llvm::Type::getInt64Ty(*TheContext);
//     }
//     // else if (name == "bool")
//     // {
//     //     return llvm::Type::getB(*TheContext);
//     // }
//     else if (name == "string")
//     {
        
//     }

// }



// llvm::Function *FunctionPrototype::codegen()
// {
//     std::vector<llvm::Type*> Doubles(params.size(),
//                                 llvm::Type::getDoubleTy(*TheContext));
//     llvm::FunctionType *FT =
//         llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);

//     llvm::Function *F =
//         llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, TheModule.get());
// }


// llvm::Value *FunctionDefenition::codegen()
// {

// }
