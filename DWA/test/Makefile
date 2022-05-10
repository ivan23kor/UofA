ANALYSIS = intra-mono-fca
CLANGPLUS = clang++ -std=c++11
DEFAULT_TARGET = fca.ll

all: $(DEFAULT_TARGET)

%.ll: %.cpp
	$(CLANGPLUS) -S -emit-llvm $< -o $@
	phasar-llvm -m $@ -D $(ANALYSIS)

clean:
	$(RM) *.ll
