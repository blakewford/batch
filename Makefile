SRC := main.cpp
OPT := -m64 -Ofast -flto -march=native -funroll-loops -fopenmp
STD := -std=c++17

dubitare: $(SRC)
	g++ $(STD) $< -o $@ $(OPT)

clean:
	-rm dubitare
