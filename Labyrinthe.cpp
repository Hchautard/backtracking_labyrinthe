#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Labyrinthe.hpp"

using namespace std;

Labyrinthe::Labyrinthe() : largeur(0), hauteur(0) {}

void Labyrinthe::afficherLabyrinthe() const {
    for (const auto& ligne : grille) {
        std::cout << ligne << std::endl;
    }
}

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

// Initialise les positions (départ, arrivée, objets)
void Labyrinthe::initPositions() {

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            char c = grille[y][x];
            
            if (c == 'D') {
                positionDepart = make_pair(x, y);
            } else if (c == 'A') {
                positionArrivee = make_pair(x, y);
            } else if (c == 'C' || c == 'B' || c == 'E') {
                // C = Couronne, B = Bouclier, E = Épée
                positionsObjets.push_back(make_pair(x, y));
            }
        }
    }
}

// Récupère la case depuis des coordonnées
char Labyrinthe::getCase(const pair<int, int>& position) const {
    return getCase(position.first, position.second);
}

// Récupère la case
char Labyrinthe::getCase(int x, int y) const {
    if (isValidPosition(x, y)) {
        return grille[y][x];
    }
    return ' '; 
}

// Vérifie si la position
bool Labyrinthe::isValidPosition(int x, int y) const {

    if(x >= 0 && x < largeur && y >= 0 && y < hauteur) {
        return true;
    }

    return false;
}

// Vérifie si c'est un mur
bool Labyrinthe::isWall(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return true;
    }
    return grille[y][x] == '#';
}

// Vérifie si c'est un objet
bool Labyrinthe::isObject(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    char c = grille[y][x];
    return (c == 'C' || c == 'B' || c == 'E');
}
