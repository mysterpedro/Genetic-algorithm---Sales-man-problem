#ifndef __ARETE_HPP__
#define __ARETE_HPP__ 
#include "Sommet.hpp"
class Arete{
	private:
		Sommet * end ;
		float distance;
		float phermone;
	public :
	Arete ( Sommet * , float,float) ;
	~Arete ();
	Sommet * getEnd() ;
	float getDistance(){return this->distance;}
	void setDistance(float d){this->distance=d;}
	float getPhermone(){return this->phermone;}
	void setPhermone(float p) {this->phermone=p;}

};





#endif