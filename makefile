ex3.o: ex3.cpp
	g++ ex3.cpp -o ex3.out -pthread -std=c++11

clean:
	rm -f *.o ex3.out
