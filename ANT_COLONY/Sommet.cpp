#include "Sommet.hpp"
#include "Arete.hpp"

Sommet ::Sommet (  int i) : indice{i} , estVisite {false}{}
Sommet :: ~Sommet (){
}



void Sommet :: ajouterVoisin(Arete * a){
	this->listeArete.push_back(a);
}

void Sommet :: marquer(){
	this->estVisite= true;
}