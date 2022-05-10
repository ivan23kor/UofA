SRC=test1.cc
LIB_BUILD_DIR="/home/VUOQ/build"
MYLIB="$(LIB_BUILD_DIR)/LLVMVUOQPass.so"

CLANG=clang++
OPT=opt
PASSES=--mem2reg --indvars --loop-rotate --simplifycfg --instcombine --loop-simplify
# PASSES="mem2reg,function(loop(indvars),loop(rotate),simplify-cfg,instcombine,loop-simplify)"

all: $(SRC:.cc=.ll)

$(MYLIB):
	@echo 'Building my lib as a plugin'
	@/bin/bash -c "pushd $(LIB_BUILD_DIR) >/dev/null && make >/dev/null && popd >/dev/null"
	@echo 'Built my lib plugin'

%.ll: $(MYLIB) %.cc
	$(CLANG) -S -emit-llvm $(SRC) -o $(@:.ll=_simple.ll)
	$(OPT) -S $(PASSES) $(@:.ll=_simple.ll) -o $(@:.ll=_norm.ll)
	$(OPT) -S -load $(MYLIB) --vuoqpass $(@:.ll=_norm.ll) -o $@

clean:
	$(RM) *.ll *.s a.out
