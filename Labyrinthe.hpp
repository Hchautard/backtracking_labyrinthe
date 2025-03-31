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

        void afficherLabyrinthe() const {
            for (const auto& ligne : grille) {
                cout << ligne << endl;
            }
        }

};

#endif