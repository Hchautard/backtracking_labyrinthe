#include <iostream>
#include <string>

#include "Labyrinthe.hpp"

using namespace std;

int main()
{
    // Chargement des labyrinthes depuis un fichier
    string nomFichier = "labyrinthe.txt";
    vector<Labyrinthe> labyrinthes = Labyrinthe::loadFile(nomFichier);

    cout << "Nombre de labyrinthes : " << labyrinthes.size() << endl;

    // Affichage de chaque labyrinthe
    for (size_t i = 0; i < labyrinthes.size(); ++i) {
        cout << "Labyrinthe " << i + 1 << ":" << endl;
        labyrinthes[i].afficherLabyrinthe();
        cout << endl;
    }

    return 0;
}