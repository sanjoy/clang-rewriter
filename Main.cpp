#include "ClangRewriter.hpp"

OPEN_REWRITER_NAMESPACE;

static void InitMatcher(MatcherSet &MS) {
  // Create your matchers here.
  auto Inner =
      cxxMemberCallExpr(callee(cxxMethodDecl(hasName("getUnsignedRange"))),
                        hasArgument(0, expr().bind("inner")));

  MS.add(cxxMemberCallExpr(callee(cxxMethodDecl(hasName("getUnsignedMax"))),
                           on(cxxBindTemporaryExpr(has(Inner))))
             .bind("outer"));
}

static void MatchHandler(const MatchFinder::MatchResult &Result, Rewriter &R) {
  // Create your rewriter here.
  auto *Outer = Result.Nodes.getNodeAs<clang::Expr>("outer");
  auto *Inner = Result.Nodes.getNodeAs<clang::Expr>("inner");
  std::string InnerName;
  {
    raw_string_ostream OS(InnerName);
    Inner->printPretty(OS, nullptr, Result.Context->getPrintingPolicy());
  }
  R.ReplaceText(SourceRange(Outer->getLocStart(), Outer->getLocEnd()),
                "getUnsignedMax(" + InnerName + ")");
}

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, cl::GeneralCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  return Run(Tool, InitMatcher, MatchHandler);
}
