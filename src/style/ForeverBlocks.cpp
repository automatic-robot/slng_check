// SPDX-FileCopyrightText: Michael Popoloski
// SPDX-License-Identifier: MIT
#include <iostream>
#include "ASTHelperVisitors.h"
#include "TidyDiags.h"
#include "fmt/color.h"

#include "slang/syntax/AllSyntax.h"

using namespace slang;
using namespace slang::ast;

namespace foreverblockdelay {
using namespace std;




struct MainVisitor : public TidyVisitor, ASTVisitor<MainVisitor, true, false> {
    explicit MainVisitor(Diagnostics& diagnostics) : TidyVisitor(diagnostics) {}
      void handle(const ForeverLoopStatement& symbol){

         const slang::syntax::SyntaxNode* sym_syntax = symbol.syntax;
         detect(*sym_syntax,symbol);

      }
      MainVisitor& detect(const slang::syntax::SyntaxNode& node,const ForeverLoopStatement& symbol ) {
          size_t childCount = node.getChildCount();
          for (size_t i = 0; i < childCount; i++) {
              if (auto childNode = node.childNode(i); childNode) {
                  auto mykind = childNode->kind;
                  if (mykind == slang::syntax::SyntaxKind::DelayControl) {

                      diags.add(diag::ForeverBlocksDelay, childNode->getFirstToken().location());
                  }
                  detect(*childNode, symbol);
              }
          }
          return *this;
      }
};
} // namespace foreverblockdelay

using namespace foreverblockdelay;
class ForeverBlockDelayCheck : public TidyCheck {
public:
    [[maybe_unused]] explicit ForeverBlockDelayCheck(TidyKind kind) : TidyCheck(kind) {}

    bool check(const ast::RootSymbol& root) override {
        MainVisitor visitor(diagnostics);
        root.visit(visitor);
        if (!diagnostics.empty())
            return false;
        return true;
    }

    DiagCode diagCode() const override { return diag::ForeverBlocksDelay; }
    DiagnosticSeverity diagSeverity() const override { return DiagnosticSeverity::Warning; }
    std::string diagString() const override {
        return "use of a ForeverBlockDelayCheck for a non local variables inside always_ff";
    }
    std::string name() const override { return "ForeverBlockDelay"; }
    std::string description() const override { return shortDescription(); }
    std::string shortDescription() const override {
        return "Enforces a Class Name check used inside always_ff "
               "for non local variables";
    }
};

REGISTER(ForeverBlockDelayCheck, ForeverBlockDelayCheck, TidyKind::Style)
