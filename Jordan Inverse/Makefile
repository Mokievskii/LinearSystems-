program: main.o matrix.o solve.o
	g++ $^ -o prog
main.o: main.cpp matrix.hpp solve.hpp
	g++ -c $<
matrix.o: matrix.cpp matrix.hpp solve.hpp
	g++ -c $<
solve.o: solve.cpp matrix.hpp solve.hpp
	g++ -c $<
