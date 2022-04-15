all:
	g++ -w -std=c++17 -o main.exe ./main.cc
clean:
	rm *.exe *.out *.o
run:
	./main.exe test/* | column -t