BASE_DIR=/home/sanjoy/Code/llvm

LLVM_CONFIG=$(BASE_DIR)/build/debug+asserts+all/bin/llvm-config
LLVM_CXX_LD_FLAGS := `$(LLVM_CONFIG) --cxxflags --ldflags`
LLVM_LIBS=`$(LLVM_CONFIG) --libs --system-libs`
CLANG_INCLUDES=-I$(BASE_DIR)/tools/clang/include/ -I$(BASE_DIR)/build/debug+asserts+all/tools/clang/include/
CLANG_LIBS := \
	-Wl,--start-group \
	-lclangIndex \
	-lclangCodeGen \
	-lclangToolingCore \
	-lclangFrontend \
	-lclangRewriteFrontend \
	-lclangSema \
	-lclangSerialization \
	-lclangParse \
	-lclangASTMatchers \
	-lclangAST \
	-lclangARCMigrate \
	-lclangAnalysis \
	-lclangEdit \
	-lclangRewrite \
	-lclangFrontendTool \
	-lclangDriver \
	-lclangDynamicASTMatchers \
	-lclangToolingRefactor \
	-lclangFormat \
	-lclangStaticAnalyzerCore \
	-lclangTooling \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerFrontend \
	-lclangBasic \
	-lclangLex \
	-Wl,--end-group

Rewrite: ClangRewriter.cpp Main.cpp
	clang++ ClangRewriter.cpp Main.cpp $(LLVM_CXX_LD_FLAGS) $(CLANG_INCLUDES) $(CLANG_LIBS) $(LLVM_LIBS) -o Rewrite
