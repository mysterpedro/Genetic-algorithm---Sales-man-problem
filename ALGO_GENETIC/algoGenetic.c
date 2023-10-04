#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>
#include "algoGenetic.hpp"

//Fonction intermédiaire 
///////////////////////////////////////////////////////////////////
//afficher la matrice
void displayGraph(float matrix[NUMBEROFCITIES][NUMBEROFCITIES])
{
	printf("\t");
	for (int j = 0; j < NUMBEROFCITIES; j++)
	{
		printf("[%d]\t", j);
	}
	printf("\n");

	for (int i = 0; i < NUMBEROFCITIES; i++)
	{
		printf("[%d]\t", i);
		for (int j = 0; j < NUMBEROFCITIES; j++)
		{
			printf("%.2f\t", matrix[i][j]);
		}
		printf("\n");
	}
}




//comparer deux tableaux
bool areTheSame(int *a, int *b, int size)
{
	for (int i = 0; i < size; i++)
		if (a[i] != b[i])
			return false;
	return true;
}

//calculer la somme

float sum(float *T, int size)
{
	float sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += T[i];
	}
	return sum;
}


void swap(int *a, int *b)
{
	int x = (*a);
	*a = *b;
	*b = x;
}

void printArray(int *T, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		printf("%d -> ", T[i]);
	}

	printf(" -> %d \n", T[size - 1]);
}

// modifier les position d'un tableau d'un manière aléatoire
void shuffleArray(int *T, int size)
{
	int tmp;
	// changer l'ordre des villes aléatoirement
	for (int i = 0; i < size; i++)
	{
		tmp = rand() % (size - 1) + 1;
		swap(&T[i], &T[tmp]);
	}
}

void copyArray(int *a, int *b, int size)
{
	for (int i = 0; i < size; i++)
	{
		a[i] = b[i];
	}
}

///////////////////////////////////////////////////////////////////

//calculer la fonction f(n)
float calculatFitness(int *chromosome, float graph[NUMBEROFCITIES][NUMBEROFCITIES])
{
	float distance = 0;
	float sum = 0;
	// calculer la distance de chaque chromosome

	for (int j = 0; j < NUMBEROFCITIES; j++)
	{
		distance = graph[chromosome[j]][chromosome[(j + 1) % NUMBEROFCITIES]]; // chercher les distance dans la matrice graph, chromosome[j] represente la ville courante, chromosome[j+1] représente la ville suivante, le modulo pour revenir à la ville de départ
		sum += distance;
	}

	return sum;
}




//générer la population
void createPopulation(int **population)
{
	int cities[NUMBEROFCITIES];

	// les numeros des villes [0,1,2,3,..., n]
	for (int i = 0; i < NUMBEROFCITIES; i++)
	{
		cities[i] = i;
	}

	// création d'un population aléatoire
	for (int i = 0; i < NUMBEROFPOPULATION; i++)
	{
		shuffleArray(cities, NUMBEROFCITIES);
		copyArray(population[i], cities, NUMBEROFCITIES);
	}
}

// fonction renvoie l'index des parents dans le tableau population

int *selectParents(int **population, float *fitness)
{
	int *parents = (int *)malloc(2 * sizeof(int));
	float cumulativeProbs[NUMBEROFPOPULATION];
	float total = 0;
	float randValue = 0.0f; // random value

	float sumFitness = sum(fitness, NUMBEROFPOPULATION);

	for (int i = 0; i < NUMBEROFPOPULATION; i++)
	{
		total += fitness[i];
		cumulativeProbs[i] = total / sumFitness;
	}

	// choix des parents
	do
	{	
		for (int j = 0; j < 2; j++)
		{
			randValue = (float)rand() / RAND_MAX;
			for (int i = 0; i < NUMBEROFPOPULATION; i++)
			{
				if (randValue < cumulativeProbs[i])
				{
					parents[j] = i;
					break;
				}
			}
		}
	} while (areTheSame(population[parents[0]], population[parents[1]], NUMBEROFCITIES)); // verifier que les parents ne sont pas les mêmes

	return parents;
}

// the crossover function
int *getGenome(int *parentA, int *parentB, int offset)
{
	int *genome = (int *)malloc(sizeof(int) * NUMBEROFCITIES);
	int i;
	bool isHere = false;

	for (i = 0; i < offset; i++)
	{
		genome[i] = parentA[i];
	}

	for (int j = 0; j < NUMBEROFCITIES; j++, isHere = false)
	{

		for (int k = 0; k < offset; k++)
		{
			if (parentB[j] == parentA[k])
			{
				isHere = true;
				break;
			}
		}

		if (!isHere)
		{
			genome[i++] = parentB[j];
		}
	}
	return genome;
}

void mutateGenome(int *genome)
{
	int i = rand() % NUMBEROFCITIES;
	int j = 0;

	do
	{
		j = rand() % NUMBEROFCITIES;
	} while (i == j);

	swap(&genome[i], &genome[j]);
}

