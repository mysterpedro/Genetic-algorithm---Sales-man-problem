#include "Arete.hpp"

Arete :: Arete (Sommet *e, float d , float p ) : end {e}, distance{d}, phermone{p} {}
Arete :: ~ Arete (){
	delete end;
}

Sommet * Arete :: getEnd (){
	return this->end ;
} 

