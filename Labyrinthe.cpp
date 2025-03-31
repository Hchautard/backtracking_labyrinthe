#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Labyrinthe.hpp"

using namespace std;

Labyrinthe::Labyrinthe() : largeur(0), hauteur(0) {}

// Création avec une liste d'initialisation
Labyrinthe::Labyrinthe(const vector<string>& _grille) : grille(_grille) {
    if (!grille.empty()) {
        hauteur = grille.size();
        largeur = grille[0].size();
    } else {
        hauteur = 0;
        largeur = 0;
    }
}
// Charge les labyrinthes depuis un fichier
vector<Labyrinthe> Labyrinthe::loadFile(const string &nomFichier)
{
    // Lecture du fichier
    ifstream fichier(nomFichier);
    if (!fichier.is_open())
    {
        throw runtime_error("Impossible d'ouvrir le fichier: " + nomFichier);
    }

    vector<Labyrinthe> labyrinthes;
    vector<string> grilleActuelle;
    string ligne;

    while (getline(fichier, ligne))
    {
        // Si la ligne est vide et que la grille actuelle n'est pas vide, 
        // On considère que c'est la fin d'un labyrinthe
        // On l'ajoute à la liste des labyrinthes
        if (ligne.empty() && !grilleActuelle.empty())
        {
            labyrinthes.push_back(Labyrinthe(grilleActuelle));

            grilleActuelle.clear();
            continue;
        }

        // Si la ligne est vide on l'ignore
        if (ligne.empty())
        {
            continue;
        }

        grilleActuelle.push_back(ligne);
    }

    if (!grilleActuelle.empty())
    {
        labyrinthes.push_back(Labyrinthe(grilleActuelle));
    }

    fichier.close();
    return labyrinthes;
}