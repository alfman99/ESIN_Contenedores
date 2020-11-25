OPCIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O0 -Wall -Wextra -Wno-sign-compare -std=c++11 -g

program.exe: driver_gesterm.o ubicacio.o contenidor.o terminal.o
	g++ -o program.exe driver_gesterm.o ubicacio.o contenidor.o terminal.o -lesin
	rm *.o

driver_gesterm.o: driver_gesterm.cpp ubicacio.hpp ubicacio.rep contenidor.hpp contenidor.rep cataleg.t cataleg.rep cataleg.hpp
	g++ -c driver_gesterm.cpp $(OPCIONS)

ubicacio.o: ubicacio.cpp ubicacio.hpp ubicacio.rep 
	g++ -c ubicacio.cpp $(OPCIONS)

contenidor.o: contenidor.cpp contenidor.hpp contenidor.rep 
	g++ -c contenidor.cpp $(OPCIONS)

terminal.o: terminal.cpp  terminal.hpp terminal.rep ubicacio.hpp ubicacio.rep contenidor.hpp contenidor.rep cataleg.hpp cataleg.rep
	g++ -c terminal.cpp $(OPCIONS)

clean:
	rm *.o
	rm *.exe
	rm *.gch
