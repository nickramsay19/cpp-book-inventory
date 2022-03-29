flags = -std=c++20

default: main

main: dist build build/main.o build/Book.o build/InventoryFile.o
	g++ $(flags) -o dist/main build/main.o build/Book.o build/InventoryFile.o

build/main.o: build src/main.cpp
	g++ $(flags) -o build/main.o -c src/main.cpp

build/Book.o: build src/Book.cpp
	g++ $(flags) -o build/Book.o -c src/Book.cpp

build/InventoryFile.o: build src/InventoryFile.cpp
	g++ $(flags) -o build/InventoryFile.o -c src/InventoryFile.cpp

# clean method
clean:
	rm dist/main
	rm build/*.o

# create required dirs
dist:
	mkdir dist

build:
	mkdir build