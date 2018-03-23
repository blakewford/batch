SRC := main.cpp
OPT := -m64 -Ofast -flto -march=native -funroll-loops -lpthread
STD := -std=c++17

dubitare: $(SRC)
	g++ $(OPT) $(STD) $< -o $@

clang: $(SRC)
	clang++-3.9 $(OPT) $< -o $@

java: Dubitare.java
	javac $<

profile: $(SRC)
	g++ $(OPT) $(STD) $< -o $@ -pg

eastl: $(SRC)
	g++ $(OPT) $(STD) $< -o $@ -DUSE_EASTL -IEASTL/include -IEABase/include/Common -L. -lEASTL

clean:
	-rm dubitare
	-rm profile
	-rm eastl
	-rm clang
	-rm Dubitare.class
