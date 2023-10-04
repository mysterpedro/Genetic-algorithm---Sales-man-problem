#ifndef __ALGOGENETIC_HPP__
#define __ALGOGENETIC_HPP__  


#define NUMBEROFCITIES  10
#define NUMBEROFPOPULATION 50
#define OFFSET 7 // offset pour le croisement des chromosome


void displayGraph(float matrix[NUMBEROFCITIES][NUMBEROFCITIES]) ;

bool areTheSame (int *a, int*b , int size);

float sum (float *T , int size);

void swap(int *a , int*b);

void printArray ( int * T , int size) ;

void shuffleArray( int * T , int size) ;

float  calculatFitness( int *  , float graph[NUMBEROFCITIES][NUMBEROFCITIES]);

void copyArray(int *a , int *b , int size);

void createPopulation(  int ** population ) ;

int * selectParents(int ** population, float * fitness) ;

int * getGenome (int * parentA , int * parentB , int offset);

void mutateGenome(int * genome);

void algorithmeGenetique(int nbIteration);



#endif