#ifndef LARBYRINTHE_HPP
#define LARBYRINTHE_HPP
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Labyrinthe {

    private:
        vector<string> grille;
        int largeur;
        int hauteur;

    public :
        Labyrinthe();
        Labyrinthe(const vector<string>& grille);
        // ~Labyrinthe();
        
        static std::vector<Labyrinthe> loadFile(const string& nomFichier);

        void afficherLabyrinthe() const;

        // Getters
        int getLargeur() const { return largeur; }
        int getHauteur() const { return hauteur; }
        const vector<string>& getGrille() const { return grille; }
        pair<int, int> getDepart() const { return positionDepart; }
        pair<int, int> getArrivee() const { return positionArrivee; }
        vector<pair<int, int>> getObjets() const { return positionsObjets; }

        // Setters
        void setLargeur(int largeur) { this->largeur = largeur; }
        void setHauteur(int hauteur) { this->hauteur = hauteur; }
        void setGrille(const vector<string>& grille) { this->grille = grille; }

        // Positions
        pair<int, int> positionDepart;
        pair<int, int> positionArrivee;
        vector<pair<int, int>> positionsObjets;
        
        // Initialisation des positions
        void initPositions();

        // Méthodes utilitaires
        bool isValidPosition(int x, int y) const;
        bool isWall(int x, int y) const;
        char getCase(int x, int y) const;
        char getCase(const pair<int, int>& position) const;
        bool isObject(int x, int y) const;


};

#endif