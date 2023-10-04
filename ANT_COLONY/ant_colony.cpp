
#include <iomanip>
#include "ant_colony.hpp"



void displayGraph(float matrix[NUMBEROFCITIES][NUMBEROFCITIES]) {
    printf("\t");
    for (int j = 0; j < NUMBEROFCITIES; j++) {
        printf("[%d]\t", j);
    }
    printf("\n");
    
    for (int i = 0; i < NUMBEROFCITIES; i++) {
        printf("[%d]\t", i);
        for (int j = 0; j < NUMBEROFCITIES; j++) {
            printf("%.2f\t", matrix[i][j]);
        }
        printf("\n");
    }
}
// la fonction retoune l'indice du chemin le plus probable d'obtenir
Sommet *selectionWheel(std::vector<float> probabilites, std::vector<Arete *> voisinsAchoisir)
{
    std::vector<float> cumulativeProbabilities;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    float randomNum = dis(gen); // générer un nombre aléatoire de 0 à 1.
    float sum = 0;
    int j = 0;

    for (float i : probabilites)
    {
        sum += i;
        cumulativeProbabilities.push_back(sum);
    }

    for (; j < (int)cumulativeProbabilities.size(); j++)
    {
        if (randomNum <= cumulativeProbabilities[j])
        {
            return voisinsAchoisir.at(j)->getEnd();
        }
    }
    // j sera l'indice la ville suivante dans le cas ou il reste une seule ville
    return voisinsAchoisir.at(j - 1)->getEnd();
}

float calculerProbabilite(Arete *arete, std::vector<Arete *> voisins, int alpha, int beta)
{
    float somme = 0;
    float phermone = 0;
    float distance = 0;
    for (int i = 0; i < (int)voisins.size(); i++)
    {
        if (voisins.at(i)->getEnd()->getEstVisite())
            continue; // ville déjà visitée
        phermone = voisins.at(i)->getPhermone();
        distance = voisins.at(i)->getDistance();
        somme += pow(phermone, alpha) * pow((1 / distance), beta);
    }
    return ((pow(arete->getPhermone(), alpha)) * (pow(1 / arete->getDistance(), beta))) / somme;
}


Solution calculerCheminParUneFourmie(std::vector<Sommet *> graph, Arete *aretes[NUMBEROFCITIES][NUMBEROFCITIES], int alpha, int beta)
{
    std::srand(std::time(nullptr)); // Seed the random number generator
    Sommet *villeSuivante = NULL;
    int r =  std::rand() % graph.size();  //positioner la fourmis dans une ville aléatoire
    float distance = 0;
    std::vector<float> probabilites; // tableau de probabilité de chaque arête entre une ville de départ i et le sommet adjacent
    std::vector<Sommet *> path;
    std::vector<Arete *> voisinsAchoisir;
    Sommet *villeDepart = graph.at(r);
    villeDepart->marquer();
    path.push_back(villeDepart);

    // tant qu'on a pas parcouru toutes les graph
    for (int i = 0; i < (int)graph.size() - 1; i++)
    {
        // on calcule les probas pour chaque arete
        for (Arete *a : villeDepart->getNeighbors())
        {
            if (a->getEnd()->getEstVisite())
            {
                continue;
            }
            else
            {
                voisinsAchoisir.push_back(a);
            }
            probabilites.push_back(calculerProbabilite(a, villeDepart->getNeighbors(), alpha, beta));
        }
        villeSuivante = selectionWheel(probabilites, voisinsAchoisir);
        villeSuivante->marquer();

        // on choisit une ville qui n'a pas été visitée

        probabilites.clear();
        voisinsAchoisir.clear();
        distance += aretes[villeDepart->getIndice()][villeSuivante->getIndice()]->getDistance();
        path.push_back(villeSuivante);

        villeDepart = villeSuivante;
        villeSuivante = NULL;
    }
    // rénitialiser est visité à false

    for (Sommet *s : graph)
    {
        s->setEstVisite(false);
    }
    distance += aretes[path.at(0)->getIndice()][path.at(path.size() - 1)->getIndice()]->getDistance();
    path.push_back(path.at(0));
    return {path, distance};
}



