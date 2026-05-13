CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

OBJ = src/main.o \
      src/CommandProcessor.o \
      src/Directory.o \
      src/File.o \
      src/FileSystem.o \
      src/FileSystemItem.o \
      src/FileSystemVirtual.o

all: file-system.exe

file-system.exe: ${OBJ}
	${CXX} ${CXXFLAGS} -o file-system.exe ${OBJ}

src/main.o: src/main.cpp
	${CXX} ${CXXFLAGS} -c src/main.cpp -o src/main.o

src/CommandProcessor.o: src/CommandProcessor.cpp
	${CXX} ${CXXFLAGS} -c src/CommandProcessor.cpp -o src/CommandProcessor.o

src/Directory.o: src/Directory.cpp
	${CXX} ${CXXFLAGS} -c src/Directory.cpp -o src/Directory.o

src/File.o: src/File.cpp
	${CXX} ${CXXFLAGS} -c src/File.cpp -o src/File.o

src/FileSystem.o: src/FileSystem.cpp
	${CXX} ${CXXFLAGS} -c src/FileSystem.cpp -o src/FileSystem.o

src/FileSystemItem.o: src/FileSystemItem.cpp
	${CXX} ${CXXFLAGS} -c src/FileSystemItem.cpp -o src/FileSystemItem.o

src/FileSystemVirtual.o: src/FileSystemVirtual.cpp
	${CXX} ${CXXFLAGS} -c src/FileSystemVirtual.cpp -o src/FileSystemVirtual.o

clean:
	del /f *.exe src\*.o