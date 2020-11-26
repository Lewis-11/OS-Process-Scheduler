all: mainSeq mainPar

mainSeq: main.cpp
	g++ -std=c++14 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -O3 main.cpp -o main

mainPar: parallel.cpp
	g++ -std=c++14 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -O3 -fopenmp  parallel.cpp -o parallel

clean:
	rm main 
	rm parallel
	rm *.txt