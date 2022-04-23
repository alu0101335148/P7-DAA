all:
	g++ -w -std=c++17 -o ./bin/main.exe ./src/*.cc
clean:
	rm ./bin/*.exe ./bin/*.out ./bin/*.o
run:
	./main.exe