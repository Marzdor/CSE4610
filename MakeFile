Project: Utilities.o Sdisk.o Filesys.o Shell.o Table.o main.o
	g++ Utilities.o Sdisk.o Filesys.o Shell.o Table.o main.o

Utilities.o: Utilities.cpp Utilities.h
	g++ -c Utilities.cpp
	
Sdisk.o: Sdisk.cpp Sdisk.h
	g++ -c Sdisk.cpp

Filesys.o: Filesys.cpp Filesys.h
	g++ -c Filesys.cpp

Shell.o: Shell.cpp Shell.h
	g++ -c Shell.cpp

Table.o: Table.cpp Table.h
	g++ -c Table.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o
