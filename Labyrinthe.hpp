#ifndef LABYRINTHE_MULTI_HPP
#define LABYRINTHE_MULTI_HPP

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <tuple>
#include <algorithm>

using namespace std;

class Labyrinthe {
private:
    // Structure pour représenter le labyrinthe complet avec tous les niveaux
    vector<vector<string>> niveaux;  // Chaque niveau est une grille
    int largeur;
    int hauteur;
    int nbNiveaux;

    // Positions importantes
    vector<pair<int, int>> positionsTeleporteurs1;  // Positions des téléporteurs 1 pour chaque niveau
    vector<pair<int, int>> positionsTeleporteurs2;  // Positions des téléporteurs 2 pour chaque niveau
    vector<pair<int, int>> positionsDynamite;       // Positions des dynamites T pour chaque niveau

    // Backtracking pour explorer le labyrinthe
    bool backtrackingMultiNiveau(
        int x, int y, int niveau,
        vector<pair<int, int>>& objetsCollectes,
        vector<tuple<int, int, int>>& chemin,
        set<tuple<int, int, int>>& visite,
        bool murExplose
    );

    // Fonctions helper
    bool tousObjetsCollectes(const vector<pair<int, int>>& objetsCollectes) const;
    bool contientPosition(const vector<pair<int, int>>& positions, int x, int y) const;
    vector<pair<int, int>> getVoisinsAccessibles(int x, int y, int niveau, bool murExplose) const;

public:
    Labyrinthe();
    Labyrinthe(const vector<vector<string>>& niveaux);

    // Chargement des labyrinthes depuis un fichier
    static Labyrinthe loadFile(const string& nomFichier);

    // Affichage
    void afficherLabyrinthe() const;
    void afficherLabyrintheAvecChemin(const vector<tuple<int, int, int>>& chemin) const;

    // Positions de départ et d'arrivée
    pair<int, int> positionDepart;
    int niveauDepart;
    pair<int, int> positionArrivee;
    int niveauArrivee;
    vector<tuple<int, int, int>> positionsObjets;  // (x, y, niveau)

    // Initialisation des positions
    void initPositions();

    // Méthodes utilitaires
    bool isValidPosition(int x, int y, int niveau) const;
    bool isWall(int x, int y, int niveau) const;
    char getCase(int x, int y, int niveau) const;
    bool isObject(int x, int y, int niveau) const;

    // Résolution du labyrinthe
    vector<tuple<int, int, int>> resolve();  // Retourne un chemin: (x, y, niveau)
};

#endif