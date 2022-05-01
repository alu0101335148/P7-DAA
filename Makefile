all:
	g++ -w -std=c++17 -o ./bin/main.exe ./src/*.cc -O3
clean:
	rm ./bin/*.exe ./bin/*.out ./bin/*.o
tar:
	tar -vczf P7_Airam_rafael_luque_leon.tar.gz *