
program: main.o matrix.o solve.o
	g++ -O3 -pthread $^ -o prog
main.o: main.cpp matrix.hpp solve.hpp
	g++ -O3 -c $<
matrix.o: matrix.cpp matrix.hpp solve.hpp
	g++ -O3 -c $<
solve.o: solve.cpp matrix.hpp solve.hpp
	g++ -O3 -c $<
clean: rm -rf *.o program
