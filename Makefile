all:
	g++ -w -std=c++17 -o ./bin/main.exe ./src/main.cc ./src/problems.cc
clean:
	rm ./bin/*.exe ./bin/*.out ./bin/*.o
run:
	./main.exe