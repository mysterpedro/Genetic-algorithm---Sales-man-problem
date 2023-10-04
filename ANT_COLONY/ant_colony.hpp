#ifndef __ANT_COLONY_HPP__
#define __ANT_COLONY_HPP__ 
#define NUMBEROFCITIES 10 //on n'oublie pas de changer le nombre de villes ( sommets)
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string.h>
#include <random>
#include <limits.h>
#include "Sommet.hpp"
#include "Arete.hpp"
#include <iostream>

struct Solution
{
    std::vector<Sommet*> chemin ;
    float distance ;
}typedef Solution;

void displayGraph(float matrix[NUMBEROFCITIES][NUMBEROFCITIES]);

Sommet *selectionWheel(std::vector<float> probabilites, std::vector<Arete *> voisinsAchoisir);

float calculerProbabilite(Arete *arete, std::vector<Arete *> voisins, int alpha, int beta);


Solution calculerCheminParUneFourmie(std::vector<Sommet *> villes, Arete *aretes[NUMBEROFCITIES][NUMBEROFCITIES], int alpha, int beta);


Solution algorithm(std::vector<Sommet *> villes, Arete *aretes[NUMBEROFCITIES][NUMBEROFCITIES], int nbrDeTours, int nbrFourmis, int alpha, int beta, float gho);


#endif