void algorithmeGenetique(int nbIteration)
{
	srand(time(0));
	float optimalValue = INT_MAX;
	float sumFitness = 0;
	int *optimalPath = (int *)malloc(NUMBEROFCITIES * sizeof(int));
	int *parents = NULL;
	int *genome = NULL;
	float randValue = 0;
	float fitnessValues[NUMBEROFPOPULATION];
	/*float graph[NUMBEROFCITIES][NUMBEROFCITIES] = {
    {0, 281, 176, 243, 106},
    {281, 0, 25, 192, 65},
    {176, 25, 0, 193, 17},
    {243, 192, 193, 0, 138},
    {106, 65, 17, 138, 0},
};*/


/*float graph[NUMBEROFCITIES][NUMBEROFCITIES] = {
    {0, 38, 146, 194, 68, 35, 19, 64, 75, 73},
    {38, 0, 270, 202, 172, 69, 242, 59, 280, 144},
    {146, 270, 0, 255, 105, 176, 256, 6, 123, 137},
    {194, 202, 255, 0, 5, 246, 201, 99, 177, 232},
    {68, 172, 105, 5, 0, 253, 24, 222, 272, 144},
    {35, 69, 176, 246, 253, 0, 99, 31, 153, 237},
    {19, 242, 256, 201, 24, 99, 0, 65, 105, 214},
    {64, 59, 6, 99, 222, 31, 65, 0, 65, 258},
    {75, 280, 123, 177, 272, 153, 105, 65, 0, 5},
    {73, 144, 137, 232, 144, 237, 214, 258, 5, 0},
};
*/
float graph[NUMBEROFCITIES][NUMBEROFCITIES] = {
    {0, 75, 265, 192, 161, 61, 62, 203, 183, 207},
    {75, 0, 157, 275, 141, 60, 160, 57, 70, 208},
    {265, 157, 0, 66, 165, 291, 154, 105, 220, 109},
    {192, 275, 66, 0, 41, 139, 170, 190, 1, 152},
    {161, 141, 165, 41, 0, 192, 151, 138, 53, 109},
    {61, 60, 291, 139, 192, 0, 5, 191, 195, 46},
    {62, 160, 154, 170, 151, 5, 0, 54, 258, 234},
    {203, 57, 105, 190, 138, 191, 54, 0, 211, 101},
    {183, 70, 220, 1, 53, 195, 258, 211, 0, 43},
    {207, 208, 109, 152, 109, 46, 234, 101, 43, 0},
};






	displayGraph(graph);
	printf("===================================================\n");

	printf("Bien verifier les paramètres !! \n");
	printf("Nombre de tours : %d\n",nbIteration);
	printf("Nombre de populations : %d\n",NUMBEROFPOPULATION);
	printf("Nombre de sommets : %d\n",NUMBEROFCITIES);
	printf("offset pour croisement : %d\n",OFFSET);
	printf("===================================================\n");


	int **population = (int **)malloc(NUMBEROFPOPULATION * sizeof(int *));
	for (int i = 0; i < NUMBEROFPOPULATION; i++)
	{
		population[i] = (int *)malloc(sizeof(int) * NUMBEROFCITIES);
	}

	// generer aléatoirement la population
	createPopulation(population);

	for (int k = 0; k<nbIteration; k++)
	{

		// création de la nouvelle de la nouvelle population
		int **newPopulation = (int **)malloc(NUMBEROFPOPULATION * sizeof(int *));
		for (int i = 0; i < NUMBEROFPOPULATION; i++)
		{
			newPopulation[i] = (int *)malloc(sizeof(int) * NUMBEROFCITIES);
		}

		for (int j = 0; j < NUMBEROFPOPULATION; j++)
		{

			// calculer f(n) de chaque chromosome
			for (int i = 0; i < NUMBEROFPOPULATION; i++)
			{
				fitnessValues[i] = calculatFitness(population[i], graph);
			}

			// choisir les parents avec une proba qui augument avec f(n) ==> Wheel selection
			parents = selectParents(population, fitnessValues);

			// calculer le genome fils n*
			genome = getGenome(population[parents[0]], population[parents[1]], OFFSET);

			// effectuer la mutation au genome avec une probabilité de 20%
			randValue = (float)rand() / RAND_MAX;

			if (randValue < 0.2)
			{
				mutateGenome(genome);
			}

			copyArray(newPopulation[j], genome, NUMBEROFCITIES);

			sumFitness = calculatFitness(genome, graph);

			if (sumFitness < optimalValue)
			{
				optimalValue = sumFitness;
				copyArray(optimalPath, genome, NUMBEROFCITIES);
			}

			free(genome);
			free(parents);
			genome = NULL;
			parents = NULL;
		}

		// liberer m'éspace mémoire
		for (int i = 0; i < NUMBEROFPOPULATION; i++)
		{
			free(population[i]);
		}
		free(population);
		population = newPopulation;
		newPopulation = NULL;
	}
	
	printf("Estimated Optimal value is %.2f \n", optimalValue);
	printArray(optimalPath, NUMBEROFCITIES);
	for (int i = 0; i < NUMBEROFCITIES; i++)
	{
		printf("{%d ==> %d = %.2f}\n", optimalPath[i], optimalPath[(i + 1) % NUMBEROFCITIES], graph[optimalPath[i]][optimalPath[(i + 1) % NUMBEROFCITIES]]);
	}
}

int main(int argc, char const *argv[])
{

	algorithmeGenetique(100000);

	return 0;
}