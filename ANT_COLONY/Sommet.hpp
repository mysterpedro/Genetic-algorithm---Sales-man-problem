#ifndef __SOMMET_HPP__
#define __SOMMET_HPP__ 
#include <string>
#include <vector>

class Arete ;

class Sommet {
	protected :
	 	std::vector <Arete *> listeArete;
		int indice ;
	 	bool estVisite ;
	 	
	public :
	 	Sommet (int) ;
	 	~Sommet () ;
	 	int getIndice() {
	 		return this->indice ;
	 	}
	 	std::vector <Arete *> getNeighbors () {return this->listeArete;}
	 	bool getEstVisite(){return this->estVisite;}
	 	void setEstVisite(bool b){this->estVisite=b;}
	 	void afficherVoisins();
	 	void ajouterVoisin(Arete *);
	 	void marquer();
 };




#endif