Solution algorithm(std::vector<Sommet *> graph, Arete *aretes[NUMBEROFCITIES][NUMBEROFCITIES], int nbrDeTours, int nbrFourmis, int alpha, int beta, float gho)
{
    // Solution sommetsParcourus;
    std::vector<Solution> cheminDesFourmis;
    Solution solutionOptimale;
    solutionOptimale.distance = INT_MAX;

    for (int k = 0; k < nbrDeTours; k++)
    {
        //ajouter tous les chemins parcourus dans un vecteur
        for (int j = 0; j < nbrFourmis; j++)
        {

            cheminDesFourmis.push_back(calculerCheminParUneFourmie(graph, aretes, alpha, beta));
        }

        // Reduire les  phermones par un GHO

        for (int i = 0; i < NUMBEROFCITIES; i++)
        {
            for (int j = 0; j < NUMBEROFCITIES; j++)
            {
                if (j == i)
                    continue;
                aretes[i][j]->setPhermone((1 - gho) * aretes[i][j]->getPhermone());
            }
        }
        // ajouer le niveau de phermone pour chaque fourmie

        for (Solution s : cheminDesFourmis)
        {

            for (int i = 0; i < (int)s.chemin.size() - 1; i++)
            {
                int cur = s.chemin.at(i)->getIndice();
                int next = s.chemin.at(i + 1)->getIndice();
                aretes[cur][next]->setPhermone(aretes[cur][next]->getPhermone() + (1 / s.distance));
            }

            // verifier la solution optimale

            if (s.distance < solutionOptimale.distance)
            {
                solutionOptimale = s;
            }
        }
    }
    return solutionOptimale;
}

int main(int argc, char const *argv[])
{
    // creer les graph
    std::vector<Sommet *> graph;
    std::vector<Sommet *> path;
    int alpha =1 ;
    int beta =1;
    float gho =0.5;
    int nombreDeFourmis=50 ;
    int nombreDeTours=1000;

    for (int i = 0; i < NUMBEROFCITIES; i++)
    {
        graph.push_back(new Sommet(i));
    }

    // definir une matrice de distance

  /*float distances[NUMBEROFCITIES][NUMBEROFCITIES] = {
    {0, 281, 176, 243, 106},
    {281, 0, 25, 192, 65},
    {176, 25, 0, 193, 17},
    {243, 192, 193, 0, 138},
    {106, 65, 17, 138, 0},
};*/

/*float distances[NUMBEROFCITIES][NUMBEROFCITIES] = {
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
};*/

    float distances[NUMBEROFCITIES][NUMBEROFCITIES] = {
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




    Arete *aretes[NUMBEROFCITIES][NUMBEROFCITIES];

    // création du graphe
    for (int i = 0; i < NUMBEROFCITIES; i++)
    {
        for (int j = 0; j < NUMBEROFCITIES; j++)
        {
            if (j == i)
                continue;
            else
            {
                aretes[i][j] = new Arete{graph.at(j), distances[i][j], 1};
                graph.at(i)->ajouterVoisin(aretes[i][j]);
            }
        }
    }

    displayGraph(distances);
    printf("===================================================\n");

    printf("Bien verifier les paramètres !! \n");
    printf("Nombre de tours : %d\n",nombreDeTours);
    printf("Nombre de fourmis : %d\n",nombreDeFourmis);
    printf("Nombre de sommets : %d\n",NUMBEROFCITIES);
    printf("Alpha %d, Beta %d, Gho %.2f \n",alpha, beta, gho);
    printf("===================================================\n");

    Solution solutionOptimale = algorithm(graph, aretes, nombreDeTours, nombreDeFourmis, alpha, beta, gho);
    std::cout << "====================================================== " << std::endl;

    std::cout << "Estimated Optimal value is: " << solutionOptimale.distance <<std::endl;

    for (int i = 0; i < (int)solutionOptimale.chemin.size()-1; i++)
    {
        std::cout << solutionOptimale.chemin.at(i)->getIndice() << " -->  ";
    }
        std::cout << solutionOptimale.chemin.at(solutionOptimale.chemin.size()-1)->getIndice() << std::endl;

    std::cout << "====================================================== " << std::endl;
        
    for (int i=0; i<(int)solutionOptimale.chemin.size(); i++){
        auto curr = solutionOptimale.chemin.at(i);
        auto next = solutionOptimale.chemin.at((i+1)%solutionOptimale.chemin.size());

        for (auto a : curr->getNeighbors()){
            if (a->getEnd()->getIndice() == next->getIndice()){
                std::cout << "{"<<curr->getIndice() << " -> " << next->getIndice() <<"} : "<< a->getDistance()<< std::endl;
                break ;
            }
        }
    }


    return 0;
}