#include "ClangRewriter.hpp"

OPEN_REWRITER_NAMESPACE;

struct MyHandler : public MatchFinder::MatchCallback {
  MyHandler(Rewriter &R, MatchHandlerTy &MH) : R(R), MH(MH) {}
  virtual void run(const MatchFinder::MatchResult &Result) { MH(Result, R); }
  Rewriter &R;
  MatchHandlerTy MH;
};

struct MyASTConsumer : public ASTConsumer {
  MatchFinder Matcher;
  MyHandler Handler;

  MyASTConsumer(Rewriter &R, InitMatcherTy &InitMatcher,
                MatchHandlerTy &MatchHandler)
      : Handler(R, MatchHandler) {
    MatcherSet MS(Matcher, Handler);
    InitMatcher(MS);
  }

  void HandleTranslationUnit(ASTContext &Context) override {
    Matcher.matchAST(Context);
  }
};

struct MyFrontendAction : public clang::ASTFrontendAction {
  Rewriter &R;
  InitMatcherTy &InitMatcher;
  MatchHandlerTy &MatchHandler;

  MyFrontendAction(Rewriter &R, InitMatcherTy &InitMatcher,
                   MatchHandlerTy &MatchHandler)
      : R(R), InitMatcher(InitMatcher), MatchHandler(MatchHandler) {}

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    R.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return llvm::make_unique<MyASTConsumer>(R, InitMatcher, MatchHandler);
  }

  void EndSourceFileAction() override {
    R.overwriteChangedFiles();
  }
};

struct MyFrontendActionFactory : public FrontendActionFactory {
  Rewriter &R;
  InitMatcherTy &InitMatcher;
  MatchHandlerTy &MatchHandler;

  MyFrontendActionFactory(Rewriter &R, InitMatcherTy &InitMatcher,
                          MatchHandlerTy &MatchHandler)
      : R(R), InitMatcher(InitMatcher), MatchHandler(MatchHandler) {}

  FrontendAction *create() override {
    return new MyFrontendAction(R, InitMatcher, MatchHandler);
  }
};

int Run(ClangTool &Tool, InitMatcherTy InitMatcher,
         MatchHandlerTy MatchHandler) {
  Rewriter R;
  auto Factory =
      make_unique<MyFrontendActionFactory>(R, InitMatcher, MatchHandler);
  return Tool.run(Factory.get());
}
