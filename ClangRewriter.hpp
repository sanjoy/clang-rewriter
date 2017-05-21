#ifndef CLANG_REWRITER_H
#define CLANG_REWRITER_H

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include <functional>

#define OPEN_REWRITER_NAMESPACE                                                \
  using namespace clang;                                                       \
  using namespace clang::ast_matchers;                                         \
  using namespace clang::tooling;                                              \
  using namespace llvm

struct MatcherSet {
  clang::ast_matchers::MatchFinder &MF;
  clang::ast_matchers::MatchFinder::MatchCallback &MC;

  MatcherSet(clang::ast_matchers::MatchFinder &MF,
             clang::ast_matchers::MatchFinder::MatchCallback &MC)
      : MF(MF), MC(MC) {}

#define DEFINE_ADD_MATCHER(TYPE)                                               \
  void add(const clang::ast_matchers::TYPE##Matcher &NodeMatch) {              \
    MF.addMatcher(NodeMatch, &MC);                                             \
  }

  DEFINE_ADD_MATCHER(Declaration)
  DEFINE_ADD_MATCHER(Type)
  DEFINE_ADD_MATCHER(Statement)
  DEFINE_ADD_MATCHER(NestedNameSpecifier)
  DEFINE_ADD_MATCHER(NestedNameSpecifierLoc)
  DEFINE_ADD_MATCHER(TypeLoc)
  DEFINE_ADD_MATCHER(CXXCtorInitializer)
};

typedef std::function<void(MatcherSet &)> InitMatcherTy;

typedef std::function<void(
    const clang::ast_matchers::MatchFinder::MatchResult &, clang::Rewriter &)>
    MatchHandlerTy;

int Run(clang::tooling::ClangTool &Tool, InitMatcherTy InitMatcher,
        MatchHandlerTy MatchHandler);

#endif
