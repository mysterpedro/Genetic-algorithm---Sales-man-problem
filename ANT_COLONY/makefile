output: ant_colony.o  Arete.o Sommet.o
	g++ ant_colony.o  Arete.o Sommet.o -o ant_colony -g
ant_colony.o: ant_colony.cpp
	g++ -c ant_colony.cpp -Wall -std=c++2a -g
Arete.o : Arete.cpp
	g++ -c Arete.cpp -Wall -std=c++2a -g
Sommet.o : Sommet.cpp
	g++ -c Sommet.cpp -Wall -std=c++2a -g
clean : 
	rm *.o a.out