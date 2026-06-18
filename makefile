CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

OBJ = obj/main.o \
      obj/CommandProcessor.o \
      obj/Directory.o \
      obj/File.o \
      obj/FileSystem.o \
      obj/FileSystemItem.o \
      obj/FileSystemVirtual.o

all: file-system.exe

file-system.exe: ${OBJ}
	${CXX} ${CXXFLAGS} -o file-system.exe ${OBJ}

obj/main.o: src/main.cpp
	${CXX} ${CXXFLAGS} -c src/main.cpp -o obj/main.o

obj/CommandProcessor.o: src/CommandProcessor.cpp
	${CXX} ${CXXFLAGS} -c src/CommandProcessor.cpp -o obj/CommandProcessor.o

obj/Directory.o: src/Directory.cpp
	${CXX} ${CXXFLAGS} -c src/Directory.cpp -o obj/Directory.o

obj/File.o: src/File.cpp
	${CXX} ${CXXFLAGS} -c src/File.cpp -o obj/File.o

obj/FileSystem.o: src/FileSystem.cpp
	${CXX} ${CXXFLAGS} -c src/FileSystem.cpp -o obj/FileSystem.o

obj/FileSystemItem.o: src/FileSystemItem.cpp
	${CXX} ${CXXFLAGS} -c src/FileSystemItem.cpp -o obj/FileSystemItem.o

obj/FileSystemVirtual.o: src/FileSystemVirtual.cpp
	${CXX} ${CXXFLAGS} -c src/FileSystemVirtual.cpp -o obj/FileSystemVirtual.o

clean:
	del /f *.exe obj\*.o