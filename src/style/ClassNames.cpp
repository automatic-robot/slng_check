// SPDX-FileCopyrightText: Michael Popoloski
// SPDX-License-Identifier: MIT
#include <iostream>
#include "ASTHelperVisitors.h"
#include "TidyDiags.h"
#include "fmt/color.h"

#include "slang/syntax/AllSyntax.h"

using namespace slang;
using namespace slang::ast;

namespace classname {
using namespace std;
struct MainVisitor : public TidyVisitor, ASTVisitor<MainVisitor, true, false> {
    explicit MainVisitor(Diagnostics& diagnostics) : TidyVisitor(diagnostics) {}

      void handle(const ClassType& symbol) {
          NEEDS_SKIP_SYMBOL(symbol)
          const slang::syntax::SyntaxNode* cd =  symbol.getSyntax();
          auto kind = toString(cd->kind);
          std::cout << "KIND "<< kind << endl;


//          if(symbol.isKind(slang::ast::SymbolKind::ClassType)) {
//
//              diags.add(diag::ClassName, symbol.location);
//          }



      }


//    void handle(const ProceduralBlockSymbol& symbol) {
//        NEEDS_SKIP_SYMBOL(symbol)
//        if (symbol.procedureKind == ProceduralBlockKind::AlwaysFF) {
//            // Collect all declared local variables
//            std::vector<std::string_view> locals;
//            symbol.visitStmts(makeVisitor([&](auto&, const VariableDeclStatement& decl) {
//                locals.push_back(decl.symbol.name);
//            }));
//
//            bool hasBlockingAssignments = false;
//            symbol.visitStmts(makeVisitor([&](auto&, const AssignmentExpression& expr) {
//                if (expr.isBlocking()) {
//                    auto identifier = getIdentifier(expr.left());
//                    if (identifier && std::find(locals.begin(), locals.end(), identifier.value()) ==
//                                          locals.end()) {
//                        hasBlockingAssignments = true;
//                    }
//                }
//            }));


};
} // namespace classname

using namespace classname;
class ClassNameCheck : public TidyCheck {
public:
    [[maybe_unused]] explicit ClassNameCheck(TidyKind kind) : TidyCheck(kind) {}

    bool check(const ast::RootSymbol& root) override {
        MainVisitor visitor(diagnostics);
        root.visit(visitor);
        if (!diagnostics.empty())
            return false;
        return true;
    }

    DiagCode diagCode() const override { return diag::ClassName; }
    DiagnosticSeverity diagSeverity() const override { return DiagnosticSeverity::Warning; }
    std::string diagString() const override {
        return "use of a ClassNameCheck for a non local variables inside always_ff";
    }
    std::string name() const override { return "ClassName"; }
    std::string description() const override { return shortDescription(); }
    std::string shortDescription() const override {
        return "Enforces a Class Name check used inside always_ff "
               "for non local variables";
    }
};

REGISTER(ClassNameCheck, ClassNameCheck, TidyKind::Style)
