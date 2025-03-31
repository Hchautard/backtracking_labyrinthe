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

    return 0;
}